#include "trader.h"

// Noise trader:
// - With probability p_trade_noise, they trade
// - If they trade, they randomly buy or sell
Trade NoiseTrader::respond(const Quote& q, double /*true_value*/, RNG& rng, const Config& cfg) {
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

// Informed trader:
// They trade only if it looks favorable compared to true value.
// - If ask is below true value, they buy (MM sells too cheap)
// - If bid is above true value, they sell (MM buys too expensive)
// Otherwise no trade.
Trade InformedTrader::respond(const Quote& q, double true_value, RNG& /*rng*/, const Config& /*cfg*/) {
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
