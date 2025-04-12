export module Layer;

import <vector>;

namespace ACFT
{
	export class Layer
	{

	};

	export class LayerStack
	{
	public:
		LayerStack() = default;

		void PushLayer(Layer layer);
		void PopLayer();

	private:
		std::vector<Layer> m_Layers;
	};
}