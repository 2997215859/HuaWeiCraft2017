#pragma once

#ifndef  RANDOM_H
#define RANDOM_H

#include <random>
#include <ctime>
class Random {
	std::default_random_engine generator;
public:
	// Random(time_t t = time(NULL)): generator(t) {}
	inline uint32_t random_int(uint32_t min, uint32_t max) { // [min, max]
		return std::uniform_int_distribution<uint32_t>{min, max}(generator);
	}
	inline double random_double(double min, double max) { // [min, max)
		return std::uniform_real_distribution<double>{min, max}(generator);
	}
};
#endif
