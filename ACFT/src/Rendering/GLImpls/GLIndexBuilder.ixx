export module Renderer:GLIndexBuilder;

export import <vector>;
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
					rs.push_back(0 + group_idx * 4);
					rs.push_back(1 + group_idx * 4);
					rs.push_back(2 + group_idx * 4);
					rs.push_back(0 + group_idx * 4);
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
}


namespace GLImplementations
{
	using namespace ACFT;
	using namespace IndexMode;

	export constexpr size_t defaultIndexCount = 3000;
	
	export class IndexBuilder
	{
	public:
		static IndexBuilder Create(Primitive primitive);
		
		/* If parameter "count" cannot be devided by the count of indices per primitive
		* should has, the ultimate size of index list will be cut to fit. Notice that.
		*/
		IndexBuilder& IndexCount(unsigned int count);
		IndexBuilder& Step(int step);
		std::vector<unsigned int> Build();

		static constexpr std::vector<unsigned int> GetDefaultBuild(Primitive primitive)
		{
			switch (primitive)
			{
			case triangle_fan:
				return IndexGenerator<TriangleFan>().Generate(defaultIndexCount);
				break;
			case square:
				return IndexGenerator<Square>().Generate(defaultIndexCount);
				break;
			case cube:
				return IndexGenerator<Cube>().Generate(defaultIndexCount);
				break;
			default:
				return IndexGenerator<Basic>().Generate(defaultIndexCount);
				break;
			}
		}

	private:
		IndexBuilder(Primitive primitive) : m_Primitive(primitive) {}

	private:
		Primitive m_Primitive;
		unsigned int m_IndexCount{ 0 };
		int m_Step{ 1 };
	};
}

