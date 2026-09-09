#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "config.h"
#include "rng.h"
#include "market.h"
#include "strategy.h"
#include "trader.h"
#include "csv.h"
#include <string>

// Tracks the market maker’s financial state over time.
struct State {

    // Current inventory (number of units held).
    int inventory = 0;

    // Cash balance from executed trades.
    double cash = 0.0;
};

// Identifies which quoting strategy the simulator should use.
// The choice is fixed for the duration of a Simulator run.
enum class StrategyType {
    FixedSpread,        // constant spread around the signal
    InventorySkew,      // shifts quotes based on inventory
    UncertaintyAware    // widens spread when signal is noisy
};

class Simulator {

    // Purpose:
    //   Runs a single end-to-end market-making simulation using a fixed
    //   configuration and a single quoting strategy. Produces a CSV log
    //   containing the full timestep-by-timestep evolution.

public:
    // PRE: cfg is defined and contains valid simulation parameters.
    //      strategy specifies which quoting rule to use.
    // POST: constructs a simulator ready to run with the given setup.
    Simulator(const Config& cfg, StrategyType strategy);

    // PRE: out_path specifies the CSV file to write results to.
    // POST: runs the simulation for cfg.T timesteps and writes one row
    //       per timestep to the CSV at out_path.
    void run(const std::string& out_path);

private:

    // Copy of the configuration used for this simulation run.
    Config m_cfg;

    // Which quoting strategy to apply at each timestep.
    StrategyType m_strategy;
};

#endif // SIMULATOR_H
