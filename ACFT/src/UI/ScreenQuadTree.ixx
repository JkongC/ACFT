export module ACFT.ScreenQuadTree;

import <array>;
import <vector>;

import Types;
import Event;
import Input;
import ACFT.UIBase;

namespace ACFT::UI
{
	class Screen;
	class Button;
	class Menu;

	export enum class QuadArea
	{
		top_left = 0, top_right, bottom_left, bottom_right 
	};
	
	export class ScreenQuadTreeNode
	{
	public:
		ScreenQuadTreeNode(Maths::Rect area);
		ScreenQuadTreeNode(const ScreenQuadTreeNode& parent, QuadArea area);

		void AddElement(Ref<UIElement>& element);

		void OnEvent(Ref<Event>& event, const ElementLocation& pos);

	private:
		friend class ScreenQuadTree;

		void SpawnChildren(int layers_left);

	private:
		std::array<Scope<ScreenQuadTreeNode>, 4> m_Children;
		std::vector<Ref<UIElement>> m_Elements;
		Maths::Rect m_Area;
		unsigned int m_ElementCount = 0;
		ScreenQuadTreeNode* m_Parent = nullptr;
		bool m_Leaf = false;
	};

	class ScreenQuadTree
	{
	public:
		static constexpr int maxDepth = 10;

		ScreenQuadTree(int depth);

		void AddElement(Ref<UIElement> menu);

		void OnEvent(Ref<Event> event);

	private:
		Scope<ScreenQuadTreeNode> m_Root;
	};
}
