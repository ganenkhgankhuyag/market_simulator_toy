#ifndef CONFIG_H
#define CONFIG_H

struct Config {

    // How many timesteps (rounds) to simulate.
    // The simulator runs exactly T rounds and logs one CSV row per round.
    int T = 200;

    // Randomness control for repeatable experiments.
    // Using the same seed makes the simulation produce the same sequence
    // of random outcomes, which lets you compare strategies fairly.
    unsigned int seed = 12345;

    // Base quote distance from the observed signal.
    // The market maker sets a "bid" (buy price) below the signal and an
    // "ask" (sell price) above the signal. This number is the size of that
    // distance on each side. Larger values mean wider quotes (safer, fewer trades).
    double base_half_spread = 1.0;

    // Inventory-control strength (used only by the inventory-skew strategy).
    // If the market maker is holding too much inventory, this coefficient controls
    // how aggressively quotes are shifted to encourage trades that reduce inventory.
    double inventory_skew_k = 0.10;

    // True value dynamics: how much the hidden "true value" can move each round.
    // Larger values make the market more volatile from one timestep to the next.
    double value_step_std = 1.0;

    // Signal noise level: how wrong the market maker's observation can be.
    // Larger values mean the observed signal is less reliable.
    double signal_noise_std = 2.0;

    // Trader mix: probability the arriving trader is informed (uses true value).
    double p_informed = 0.40;

    // Noise-trader activity: probability a noise trader trades at all when chosen.
    // Noise traders ignore true value and buy/sell randomly, adding uninformed volume.
    double p_trade_noise = 0.30;

    // Price sensitivity used by the price-sensitive uninformed trader.
    // Calibrated so that a quote 1.0 away from the signal gives a 0.30 probability of trading.
    double price_sensitivity_beta = 1.203972804;

    // Risk penalty strength used for "risk-adjusted PnL" in the log.
    // Larger values punish holding large inventory more heavily (penalty grows with inventory^2).
    double inventory_penalty_lambda = 0.01;

    // Uncertainty-spread strength (used only by the uncertainty-aware strategy).
    // Controls how much the strategy widens its quotes when the signal is noisy.
    // Larger values => wider quotes under noise => fewer trades, less adverse selection.
    double spread_alpha = 0.50;
};

#endif // CONFIG_H
