#ifndef ACFT_MATHHELPER_H_
#define ACFT_MATHHELPER_H_

namespace ACFT
{
	template<typename Type>
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
	
	template<typename RetType, int Iterations = 10>
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
}

#endif
