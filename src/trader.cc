#include "trader.h"
#include <cmath>

// Noise trader:
// - With probability p_trade_noise, they trade
// - If they trade, they randomly buy or sell
Trade NoiseTrader::respond(const Quote& q, double /*signal*/, double /*true_value*/, RNG& rng, const Config& cfg) {
    Trade t;

    if (!rng.bernoulli(cfg.p_trade_noise)) {
        t.side = TradeSide::None;
        return t;
    }

    bool buy = rng.bernoulli(0.5); // 50/50 buy/sell
    if (buy) {
        t.side = TradeSide::BuyFromMM; // they buy, so MM sells at ask
        t.price = q.ask;
    } else {
        t.side = TradeSide::SellToMM;  // they sell, so MM buys at bid
        t.price = q.bid;
    }
    return t;
}

// Price-sensitive uninformed trader:
// - Randomly chooses whether they want to buy or sell
// - Uses the noisy signal as their reference price
// - More expensive quotes are less likely to be accepted
Trade PriceSensitiveUninformedTrader::respond(
    const Quote& q,
    double signal,
    double /*true_value*/,
    RNG& rng,
    const Config& cfg
) {
    Trade t;

    // Randomly choose what the trader wants to do first.
    bool buy = rng.bernoulli(0.5);

    double distance;

    if (buy) {
        distance = q.ask - signal;
    } else {
        distance = signal - q.bid;
    }

    // A less attractive price makes the trader less likely to execute.
    double trade_probability =
        std::exp(-cfg.price_sensitivity_beta * distance);

    // If the quote is at least as favorable as the trader's signal,
    // the trader always accepts it.
    if (trade_probability > 1.0) {
        trade_probability = 1.0;
    }

    if (!rng.bernoulli(trade_probability)) {
        return t;
    }

    if (buy) {
        t.side = TradeSide::BuyFromMM;
        t.price = q.ask;
    } else {
        t.side = TradeSide::SellToMM;
        t.price = q.bid;
    }

    return t;
}

// Informed trader:
// They trade only if it looks favorable compared to true value.
// - If ask is below true value, they buy (MM sells too cheap)
// - If bid is above true value, they sell (MM buys too expensive)
// Otherwise no trade.
Trade InformedTrader::respond(const Quote& q, double /*signal*/, double true_value, RNG& /*rng*/, const Config& /*cfg*/) {
    Trade t;

    if (q.ask < true_value) {
        t.side = TradeSide::BuyFromMM;
        t.price = q.ask;
        return t;
    }
    if (q.bid > true_value) {
        t.side = TradeSide::SellToMM;
        t.price = q.bid;
        return t;
    }

    t.side = TradeSide::None;
    return t;
}
