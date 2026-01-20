#include "strategy.h"

FixedSpreadStrategy::FixedSpreadStrategy(double half_spread)
    : half_spread_(half_spread) {}

Quote FixedSpreadStrategy::quote(double signal, int /*inventory*/) {
    Quote q;
    q.bid = signal - half_spread_;
    q.ask = signal + half_spread_;
    return q;
}

InventorySkewStrategy::InventorySkewStrategy(double half_spread, double skew_k)
    : half_spread_(half_spread), skew_k_(skew_k) {}

Quote InventorySkewStrategy::quote(double signal, int inventory) {
    Quote q;

    double shift = skew_k_ * (double)inventory;

    // Shift both bid/ask down when inventory is positive,
    // shift both up when inventory is negative.
    double mid = signal;

    q.bid = mid - half_spread_ - shift;
    q.ask = mid + half_spread_ - shift;

    return q;
}

UncertaintySpreadStrategy::UncertaintySpreadStrategy(double base_half_spread,
                                                     double spread_alpha,
                                                     double signal_noise_std)
    : base_half_spread_(base_half_spread),
      spread_alpha_(spread_alpha),
      signal_noise_std_(signal_noise_std) {}

Quote UncertaintySpreadStrategy::quote(double signal, int /*inventory*/) {
    Quote q;

    // Wider spread when uncertainty is higher.
    double half_spread = base_half_spread_ + spread_alpha_ * signal_noise_std_;

    q.bid = signal - half_spread;
    q.ask = signal + half_spread;

    return q;
}
