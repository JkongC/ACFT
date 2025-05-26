export module ACFT.FrameBuffer;

import Types;

namespace ACFT
{
	export class FrameBuffer
	{
	public:
		static Ref<FrameBuffer> Create();

		RenderObjectIdentifier GetIdentifier() const { return m_Identifier; }

	private:
		RenderObjectIdentifier m_Identifier;
	};
}