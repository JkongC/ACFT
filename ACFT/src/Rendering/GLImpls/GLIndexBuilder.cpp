module Renderer:GLIndexBuilder;

import <vector>;
import Tesselator;

namespace GLImplementations
{
	using namespace ACFT;
	
	template<typename T>
	class IndexGenerator
	{
	public:
		constexpr std::vector<unsigned int> Generate(unsigned int count, int step = 1)
		{
			return static_cast<T*>(this)->GenerateIndex(count, step);
		}

		constexpr IndexGenerator() = default;
	};

	namespace IndexMode
	{
		class Basic : public IndexGenerator<Basic>
		{
		public:
			constexpr std::vector<unsigned int> GenerateIndex(unsigned int count, int step)
			{
				std::vector<unsigned int> rs;
				rs.reserve(count);
				for (unsigned int group_idx = 0; group_idx < count; group_idx++)
				{
					rs.push_back(group_idx);
				}

				return rs;
			}
		};

		class TriangleFan : public IndexGenerator<TriangleFan>
		{
		public:
			constexpr std::vector<unsigned int> GenerateIndex(unsigned int count, int step)
			{
				std::vector<unsigned int> rs;
				rs.reserve(count);
				for (unsigned int group_idx = 0; group_idx < count / IndexCountPerPrimitive(triangle_fan); group_idx++)
				{
					rs.push_back(0);
					rs.push_back(1 + group_idx);
					rs.push_back(2 + group_idx);
				}

				return rs;
			}
		};

		class Square : public IndexGenerator<Square>
		{
		public:
			constexpr std::vector<unsigned int> GenerateIndex(unsigned int count, int step)
			{
				std::vector<unsigned int> rs;
				rs.reserve(count);
				for (unsigned int group_idx = 0; group_idx < count / IndexCountPerPrimitive(square); group_idx++)
				{
					rs.push_back(0);
					rs.push_back(1 + group_idx * 4);
					rs.push_back(2 + group_idx * 4);
					rs.push_back(0);
					rs.push_back(2 + group_idx * 4);
					rs.push_back(3 + group_idx * 4);
				}

				return rs;
			}
		};

		class Cube : public IndexGenerator<Cube>
		{
		public:
			constexpr std::vector<unsigned int> GenerateIndex(unsigned int count, int step)
			{
				std::vector<unsigned int> rs;
				rs.reserve(count);
				for (unsigned int group_idx = 0; group_idx < count / IndexCountPerPrimitive(cube); group_idx++)
				{
					//Up
					rs.push_back(0 + group_idx * 8);
					rs.push_back(1 + group_idx * 8);
					rs.push_back(2 + group_idx * 8);
					rs.push_back(0 + group_idx * 8);
					rs.push_back(2 + group_idx * 8);
					rs.push_back(3 + group_idx * 8);
					//Down
					rs.push_back(5 + group_idx * 8);
					rs.push_back(4 + group_idx * 8);
					rs.push_back(7 + group_idx * 8);
					rs.push_back(5 + group_idx * 8);
					rs.push_back(7 + group_idx * 8);
					rs.push_back(6 + group_idx * 8);
					//Front
					rs.push_back(1 + group_idx * 8);
					rs.push_back(5 + group_idx * 8);
					rs.push_back(6 + group_idx * 8);
					rs.push_back(1 + group_idx * 8);
					rs.push_back(6 + group_idx * 8);
					rs.push_back(2 + group_idx * 8);
					//Back
					rs.push_back(3 + group_idx * 8);
					rs.push_back(7 + group_idx * 8);
					rs.push_back(4 + group_idx * 8);
					rs.push_back(3 + group_idx * 8);
					rs.push_back(4 + group_idx * 8);
					rs.push_back(0 + group_idx * 8);
					//Left
					rs.push_back(0 + group_idx * 8);
					rs.push_back(4 + group_idx * 8);
					rs.push_back(5 + group_idx * 8);
					rs.push_back(0 + group_idx * 8);
					rs.push_back(5 + group_idx * 8);
					rs.push_back(1 + group_idx * 8);
					//Right
					rs.push_back(2 + group_idx * 8);
					rs.push_back(6 + group_idx * 8);
					rs.push_back(7 + group_idx * 8);
					rs.push_back(2 + group_idx * 8);
					rs.push_back(7 + group_idx * 8);
					rs.push_back(3 + group_idx * 8);
				}

				return rs;
			}
		};
	}

	using namespace IndexMode;

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

	constexpr std::vector<unsigned int> IndexBuilder::GetDefaultBuild(Primitive primitive)
	{
		switch (primitive)
		{
		case triangle_fan:
			return IndexGenerator<TriangleFan>().Generate(defualtIndexCount);
			break;
		case square:
			return IndexGenerator<Square>().Generate(defualtIndexCount);
			break;
		case cube:
			return IndexGenerator<Cube>().Generate(defualtIndexCount);
			break;
		default:
			return IndexGenerator<Basic>().Generate(defualtIndexCount);
			break;
		}
	}
}