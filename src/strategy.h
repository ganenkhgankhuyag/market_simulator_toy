#ifndef STRATEGY_H
#define STRATEGY_H

// Represents a price quote posted by the market maker.
// A counterparty can trade against either side.
struct Quote {

    // Price at which the market maker is willing to buy one unit.
    // If a trader sells to the market maker, this is the price used.
    double bid = 0.0;

    // Price at which the market maker is willing to sell one unit.
    // If a trader buys from the market maker, this is the price used.
    double ask = 0.0;
};

// Abstract interface for quoting behavior.
// A strategy defines how bid and ask prices are chosen based on
// the market maker’s information and current inventory.
class Strategy {
public:
    // Virtual destructor so derived strategies can be destroyed safely
    // through a base-class pointer if needed.
    virtual ~Strategy() = default;

    // PRE: signal is the observed (noisy) estimate of the asset value.
    //      inventory is the market maker’s current position.
    // POST: returns a bid and ask price to post for this timestep.
    virtual Quote quote(double signal, int inventory) = 0;
};

// Fixed-spread strategy.
// Always quotes symmetrically around the observed signal using
// a constant spread, regardless of inventory or uncertainty.
class FixedSpreadStrategy : public Strategy {
public:
    // PRE: half_spread is the desired distance from the signal
    //      to each quote side.
    // POST: constructs a fixed-spread quoting strategy.
    FixedSpreadStrategy(double half_spread);

    // PRE: signal and inventory are defined.
    // POST: returns bid = signal - half_spread,
    //       ask = signal + half_spread.
    Quote quote(double signal, int inventory) override;

private:
    // Fixed distance from the signal to bid/ask.
    double half_spread_;
};

// Inventory-skew strategy.
// Adjusts quotes based on current inventory to reduce position risk.
// Large positive inventory shifts quotes downward to encourage selling.
// Large negative inventory shifts quotes upward to encourage buying.
class InventorySkewStrategy : public Strategy {
public:
    // PRE: half_spread defines the baseline quote width.
    //      skew_k controls how strongly inventory affects quotes.
    // POST: constructs an inventory-aware quoting strategy.
    InventorySkewStrategy(double half_spread, double skew_k);

    // PRE: signal and inventory are defined.
    // POST: returns quotes shifted in a direction that encourages
    //       trades reducing the current inventory.
    Quote quote(double signal, int inventory) override;

private:
    // Baseline half-spread around the signal.
    double half_spread_;

    // Coefficient controlling how much inventory shifts quotes.
    double skew_k_;
};

// Uncertainty-aware spread strategy.
// Widens the spread when the observed signal is less reliable.
class UncertaintySpreadStrategy : public Strategy {
public:
    // PRE: base_half_spread defines the minimum quote width.
    //      spread_alpha controls how much uncertainty widens the spread.
    //      signal_noise_std measures how noisy the signal is.
    // POST: constructs an uncertainty-aware quoting strategy.
    UncertaintySpreadStrategy(double base_half_spread,
                              double spread_alpha,
                              double signal_noise_std);

    // PRE: signal and inventory are defined.
    // POST: returns bid/ask with a spread that increases as
    //       signal uncertainty increases.
    Quote quote(double signal, int inventory) override;

private:
    // Baseline half-spread used when uncertainty is low.
    double base_half_spread_;

    // Scaling factor for how uncertainty affects spread width.
    double spread_alpha_;

    // Measure of how noisy the observed signal is.
    double signal_noise_std_;
};

#endif // STRATEGY_H
