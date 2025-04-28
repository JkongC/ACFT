module;

#include "Platform.h"

export module Layer;

import <vector>;
import <unordered_set>;
import <mutex>;

import Types;
import Event;

namespace ACFT
{
	class LayerStack;
	export class Layer
	{
	private:
		friend class LayerStack;
		ACFT_API Layer() = default;
		ACFT_API virtual ~Layer() = default;

		ACFT_API void CaptureEventType(const Ref<EventType>& type);
		ACFT_API void ReleaseEventType(const Ref<EventType>& type);

		/**
		 * Handle an event passed into the layer.
		 *
		 * \return Whether this event is captured by this layer;
		 */
		ACFT_API virtual bool HandleEvent(Ref<Event> event) { return false; }

		ACFT_API virtual void Tick(time_t time_step) {}

	protected:
		std::unordered_set<Ref<EventType>> m_CaptureEventTypes;
	};

	export class LayerStack
	{
	public:
		static Ref<LayerStack> Create();

		/**
		 * Push a new layer into the layer stack.
		 * 
		 * \param ...event_type The event types that this new layer will consume.
		 */
		template<typename... EventT>
			requires (std::is_same_v<EventT, Ref<EventType>> && ...)
		void PushLayer(const EventT&... event_type)
		{
			Layer* layer = new Layer();
			m_Layers.push_back(layer);
			(m_Layers.back().ConsumeEventType(event_type), ...);
		}

		Ref<Layer> PopLayer();

		Ref<Layer> At(size_t index);

		inline size_t GetSize() const { return m_Layers.size(); }

	private:
		LayerStack() = default;
		void DispatchEvent(Ref<Event> event);

	private:
		std::vector<Ref<Layer>> m_Layers;
		std::mutex m_Mtx;
	};
}