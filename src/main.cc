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

    // Simulation horizon.
    // Each individual run contains exactly 2000 timesteps.
    cfg.T = 2000;

    // Strategy parameters (kept constant across all experiments).
    cfg.spread_alpha = 0.50;        // how aggressively the uncertainty strategy widens spread
    cfg.inventory_skew_k = 0.01;    // how aggressively inventory shifts quotes

    // Grid of signal noise values to test.
    std::vector<double> noises = {0.5, 1.0, 2.0, 4.0};

    // Number of times each experiment is repeated.
    // Different seeds give different random outcomes while keeping the experiment reproducible.
    const int number_of_runs = 50;
    const unsigned int starting_seed = 123;

    // LI:
    //   After processing the first i noise values, all repeated runs have been
    //   completed for each of those noise levels for all strategies listed below.
    for (double ns : noises) {

        // For this experiment, only the signal noise changes; all other cfg fields
        // remain fixed to isolate the effect of observation uncertainty.
        cfg.signal_noise_std = ns;

        // LI:
        //   After completing the first i runs, each strategy has been tested i times
        //   at the current noise level using the same seed for each matching run.
        for (int run = 1; run <= number_of_runs; run++) {

            // Each repeated run uses a different seed.
            // The same seed is used for all three strategies within this run
            // so they are compared under the same sequence of random outcomes.
            cfg.seed = starting_seed + run - 1;

            std::string run_number = std::to_string(run);

            // Fixed spread baseline.
            // PRE: cfg is defined and contains the parameters for this run.
            // POST: writes one CSV file containing timestep-by-timestep log data.
            Simulator fixed_spread_sim(cfg, StrategyType::FixedSpread, UninformedTraderType::Noise);

            std::string fixed_spread_path =
                "data/fixed_noise_" + clean(ns) +
                "_run_" + run_number + ".csv";

            std::cout << "Running FixedSpread, noise=" << ns
                      << ", run=" << run
                      << " -> " << fixed_spread_path << "\n";

            fixed_spread_sim.run(fixed_spread_path);


            // Inventory skew strategy.
            // PRE: cfg is defined and contains the same seed and noise level
            //      used by the fixed spread strategy for this run.
            // POST: writes one CSV file containing timestep-by-timestep log data.
            Simulator inventory_skew_sim(cfg, StrategyType::InventorySkew, UninformedTraderType::Noise);

            std::string inventory_skew_path =
                "data/skew_noise_" + clean(ns) +
                "_run_" + run_number + ".csv";

            std::cout << "Running InventorySkew (k=" << cfg.inventory_skew_k
                      << "), noise=" << ns
                      << ", run=" << run
                      << " -> " << inventory_skew_path << "\n";

            inventory_skew_sim.run(inventory_skew_path);


            // Uncertainty-aware spread strategy.
            // PRE: cfg is defined and contains the same seed and noise level
            //      used by the other strategies for this run.
            // POST: writes one CSV file containing timestep-by-timestep log data.
            Simulator uncertainty_spread_sim(
                cfg,
                StrategyType::UncertaintyAware,
                UninformedTraderType::Noise
            );

            std::string uncertainty_spread_path =
                "data/uncert_noise_" + clean(ns) +
                "_run_" + run_number + ".csv";

            std::cout << "Running UncertaintySpread (alpha=" << cfg.spread_alpha
                      << "), noise=" << ns
                      << ", run=" << run
                      << " -> " << uncertainty_spread_path << "\n";

            uncertainty_spread_sim.run(uncertainty_spread_path);
        }
    }

    std::cout << "Done. CSVs written to data/.\n";
    return 0;
}