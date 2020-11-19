#pragma once
#include "StdAfx.h"
#include <math.h>

// This class stores the information for each key frame.  Each key frame is stored as position+quaternion, though you can initialize it and/or read it as position+eulerAngle

class KeyFrame
{
public:
	float posX, posY, posZ, quatW, quatX, quatY, quatZ;
	KeyFrame(float posX, float posY, float posZ, float quatW, float quatX, float quatY, float quatZ)
		:
		posX(posX),
		posY(posY),
		posZ(posZ),
		quatW(quatW),
		quatX(quatX),
		quatY(quatY),
		quatZ(quatZ) {};

	KeyFrame(float posX, float posY, float posZ, float anglX, float anglY, float anglZ) :
		posX(posX),
		posY(posY),
		posZ(posZ)
	{
		// Source: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
		double cy = cos(anglX * 0.5);
		double sy = sin(anglX * 0.5);
		double cp = cos(anglY * 0.5);
		double sp = sin(anglY * 0.5);
		double cr = cos(anglZ * 0.5);
		double sr = sin(anglZ * 0.5);

		quatW = cr * cp * cy + sr * sp * sy;
		quatX = sr * cp * cy - cr * sp * sy;
		quatY = cr * sp * cy + sr * cp * sy;
		quatZ = cr * cp * sy - sr * sp * cy;
	}

	float * getMatrix()
	{
		float out[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, posX, posY, posZ, 1 };

		out[0] = 1.0f - 2.0f * quatY * quatY - 2.0f * quatZ * quatZ;
		out[1] = 2.0f * quatX * quatY + 2.0f * quatW * quatZ;
		out[2] = 2.0f * quatX * quatZ - 2.0f * quatW * quatY;
		out[3] = 0.0f;
		out[4] = 2.0f * quatX * quatY - 2.0f * quatW * quatZ;
		out[5] = 1.0f - 2.0f * quatX * quatX - 2.0f * quatZ * quatZ;
		out[6] = 2.0f * quatY * quatZ + 2.0f * quatW * quatX;
		out[7] = 0.0f;
		out[8] = 2.0f * quatX * quatZ + 2.0f * quatW * quatY;
		out[9] = 2.0f * quatY * quatZ - 2.0f * quatW * quatX;
		out[10] = 1.0f - 2.0f * quatX * quatX - 2.0f * quatY * quatY;
		out[11] = 0.0f;
		out[12] = posX;
		out[13] = posY;
		out[14] = posZ;
		out[15] = 1.0f;
		return out;
	}

	// Source: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	float getEulerX()
	{
		double sinr_cosp = 2 * (quatW * quatX + quatY * quatZ);
		double cosr_cosp = 1 - 2 * (quatX * quatX + quatY * quatY);
		return std::atan2(sinr_cosp, cosr_cosp);
	}

	// Source: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	float getEulerY()
	{
		double sinp = 2 * (quatW * quatY - quatZ * quatX);
		if (std::abs(sinp) >= 1)
			return std::copysign(3.14159265 / 2, sinp);
		else
		return std::asin(sinp);
	}

	// Source: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	float getEulerZ()
	{
		double siny_cosp = 2 * (quatW * quatZ + quatX * quatY);
		double cosy_cosp = 1 - 2 * (quatY * quatY + quatZ * quatZ);
		return std::atan2(siny_cosp, cosy_cosp);
	}
};
