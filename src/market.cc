#include "market.h"

Market::Market(const Config& cfg) : m_cfg(cfg), m_true_value(0.0) {}

MarketStep Market::step(RNG& rng) {
    // Random walk: value changes a little each round
    m_true_value += rng.normal(0.0, m_cfg.value_step_std);

    // You observe a noisy signal about the true value
    double signal = m_true_value + rng.normal(0.0, m_cfg.signal_noise_std);

    MarketStep ms;
    ms.true_value = m_true_value;
    ms.signal = signal;
    return ms;
}
