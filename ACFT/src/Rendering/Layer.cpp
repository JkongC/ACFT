module Layer;

import <mutex>;
import <vector>;
import <unordered_set>;

import Types;
import Event;
import Log;

namespace ACFT
{
	void Layer::CaptureEventType(const Ref<EventType>& type)
	{
		m_CaptureEventTypes.insert(type);
	}

	void Layer::ReleaseEventType(const Ref<EventType>& type)
	{
		if (auto it = m_CaptureEventTypes.find(type); it != m_CaptureEventTypes.end())
			m_CaptureEventTypes.erase(it);
	}

	
	Ref<Layer> LayerStack::PopLayer()
	{
		Ref<Layer> layer = m_Layers.back();
		m_Layers.pop_back();
		return layer;
	}

	Ref<LayerStack> LayerStack::Create()
	{
		LayerStack* stack_ptr = new LayerStack();
		Ref<LayerStack> stack = Ref<LayerStack>(stack_ptr);
		EventManager& event_manager = EventManager::Global();
		auto callback = [stk = stack.get()](Ref<Event> event) -> void {stk->DispatchEvent(event);};
		for (Ref<EventType> type : EventRegistry::GetAllTypes())
		{
			event_manager.Subscribe(stack, type, callback);
		}
		
		return stack;
	}

	void LayerStack::DispatchEvent(Ref<Event> event)
	{
		std::lock_guard<std::mutex> lock(m_Mtx);
		for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); it++)
		{
			Ref<Layer>& layer = *it;
			if (layer->OnEvent(event))
				break;
		}
	}

	Ref<Layer> LayerStack::At(size_t index)
	{
		if (index > GetSize() - 1)
		{
			ACFT_LOG_WARN("[LayerStack] Index exceeded. Returning the top layer of the stack.");
			return At(GetSize() - 1);
		}

		return m_Layers.at(index);
	}

	void LayerStack::OnUpdate(float time_step)
	{
		for (auto& layer : m_Layers)
		{
			layer->OnUpdate(time_step);
		}
	}

	void LayerStack::OnRender()
	{
		for (auto& layer : m_Layers)
		{
			layer->OnRender();
		}
	}
}
