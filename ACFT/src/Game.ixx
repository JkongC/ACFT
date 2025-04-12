export module Game;

import Renderer;
import Window;

namespace ACFT
{
	export class Game
	{
	public:
		int Run();

	private:
		Ref<Renderer> m_Renderer;
		Ref<Window> m_Window;
	};
}