#ifndef ACFT_VEC_H_
#define ACFT_VEC_H_

namespace ACFT
{	
	inline glm::vec3 GetVec3fFromYP(float yaw, float pitch)
	{
		float x = glm::cos(pitch) * glm::cos(yaw);
		float y = glm::sin(pitch);
		float z = -glm::cos(pitch) * glm::sin(yaw);
		return glm::vec3(x, y, z);
	}

	inline glm::vec3 GetVec3fFromYP(float yaw, float pitch, float length)
	{
		float x = glm::cos(pitch) * glm::cos(yaw);
		float y = glm::sin(pitch);
		float z = -glm::cos(pitch) * glm::sin(yaw);
		return glm::vec3(x, y, z) * length;
	}

	inline float Vec3Dotfi(const glm::vec3& fvec, const glm::ivec3& ivec)
	{
		return fvec.x * (float)ivec.x + fvec.y * (float)ivec.y + fvec.z * (float)ivec.z;
	}

	inline glm::vec3 operator+(const glm::vec3& fvec, const glm::ivec3& ivec)
	{
		return glm::vec3(fvec.x + ivec.x, fvec.y + ivec.y, fvec.z + ivec.z);
	}
	
	inline glm::vec3 operator-(const glm::vec3& fvec, const glm::ivec3& ivec)
	{
		return glm::vec3(fvec.x - ivec.x, fvec.y - ivec.y, fvec.z - ivec.z);
	}
}

#endif
