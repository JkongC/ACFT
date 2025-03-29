export module Renderer:GLIndexBuilder;

import <vector>;
import Tesselator;

namespace GLImplementations
{
	using namespace ACFT;

	export constexpr size_t defualtIndexCount = 3000;
	
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

		static constexpr std::vector<unsigned int> GetDefaultBuild(Primitive primitive);

	private:
		IndexBuilder(Primitive primitive) : m_Primitive(primitive) {}

	private:
		Primitive m_Primitive;
		unsigned int m_IndexCount{ 0 };
		int m_Step{ 1 };
	};
}

