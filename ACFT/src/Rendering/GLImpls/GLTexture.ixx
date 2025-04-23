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

		static void Bind(unsigned int id);
		static void Unbind(unsigned int id);

		void Bind() const;
		void Unbind() const;

	private:
		int m_Width;
		int m_Height;
		unsigned int m_ID;
	};
}
