module;

#include "Platform.h"

export module Layer;

export import <vector>;
export import <unordered_set>;
export import <mutex>;

import Types;
import Event;

namespace ACFT
{
	class LayerStack;
	export class Layer
	{
	protected:
		friend class LayerStack;
		ACFT_API Layer() = default;
		ACFT_API virtual ~Layer() = default;

		ACFT_API void CaptureEventType(const Ref<EventType>& type);
		ACFT_API void ReleaseEventType(const Ref<EventType>& type);

	public:
		/**
		 * Handle an event passed into the layer.
		 *
		 * \return Whether this event is captured by this layer;
		 */
		ACFT_API virtual bool OnEvent(Ref<Event> event) { return false; }

		ACFT_API virtual void OnUpdate(float time_step) {}

		ACFT_API virtual void OnRender() {}

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
			requires (std::is_same_v<EventT, EventType> && ...)
		void PushLayer(Ref<Layer> layer, const Ref<EventT>&... event_type)
		{
			m_Layers.push_back(layer);
			(layer->CaptureEventType(event_type), ...);
		}

		Ref<Layer> PopLayer();

		Ref<Layer> At(size_t index);

		inline size_t GetSize() const { return m_Layers.size(); }

		void OnUpdate(float time_step);

		void OnRender();

	private:
		LayerStack() = default;
		void DispatchEvent(Ref<Event> event);

	private:
		std::vector<Ref<Layer>> m_Layers;
		std::mutex m_Mtx;
	};
}