#ifndef VISUALIZER_RANDOM_H
#define VISUALIZER_RANDOM_H
#include <random>

static int RandomNumberGenerator(int start, int stop)
{
	std::random_device rd;
	std::mt19937 zGenerator(rd());
	std::uniform_int_distribution<int> uni(start, stop);

	int random_integer = uni(zGenerator);

	return random_integer;
}

#endif
