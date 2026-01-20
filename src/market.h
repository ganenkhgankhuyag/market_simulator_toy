#ifndef MARKET_H
#define MARKET_H

#include "config.h"
#include "rng.h"

// Represents the state of the market at a single timestep.
// This is what the market maker observes (signal) and what is
// used internally for evaluation (true_value).
struct MarketStep {

    // The hidden true value of the asset at this timestep.
    // Used by informed traders and for PnL calculation.
    double true_value = 0.0;

    // The noisy observation of the true value seen by the market maker.
    // Quotes are based on this value, not on true_value.
    double signal = 0.0;
};

class Market {

    // Purpose:
    //   Models the evolution of a single asset’s true value over time
    //   and generates the noisy signal observed by the market maker.

public:
    // PRE: cfg is defined and contains the market dynamics parameters.
    // POST: initializes the market with true_value starting at 0.0.
    Market(const Config& cfg);

    // PRE: rng is a valid random number generator.
    // POST: advances the market by one timestep:
    //       - updates the true value via a random walk
    //       - generates and returns a noisy signal
    MarketStep step(RNG& rng);

    // POST: returns the current true value of the asset.
    double current_true_value() const { return true_value_; }

private:

    // Simulation parameters governing value dynamics and signal noise.
    Config cfg_;

    // Current true value of the asset.
    // This value evolves over time and is not directly observed by the market maker.
    double true_value_ = 0.0;
};

#endif // MARKET_H
