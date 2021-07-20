#pragma once

#include <random>

namespace RandGen
{
	int Int(int from = INT_MIN, int to = INT_MAX)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> rrand(from, to);
		return rrand(gen);
	}

	float Float(float from = -1.0f, float to = 1.0f)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> rrand(from, to);
		return rrand(gen);
	}
}