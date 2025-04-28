module Renderer:GLIndexBuilder;

namespace GLImplementations
{
	using namespace IndexMode;
	using namespace ACFT;

	IndexBuilder IndexBuilder::Create(Primitive primitive)
	{
		return IndexBuilder(primitive);
	}

	IndexBuilder& IndexBuilder::IndexCount(unsigned int count)
	{
		this->m_IndexCount = count / IndexCountPerPrimitive(this->m_Primitive) * IndexCountPerPrimitive(this->m_Primitive);
		return *this;
	}

	IndexBuilder& IndexBuilder::Step(int step)
	{
		this->m_Step = step;
		return *this;
	}

	std::vector<unsigned int> IndexBuilder::Build()
	{
		switch (this->m_Primitive)
		{
		case triangle_fan:
			return IndexGenerator<TriangleFan>().Generate(this->m_IndexCount);
			break;
		case square:
			return IndexGenerator<Square>().Generate(this->m_IndexCount);
			break;
		case cube:
			return IndexGenerator<Cube>().Generate(this->m_IndexCount);
			break;
		default:
			return IndexGenerator<Basic>().Generate(this->m_IndexCount);
			break;
		}
	}
}