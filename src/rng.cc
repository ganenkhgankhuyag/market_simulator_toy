#include "rng.h"
#include <cassert>

RNG::RNG(unsigned int seed) : m_gen(seed) {}

bool RNG::bernoulli(double p) {
    if (p <= 0.0) return false;
    if (p >= 1.0) return true;
    std::bernoulli_distribution d(p);
    return d(m_gen);
}

double RNG::uniform(double a, double b) {
    std::uniform_real_distribution<double> d(a, b);
    return d(m_gen);
}

double RNG::normal(double mean, double stddev) {
    // stddev should be >= 0; if 0, this returns exactly mean
    assert(stddev >= 0.0);
    std::normal_distribution<double> d(mean, stddev);
    return d(m_gen);
}
