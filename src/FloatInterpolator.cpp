#pragma once
#include "StdAfx.h"
#include <math.h>

//This class deals with interpolation on the most granular level: interpolating between two floating points

class FloatInterpolator
{
private:
	float m[4][4] = {};
public:
	FloatInterpolator(float blendingFunction[4][4])
	{
		for (int x = 0; x < 4; x++)
			for (int y = 0; y < 4; y++)
			{
				this->m[x][y] = blendingFunction[x][y];
			}
	}
	float interpolate(float time, float f1, float f2, float f3, float f4)
	{
		time = fmod(time, 1);
		float time_pow2 = pow(time, 2);
		float time_pow3 = pow(time, 3);

		/*return (time_pow3 * m[0][0] + time_pow2 * m[1][0] + time * m[2][0] + m[3][0]) * f1 +
			(time_pow3 * m[0][1] + time_pow2 * m[1][1] + time * m[2][1] + m[3][1]) * f2 +
			(time_pow3 * m[0][2] + time_pow2 * m[1][2] + time * m[2][2] + m[3][2]) * f3 +
			(time_pow3 * m[0][3] + time_pow2 * m[1][3] + time * m[2][3] + m[3][3]) * f4;*/
		float a = (time_pow3 * m[1][0] + time_pow2 * m[1][1] + time * m[1][2] + m[1][3]);

		return (time_pow3 * m[0][0] + time_pow2 * m[0][1] + time * m[0][2] + m[0][3]) * f1 +
			(time_pow3 * m[1][0] + time_pow2 * m[1][1] + time * m[1][2] + m[1][3]) * f2 +
			(time_pow3 * m[2][0] + time_pow2 * m[2][1] + time * m[2][2] + m[2][3]) * f3 +
			(time_pow3 * m[3][0] + time_pow2 * m[3][1] + time * m[3][2] + m[3][3]) * f4;
	}
};