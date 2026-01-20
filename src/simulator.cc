#include "simulator.h"
#include <filesystem>
#include <iomanip>
#include <sstream>

std::string to_s(double x) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6) << x;
    return oss.str();
}

std::string side_to_string(TradeSide s) {
    switch (s) {
        case TradeSide::None:       return "none";
        case TradeSide::BuyFromMM:  return "buy_from_mm";  // counterparty buys, MM sells
        case TradeSide::SellToMM:   return "sell_to_mm";   // counterparty sells, MM buys
    }
    return "none";
}

Simulator::Simulator(const Config& cfg, StrategyType strategy)
    : cfg_(cfg), strategy_(strategy) {}

void Simulator::run(const std::string& out_path) {
    // Ensure output directory exists
    std::filesystem::create_directories(
        std::filesystem::path(out_path).parent_path()
    );

    RNG rng(cfg_.seed);
    Market market(cfg_);

    NoiseTrader noise;
    InformedTrader informed;

    State st;

    CSVWriter csv(out_path);
    csv.write_header({
        "t",
        "true_value",
        "signal",
        "bid",
        "ask",
        "trade_side",
        "trade_price",
        "inventory",
        "cash",
        "pnl_mark_to_true",
        "inv_penalty",
        "risk_adj_pnl"
    });

    for (int t = 1; t <= cfg_.T; t++) {
        MarketStep ms = market.step(rng);

        Quote q;

        switch (strategy_) {
            case StrategyType::FixedSpread: {
                FixedSpreadStrategy strat(cfg_.base_half_spread);
                q = strat.quote(ms.signal, st.inventory);
                break;
            }
            case StrategyType::InventorySkew: {
                InventorySkewStrategy strat(
                    cfg_.base_half_spread,
                    cfg_.inventory_skew_k
                );
                q = strat.quote(ms.signal, st.inventory);
                break;
            }
            case StrategyType::UncertaintyAware: {
                UncertaintySpreadStrategy strat(
                    cfg_.base_half_spread,
                    cfg_.spread_alpha,
                    cfg_.signal_noise_std
                );
                q = strat.quote(ms.signal, st.inventory);
                break;
            }
        }

        // Choose trader type
        Trader* trader = nullptr;
        if (rng.bernoulli(cfg_.p_informed)) {
            trader = &informed;
        } else {
            trader = &noise;
        }

        Trade tr = trader->respond(q, ms.true_value, rng, cfg_);

        // Update cash/inventory based on trade
        if (tr.side == TradeSide::BuyFromMM) {
            st.inventory -= 1;
            st.cash += tr.price;
        } else if (tr.side == TradeSide::SellToMM) {
            st.inventory += 1;
            st.cash -= tr.price;
        }

        double pnl = st.cash + static_cast<double>(st.inventory) * ms.true_value;

        double inv_penalty =
            cfg_.inventory_penalty_lambda *
            static_cast<double>(st.inventory) *
            static_cast<double>(st.inventory);

        double risk_adj_pnl = pnl - inv_penalty;

        csv.write_row({
            std::to_string(t),
            to_s(ms.true_value),
            to_s(ms.signal),
            to_s(q.bid),
            to_s(q.ask),
            side_to_string(tr.side),
            to_s(tr.price),
            std::to_string(st.inventory),
            to_s(st.cash),
            to_s(pnl),
            to_s(inv_penalty),
            to_s(risk_adj_pnl)
        });
    }
}
