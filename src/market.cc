#include "market.h"

Market::Market(const Config& cfg) : cfg_(cfg), true_value_(0.0) {}

MarketStep Market::step(RNG& rng) {
    // Random walk: value changes a little each round
    true_value_ += rng.normal(0.0, cfg_.value_step_std);

    // You observe a noisy signal about the true value
    double signal = true_value_ + rng.normal(0.0, cfg_.signal_noise_std);

    MarketStep ms;
    ms.true_value = true_value_;
    ms.signal = signal;
    return ms;
}
