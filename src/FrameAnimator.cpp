#pragma once
#include "StdAfx.h"
#include "KeyFrame.cpp"
#include "KeyFrameInterpolator.cpp"

// This class keeps track of all the frames, and what interpolation function to use based on the current time

class FrameAnimator
{
private:
	std::vector<KeyFrame> keyFrames;
	KeyFrameInterpolator * interpolator;
	bool loop;
public:
	FrameAnimator(float blendingFunction [4][4], bool loop)
	{
		interpolator = new KeyFrameInterpolator(blendingFunction);
		keyFrames = * new std::vector<KeyFrame>();
		this->loop = loop;
	}
	~FrameAnimator()
	{
		delete interpolator;
	}
	void addKeyFrame(KeyFrame * frame)
	{
		keyFrames.push_back(*frame);
	}
	KeyFrame * getInbetweenFrame(float time)
	{
		int idx = 0;

		if (loop)
		{
			idx = std::floor(time);
		}
		else
		{
			if (keyFrames.size() < 4)
			{
				throw std::exception("Key-framing with cubic blending requires at least four frames.  The first and last keyframe are not directly animated, and serve only as guide points");
			}
			time = fmod(time, keyFrames.size() - 3);
			idx = std::floor(time);
		}
		return interpolator->interpolate(fmod(time, 1.0), keyFrames[idx % keyFrames.size()], keyFrames[(idx + 1) % keyFrames.size()], keyFrames[(idx + 2) % keyFrames.size()], keyFrames[(idx + 3) % keyFrames.size()]);
	}
};