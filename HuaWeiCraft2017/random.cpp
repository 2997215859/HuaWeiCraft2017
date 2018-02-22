#include "random.h"

uint32_t Random::random_int(uint32_t min, uint32_t max) { // [min, max]
	return std::uniform_int_distribution<uint32_t>{min, max}(generator);
}
double Random::random_double(double min, double max) { // [min, max)
	return std::uniform_real_distribution<double>{min, max}(generator);
}
double Random::rnd(double dbLow, double dbUpper) {
	double dbTemp = rand() / ((double)RAND_MAX + 1.0);
	return dbLow + dbTemp*(dbUpper - dbLow);
}