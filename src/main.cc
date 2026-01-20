#include "simulator.h"
#include "config.h"

#include <iostream>
#include <vector>
#include <string>

// PRE: x is defined.
// POST: returns a cleaned decimal string suitable for embedding in filenames.
std::string clean(double x) {
    std::string s = std::to_string(x);

    // LI:
    //   s is always a valid string representation; each loop iteration removes
    //   one trailing '0', so the loop makes progress and terminates.
    while (!s.empty() && s.back() == '0') s.pop_back();

    if (!s.empty() && s.back() == '.') s.pop_back();

    return s;
}

int main() {
    
    Config cfg;

    // Simulation horizon and randomness control.
    // Setting seed makes each experiment reproducible for fair comparisons.
    cfg.T = 2000;
    cfg.seed = 123;

    // Strategy parameters (kept constant across the noise sweep).
    cfg.spread_alpha = 0.50;        // how aggressively the uncertainty strategy widens spread
    cfg.inventory_skew_k = 0.01;    // how aggressively inventory shifts quotes

    // Grid of signal noise values to test.
    std::vector<double> noises = {0.5, 1.0, 2.0, 4.0};

    // LI:
    //   After processing the first i noise values, CSV outputs have been written
    //   for each of those noise levels for all strategies listed below.
    for (double ns : noises) {
        // For this experiment, only the signal noise changes; all other cfg fields
        // remain fixed to isolate the effect of observation uncertainty.
        cfg.signal_noise_std = ns;

        // Fixed spread baseline.
        // PRE: cfg is defined and contains the parameters for this run.
        // POST: writes one CSV file containing timestep-by-timestep log data.
        Simulator fixed_spread_sim(cfg, StrategyType::FixedSpread);
        std::string fixed_spread_path =
            "data/fixed_noise_" + clean(ns) + ".csv";
        std::cout << "Running FixedSpread, noise=" << ns
                  << " -> " << fixed_spread_path << "\n";
        fixed_spread_sim.run(fixed_spread_path);

        // Inventory skew strategy.
        Simulator inventory_skew_sim(cfg, StrategyType::InventorySkew);
        std::string inventory_skew_path =
            "data/skew_noise_" + clean(ns) + ".csv";
        std::cout << "Running InventorySkew (k=" << cfg.inventory_skew_k
                  << "), noise=" << ns << " -> " << inventory_skew_path << "\n";
        inventory_skew_sim.run(inventory_skew_path);

        // Uncertainty-aware spread strategy.
        Simulator uncertainty_spread_sim(cfg, StrategyType::UncertaintyAware);
        std::string uncertainty_spread_path =
            "data/uncert_noise_" + clean(ns) + ".csv";
        std::cout << "Running UncertaintySpread (alpha=" << cfg.spread_alpha
                  << "), noise=" << ns << " -> " << uncertainty_spread_path << "\n";
        uncertainty_spread_sim.run(uncertainty_spread_path);
    }

    std::cout << "Done. CSVs written to data/.\n";
    return 0;
}
