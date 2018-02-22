#ifndef  RANDOM_H
#define RANDOM_H
#include <random>
class Random{
private:
	static std::default_random_engine generator;

public:
	// Random(time_t t = time(NULL)): generator(t) {}
	static inline uint32_t random_int(uint32_t min, uint32_t max);
	static inline double random_double(double min, double max);
	static double Random::rnd(double dbLow, double dbUpper)
};

#endif // ! RANDOM_H
