module ACFT.ScreenQuadTree;

import Log;
import Window;
import Renderer;
import Input;
import ACFT.Screen;

namespace ACFT::UI
{
	static QuadArea JudgeAreaInRectByPos(const UIAreaRect& rect, const ElementLocation& point)
	{
		auto width = Renderer::GetWindow()->GetWidth();
		auto height = Renderer::GetWindow()->GetHeight();

		bool left = point.x <= width * (rect.left + rect.right) / 2;
		bool top = point.y <= height * (rect.top + rect.bottom) / 2;

		if (left && top)
			return QuadArea::top_left;
		else if (!left && top)
			return QuadArea::top_right;
		else if (left && !top)
			return QuadArea::bottom_left;
		else
			return QuadArea::bottom_right;
	}

	static UIAreaRect CutAreaInRect(const UIAreaRect& rect, QuadArea area)
	{
		UIAreaRect ret{};
		ret.type = LocationType::percentage;
		switch (area)
		{
		case ACFT::UI::QuadArea::bottom_right:
			ret.right = rect.right;
			ret.bottom = rect.bottom;
			ret.left = (rect.right + rect.left) / 2.0f;
			ret.top = (rect.top + rect.bottom) / 2.0f;
			break;
		case ACFT::UI::QuadArea::bottom_left:
			ret.left = rect.left;
			ret.bottom = rect.bottom;
			ret.right = (rect.right + rect.left) / 2.0f;
			ret.top = (rect.top + rect.bottom) / 2.0f;
			break;
		case ACFT::UI::QuadArea::top_right:
			ret.top = rect.top;
			ret.right = rect.right;
			ret.bottom = (rect.top + rect.bottom) / 2.0f;
			ret.left = (rect.right + rect.left) / 2.0f;
			break;
		case ACFT::UI::QuadArea::top_left:
			ret.top = rect.top;
			ret.left = rect.left;
			ret.bottom = (rect.top + rect.bottom) / 2.0f;
			ret.right = (rect.right + rect.left) / 2.0f;
			break;
		}

		return ret;
	}
	
	ScreenQuadTreeNode::ScreenQuadTreeNode(UIAreaRect area)
	{
		if (area.type != LocationType::percentage)
		{
			ACFT_LOG_ERROR("ScreenQuadTreeNode's area must be represented by percentage!");
			area.type = LocationType::percentage;
			area.top = 0.0f;
			area.bottom = 1.0f;
			area.left = 0.0f;
			area.right = 1.0f;
		}

		m_Area = area;
	}

	ScreenQuadTreeNode::ScreenQuadTreeNode(const ScreenQuadTreeNode& parent, QuadArea area)
		: m_Area(CutAreaInRect(parent.m_Area, area))
	{ }

	void ScreenQuadTreeNode::SpawnChildren(int layers_left)
	{
		if (layers_left == 0)
		{
			m_Leaf = true;
			return;
		}
		
		for (int i = 0; i < 4; i++)
		{
			m_Children.at(i) = MakeScope<ScreenQuadTreeNode>(*this, static_cast<QuadArea>(i));
			m_Children.at(i)->m_Parent = this;
			m_Children.at(i)->SpawnChildren(layers_left);
		}
	}

	void ScreenQuadTreeNode::AddElement(Ref<UIElement>& element)
	{
		if (!m_Leaf)
		{
			const auto& location = element->GetLocation();
			m_Children.at(static_cast<int>(JudgeAreaInRectByPos(m_Area, location)))->AddElement(element);
		}
		else
		{
			m_Elements.push_back(element);
		}
	}

	void ScreenQuadTreeNode::OnEvent(Ref<Event>& event, const ElementLocation& pos)
	{
		if (!m_Leaf)
		{
			m_Children.at(static_cast<int>(JudgeAreaInRectByPos(m_Area, pos)))->OnEvent(event, pos);
		}
		else
		{
			for (auto& ele : m_Elements)
			{
				ele->OnEvent(event);
			}
		}
	}

	ScreenQuadTree::ScreenQuadTree(int depth)
	{
		UIAreaRect full{};
		full.top = 0.0f;
		full.left = 0.0f;
		full.right = 1.0f;
		full.bottom = 1.0f;
		full.type = LocationType::percentage;

		m_Root = MakeScope<ScreenQuadTreeNode>(full);
		m_Root->SpawnChildren(depth > maxDepth ? maxDepth : depth);
	}

	void ScreenQuadTree::AddElement(Ref<UIElement> element)
	{
		m_Root->AddElement(element);
	}

	void ScreenQuadTree::OnEvent(Ref<Event> event)
	{
		auto [x, y] = GetCursorPos(Renderer::GetWindow());
		ElementLocation pos{ static_cast<float>(x), static_cast<float>(y) };
		m_Root->OnEvent(event, pos);
	}
}
