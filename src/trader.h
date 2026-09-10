#ifndef TRADER_H
#define TRADER_H

#include "strategy.h"
#include "rng.h"
#include "config.h"

// Indicates how a trade (if any) occurs, from the market maker’s perspective.
enum class TradeSide {

    // No trade occurs this timestep.
    None,

    // The counterparty buys from the market maker.
    // The market maker sells at the ask price.
    BuyFromMM,

    // The counterparty sells to the market maker.
    // The market maker buys at the bid price.
    SellToMM
};

// Represents the result of a trader’s interaction with a quote.
struct Trade {

    // Which side of the market was traded (or None if no trade).
    TradeSide side = TradeSide::None;

    // Transaction price (bid or ask).
    double price = 0.0;
};

// Abstract interface for trader behavior.
// A trader observes the market maker’s quote and decides whether
// to trade, and on which side.
class Trader {
public:
    // Virtual destructor so derived trader types can be destroyed
    // safely through a base-class pointer if needed.
    virtual ~Trader() = default;

    // PRE: q is the market maker’s current bid/ask quote.
    //      true_value is the asset’s actual value at this timestep.
    //      rng provides randomness if the trader’s behavior is stochastic.
    //      cfg contains global simulation parameters.
    // POST: returns a Trade describing whether a trade occurred,
    //       and at what price if it did.
    virtual Trade respond(const Quote& q,
                          double signal,
                          double true_value,
                          RNG& rng,
                          const Config& cfg) = 0;
};

// Noise trader.
// Trades randomly without regard to the asset’s true value.
class NoiseTrader : public Trader {
public:
    // PRE: q, rng, and cfg are defined.
    // POST: with some probability, returns a buy or sell trade at
    //       the quoted price; otherwise returns no trade.
    Trade respond(const Quote& q,
                  double signal,
                  double true_value,
                  RNG& rng,
                  const Config& cfg) override;
};

// Price-sensitive uninformed trader.
// Does not know the true value. Randomly chooses whether it wants
// to buy or sell, then decides whether to trade based on the quoted
// price relative to the noisy signal.
class PriceSensitiveUninformedTrader : public Trader {
public:
    // PRE: q, signal, rng, and cfg are defined.
    // POST: randomly chooses a buy or sell intention, then trades
    //       with a probability based on how far that quote is from signal.
    Trade respond(const Quote& q,
                  double signal,
                  double true_value,
                  RNG& rng,
                  const Config& cfg) override;
};

// Informed trader.
// Trades only when the market maker’s quote is favorable compared
// to the true value, creating adverse selection.
class InformedTrader : public Trader {
public:
    // PRE: q and true_value are defined.
    // POST: trades if and only if the quote is mispriced relative
    //       to true_value; otherwise returns no trade.
    Trade respond(const Quote& q,
                  double signal,
                  double true_value,
                  RNG& rng,
                  const Config& cfg) override;
};

#endif // TRADER_H
