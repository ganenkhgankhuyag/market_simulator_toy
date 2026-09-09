#include "strategy.h"

FixedSpreadStrategy::FixedSpreadStrategy(double half_spread)
    : m_half_spread(half_spread) {}

Quote FixedSpreadStrategy::quote(double signal, int /*inventory*/) {
    Quote q;
    q.bid = signal - m_half_spread;
    q.ask = signal + m_half_spread;
    return q;
}

InventorySkewStrategy::InventorySkewStrategy(double half_spread, double skew_k)
    : m_half_spread(half_spread), m_skew_k(skew_k) {}

Quote InventorySkewStrategy::quote(double signal, int inventory) {
    Quote q;

    double shift = m_skew_k * (double)inventory;

    // Shift both bid/ask down when inventory is positive,
    // shift both up when inventory is negative.
    double mid = signal;

    q.bid = mid - m_half_spread - shift;
    q.ask = mid + m_half_spread - shift;

    return q;
}

UncertaintySpreadStrategy::UncertaintySpreadStrategy(double base_half_spread,
                                                     double spread_alpha,
                                                     double signal_noise_std)
    : m_base_half_spread(base_half_spread),
      m_spread_alpha(spread_alpha),
      m_signal_noise_std(signal_noise_std) {}

Quote UncertaintySpreadStrategy::quote(double signal, int /*inventory*/) {
    Quote q;

    // Wider spread when uncertainty is higher.
    double half_spread = m_base_half_spread + m_spread_alpha * m_signal_noise_std;

    q.bid = signal - half_spread;
    q.ask = signal + half_spread;

    return q;
}
