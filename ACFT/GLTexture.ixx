export module Renderer:GLTexture;

import Types;
import Atlas;

namespace GLImplementations
{
	using namespace ACFT;

	export class GLTexture
	{
	public:
		GLTexture(Ref<Atlas> atlas);
		GLTexture(Ref<Image> img);
		GLTexture(GLTexture&&) = default;

		inline unsigned int GetID() const { return m_ID; }

	private:
		int m_Width;
		int m_Height;
		unsigned int m_ID;
	};
}
