#ifndef ACFT_VOXEL_H_
#define ACFT_VOXEL_H_

#include "Texture.h"
#include "Vertex.h"

namespace ACFT 
{
	template<unsigned int VertexCount>
	struct VoxelVertices
	{
		std::array<Vertex, VertexCount> vertices;
		unsigned int count = VertexCount;

		constexpr VoxelVertices(std::initializer_list<Vertex> vertex_list)
		{
			int index = 0;
			for (const Vertex& vtx : vertex_list)
			{
				vertices[index] = vtx;
				index++;
				if (index > VertexCount)
					break;
			}
		}
	};

	namespace VoxelVertexType
	{
		constexpr unsigned int normalCube = 24;
		
		constexpr VoxelVertices<normalCube> fullBlock
		{
			//FRONT
			{ -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0 },
			{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0 },
			{ 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0 },
			{ 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0 },
			//BACK
			{ 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0 },
			{ 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0 },
			{ -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0 },
			{ -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0 },
			//LEFT
			{ -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0 },
			{ -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0 },
			{ -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0 },
			{ -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0 },
			//RIGHT
			{ 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0 },
			{ 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0},
			{ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0 },
			{ 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0 },
			//UP
			{ -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0 },
			{ -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0 },
			{ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0 },
			{ 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0 },
			//DOWN
			{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0 },
			{ -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0 },
			{ 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0 },
			{ 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0 }
		};
	}
	
	template<unsigned int VertexCount>
	class Voxel
	{
	public:
		Voxel(const std::string& texture_filepath, float _length, const VoxelVertices<VertexCount>& vertex_type)
			:texture(texture_filepath), length(_length), vertices(vertex_type)
		{

		}
		~Voxel() = default;

	public:
		inline float GetLength() const { return length; };
		inline const VoxelVertices<VertexCount>& GetVertices() const { return vertices; }
		inline const Texture& GetTexture() const { return texture; }

	private:
		const VoxelVertices<VertexCount>& vertices;
		float length;
		Texture texture;
	};
}

#endif
