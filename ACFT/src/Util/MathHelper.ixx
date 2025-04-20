module;

#include <type_traits>
#include <cstdint>
#include <bit>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Platform.h"

export module MathHelper;


export namespace ACFT
{
	export template<typename Type>
	inline constexpr Type Abs(Type x)
		requires std::is_arithmetic_v<Type>
	{
		if (!std::is_signed_v<Type>)
			return x;

		if (x >= 0)
			return x;
		else
			return -x;
	}

	export template<typename RetType, int Iterations = 10>
		requires std::is_arithmetic_v<RetType> && (Iterations > 0)
	inline constexpr RetType Sqrt(double x)
	{
		if (x == 0.0)
			return static_cast<RetType>(0.0);

		if (x < 0.0)
			return std::numeric_limits<RetType>::quiet_NaN();

		double guess = (x > 1.0) ? (x / 2.0) : 1.0;
		for (int i = 0; i < Iterations; i++)
		{
			double temp_guess = (guess + x / guess) / 2.0;
			if (Abs<double>(temp_guess - guess) < 1e-10)
				return static_cast<RetType>(guess);
			else
				guess = temp_guess;
		}

		return static_cast<RetType>(guess);
	}

	export template<typename RetType>
		requires std::is_arithmetic_v<RetType>
	inline constexpr RetType FastSqrt(float x)
	{
		if (x == 0.0f)
			return static_cast<RetType>(0.0);

		if (x < 0.0f)
			return std::numeric_limits<RetType>::quiet_NaN();

		uint32_t i = std::bit_cast<uint32_t>(x);
		i = 0x5f3759df - (i >> 1);
		float result = std::bit_cast<float>(i);
		result *= x;
		result = result - (result * result - x) / (2.0f * result);
		return static_cast<RetType>(result);
	}

	export ACFT_API inline glm::vec3 GetVec3fFromYP(float yaw, float pitch)
	{
		float x = glm::cos(pitch) * glm::cos(yaw);
		float y = glm::sin(pitch);
		float z = -glm::cos(pitch) * glm::sin(yaw);
		return glm::vec3(x, y, z);
	}

	export ACFT_API inline glm::vec3 GetVec3fFromYP(float yaw, float pitch, float length)
	{
		float x = glm::cos(pitch) * glm::cos(yaw);
		float y = glm::sin(pitch);
		float z = -glm::cos(pitch) * glm::sin(yaw);
		return glm::vec3(x, y, z) * length;
	}

	export ACFT_API inline float Vec3Dotfi(const glm::vec3& fvec, const glm::ivec3& ivec)
	{
		return fvec.x * (float)ivec.x + fvec.y * (float)ivec.y + fvec.z * (float)ivec.z;
	}

	export ACFT_API inline glm::vec3 operator+(const glm::vec3& fvec, const glm::ivec3& ivec)
	{
		return glm::vec3(fvec.x + ivec.x, fvec.y + ivec.y, fvec.z + ivec.z);
	}

	export ACFT_API inline glm::vec3 operator-(const glm::vec3& fvec, const glm::ivec3& ivec)
	{
		return glm::vec3(fvec.x - ivec.x, fvec.y - ivec.y, fvec.z - ivec.z);
	}
}