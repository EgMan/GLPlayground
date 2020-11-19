#pragma once
#include "StdAfx.h"
#include "FloatInterpolator.cpp"
#include "KeyFrame.cpp"

//This class is responsible for interpolating each individual field of a keyframe

class KeyFrameInterpolator
{
private:
	FloatInterpolator * interpolator;
public:
	KeyFrameInterpolator(float blendingFunction[4][4])
	{
		interpolator = new FloatInterpolator(blendingFunction);
	}
	~KeyFrameInterpolator()
	{
		delete interpolator;
	}
	KeyFrame * interpolate(float time, KeyFrame f1, KeyFrame f2, KeyFrame f3, KeyFrame f4)
	{
		float posX = interpolator->interpolate(time, f1.posX, f2.posX, f3.posX, f4.posX);
		float posY = interpolator->interpolate(time, f1.posY, f2.posY, f3.posY, f4.posY);
		float posZ = interpolator->interpolate(time, f1.posZ, f2.posZ, f3.posZ, f4.posZ);

		float quatW = interpolator->interpolate(time, f1.quatW, f2.quatW, f3.quatW, f4.quatW);
		float quatX = interpolator->interpolate(time, f1.quatX, f2.quatX, f3.quatX, f4.quatX);
		float quatY = interpolator->interpolate(time, f1.quatY, f2.quatY, f3.quatY, f4.quatY);
		float quatZ = interpolator->interpolate(time, f1.quatZ, f2.quatZ, f3.quatZ, f4.quatZ);

		return new KeyFrame(posX, posY, posZ, quatW, quatX, quatY, quatZ);
	}
};