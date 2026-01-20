#ifndef RNG_H
#define RNG_H

#include <random>

class RNG {

    // Purpose:
    //   Thin wrapper around a pseudorandom number generator.
    //   Centralizes all randomness so simulation behavior is
    //   reproducible and easy to control.

public:
    // PRE: seed is defined.
    // POST: initializes the generator so random draws are deterministic
    //       given the same seed and call sequence.
    explicit RNG(unsigned int seed);

    // PRE: 0 <= p <= 1
    // POST: returns true with probability p, false otherwise.
    //       Used for modeling random events (e.g., trader arrival, trade decision).
    bool bernoulli(double p);

    // PRE: a <= b
    // POST: returns a real number uniformly distributed in [a, b].
    //       Used for generating simple continuous randomness.
    double uniform(double a, double b);

    // PRE: stddev >= 0
    // POST: returns a real number drawn from a normal distribution
    //       with given mean and standard deviation.
    //       Used for value dynamics and signal noise.
    double normal(double mean, double stddev);

private:

    // Mersenne Twister generator.
    // Stores the full internal state of the RNG.
    std::mt19937 gen_;
};

#endif // RNG_H
