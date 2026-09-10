import csv
import sys
import re
import math
from typing import Optional

# Baseline model parameters used in the fixed spread experiment.
BASE_HALF_SPREAD = 1.0
P_INFORMED = 0.40
P_TRADE_NOISE = 0.30
PRICE_SENSITIVITY_BETA = -math.log(P_TRADE_NOISE)

def normal_density(z: float) -> float:
    return (
        1.0 / math.sqrt(2.0 * math.pi)
    ) * math.exp(-(z * z) / 2.0)

def normal_cdf(z: float) -> float:
    return 0.5 * (1.0 + math.erf(z / math.sqrt(2.0)))

def normal_right_tail(z: float) -> float:
    return 0.5 * math.erfc(z / math.sqrt(2.0))

def predicted_fixed_edge(noise: float) -> float:

    h = BASE_HALF_SPREAD

    z = h / noise

    right_tail = 1.0 - normal_cdf(z)

    noise_trade_probability = (
        (1.0 - P_INFORMED) * P_TRADE_NOISE
    )

    informed_trade_probability = (
        2.0 * P_INFORMED * right_tail
    )

    adverse_selection = (
        2.0
        * P_INFORMED
        * noise
        * normal_density(z)
    )

    total_trade_probability = (
        noise_trade_probability
        + informed_trade_probability
    )

    return (
        h
        - adverse_selection / total_trade_probability
    )

def predicted_spread_results(
    trader_model: str,
    spread: float
) -> tuple[float, float]:

    noise = 1.0
    z = spread / noise

    right_tail = 1.0 - normal_cdf(z)

    if trader_model == "original":
        uninformed_trade_rate = P_TRADE_NOISE
    else:
        uninformed_trade_rate = math.exp(
            -PRICE_SENSITIVITY_BETA * spread
        )

    noise_edge_per_round = (
        (1.0 - P_INFORMED)
        * uninformed_trade_rate
        * spread
    )

    informed_edge_per_round = (
        2.0
        * P_INFORMED
        * right_tail
        * spread
        -
        2.0
        * P_INFORMED
        * noise
        * normal_density(z)
    )

    predicted_edge_per_round = (
        noise_edge_per_round
        + informed_edge_per_round
    )

    return (
        uninformed_trade_rate,
        predicted_edge_per_round
    )

def predicted_price_sensitive_edge_per_round(
    spread: float,
    noise: float
) -> float:

    z = spread / noise
    right_tail = normal_right_tail(z)

    uninformed_trade_rate = math.exp(
        -PRICE_SENSITIVITY_BETA * spread
    )

    uninformed_edge = (
        (1.0 - P_INFORMED)
        * uninformed_trade_rate
        * spread
    )

    informed_edge = (
        2.0
        * P_INFORMED
        * right_tail
        * spread
        -
        2.0
        * P_INFORMED
        * noise
        * normal_density(z)
    )

    return uninformed_edge + informed_edge


def predicted_edge_derivative(
    spread: float,
    noise: float
) -> float:

    z = spread / noise
    right_tail = normal_right_tail(z)

    uninformed_part = (
        (1.0 - P_INFORMED)
        * math.exp(-PRICE_SENSITIVITY_BETA * spread)
        * (1.0 - PRICE_SENSITIVITY_BETA * spread)
    )

    informed_part = (
        2.0
        * P_INFORMED
        * right_tail
    )

    return uninformed_part + informed_part


def find_optimal_spread(noise: float) -> float:
    low = 0.0
    high = 50.0

    # The optimum occurs where the derivative changes
    # from positive to negative.
    for _ in range(100):
        middle = (low + high) / 2.0

        if predicted_edge_derivative(middle, noise) > 0.0:
            low = middle
        else:
            high = middle

    return (low + high) / 2.0

def parse_file_info(
    path: str
) -> tuple[str, Optional[float], Optional[float], Optional[int], str]:

    base = path.split("/")[-1].lower()

    strategy = "unknown"
    noise = None
    spread = None
    run = None
    trader_model = ""

    # Original signal-noise experiment.
    if base.startswith("fixed"):
        strategy = "fixed"
    elif base.startswith("skew"):
        strategy = "skew"
    elif base.startswith("uncert"):
        strategy = "uncert"

    noise_match = re.search(
        r"_noise_([0-9]+(?:\.[0-9]+)?)_run_([0-9]+)\.csv$",
        base
    )

    if noise_match:
        noise = float(noise_match.group(1))
        run = int(noise_match.group(2))

        return strategy, noise, spread, run, trader_model

    # Fixed-spread trader comparison experiment.
    spread_match = re.search(
        r"^(original|price_sensitive)_spread_"
        r"([0-9]+(?:\.[0-9]+)?)_run_([0-9]+)\.csv$",
        base
    )

    if spread_match:
        strategy = "fixed"
        trader_model = spread_match.group(1)
        spread = float(spread_match.group(2))
        run = int(spread_match.group(3))

    return strategy, noise, spread, run, trader_model

def analyze(path: str) -> dict:
    trades = 0
    max_abs_inv = 0
    rounds = 0
    uninformed_arrivals = 0
    uninformed_trades = 0
    last_risk_adj = None
    last_pnl = None

    edge_sum = 0.0
    edge_count = 0

    with open(path, newline="") as f:
        r = csv.DictReader(f)
        for row in r:
            rounds += 1

            trader_type = row["trader_type"]

            if trader_type in ("noise", "price_sensitive_uninformed"):
                uninformed_arrivals += 1
            
            inv = int(row["inventory"])
            max_abs_inv = max(max_abs_inv, abs(inv))

            side = row["trade_side"]
            tv = float(row["true_value"])
            price = float(row["trade_price"])

            last_pnl = float(row["pnl_mark_to_true"])
            last_risk_adj = float(row["risk_adj_pnl"])

            if side != "none":
                trades += 1

                if trader_type in ("noise", "price_sensitive_uninformed"):
                    uninformed_trades += 1

                if side == "buy_from_mm":
                    edge = price - tv
                elif side == "sell_to_mm":
                    edge = tv - price
                else:
                    edge = 0.0

                edge_sum += edge
                edge_count += 1

    avg_edge = edge_sum / edge_count if edge_count else 0.0

    uninformed_trade_rate = (
        uninformed_trades / uninformed_arrivals
        if uninformed_arrivals else 0.0
    )

    strategy, noise, spread, run, trader_model = parse_file_info(path)

    return {
        "file": path,
        "strategy": strategy,
        "noise": "" if noise is None else noise,
        "spread": "" if spread is None else spread,
        "run": "" if run is None else run,
        "trader_model": trader_model,
        "final_pnl": last_pnl,
        "final_risk_adj_pnl": last_risk_adj,
        "max_abs_inventory": max_abs_inv,
        "num_rounds": rounds,
        "num_trades": trades,
        "avg_edge_per_trade": avg_edge,
        "uninformed_arrivals": uninformed_arrivals,
        "uninformed_trades": uninformed_trades,
        "uninformed_trade_rate": uninformed_trade_rate,
    }

def write_summary_csv(out_path: str, results: list[dict]) -> None:
    fields = [
        "file",
        "strategy",
        "noise",
        "spread",
        "run",
        "trader_model",
        "final_pnl",
        "final_risk_adj_pnl",
        "max_abs_inventory",
        "num_rounds",
        "num_trades",
        "avg_edge_per_trade",
        "uninformed_arrivals",
        "uninformed_trades",
        "uninformed_trade_rate",
    ]
    with open(out_path, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        for row in results:
            w.writerow(row)

def write_repeated_summary_csv(out_path: str, results: list[dict]) -> None:
    fields = [
        "strategy",
        "noise",
        "number_of_runs",
        "avg_final_pnl",
        "avg_final_risk_adj_pnl",
        "avg_max_abs_inventory",
        "avg_num_trades",
        "avg_edge_per_trade",
        "predicted_edge",
        "difference",
    ]

    with open(out_path, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()

        for row in results:
            w.writerow(row)

def is_noise_grid_row(r: dict) -> bool:

    return r["strategy"] in ("fixed", "skew", "uncert") and r["noise"] != ""

def summarize_repeated_runs(results: list[dict]) -> list[dict]:
    """
    Groups repeated runs that use the same strategy and noise level,
    then calculates one summary row for each group.
    """

    groups = {}

    # Group runs by strategy and noise level.
    for result in results:
        if not is_noise_grid_row(result):
            continue

        key = (result["strategy"], float(result["noise"]))

        if key not in groups:
            groups[key] = []

        groups[key].append(result)

    summaries = []

    # Each group contains all repeated runs for one strategy/noise pair.
    for (strategy, noise), runs in groups.items():

        number_of_runs = len(runs)

        avg_final_pnl = (
            sum(run["final_pnl"] for run in runs) / number_of_runs
        )

        avg_final_risk_adj_pnl = (
            sum(run["final_risk_adj_pnl"] for run in runs) / number_of_runs
        )

        avg_max_abs_inventory = (
            sum(run["max_abs_inventory"] for run in runs) / number_of_runs
        )

        total_trades = sum(run["num_trades"] for run in runs)

        avg_num_trades = total_trades / number_of_runs

        # Combine execution edge across all trades from all repeated runs.
        total_edge = sum(
            run["avg_edge_per_trade"] * run["num_trades"]
            for run in runs
        )

        avg_edge_per_trade = (
            total_edge / total_trades if total_trades else 0.0
        )

        predicted_edge = ""
        difference = ""

        # We have only derived the mathematical prediction
        # for the fixed spread strategy so far.
        if strategy == "fixed":
            predicted_edge = predicted_fixed_edge(noise)
            difference = avg_edge_per_trade - predicted_edge

        summaries.append({
            "strategy": strategy,
            "noise": noise,
            "number_of_runs": number_of_runs,
            "avg_final_pnl": avg_final_pnl,
            "avg_final_risk_adj_pnl": avg_final_risk_adj_pnl,
            "avg_max_abs_inventory": avg_max_abs_inventory,
            "avg_num_trades": avg_num_trades,
            "avg_edge_per_trade": avg_edge_per_trade,
            "predicted_edge": predicted_edge,
            "difference": difference,
        })

    return summaries

def summarize_spread_runs(results: list[dict]) -> list[dict]:
    """
    Groups repeated runs by trader model and fixed half-spread.
    """
    groups = {}

    for result in results:
        if result["spread"] == "" or result["trader_model"] == "":
            continue

        key = (
            result["trader_model"],
            float(result["spread"])
        )

        if key not in groups:
            groups[key] = []

        groups[key].append(result)

    summaries = []

    for (trader_model, spread), runs in groups.items():
        number_of_runs = len(runs)

        total_trades = sum(
            run["num_trades"] for run in runs
        )

        total_rounds = sum(
            run["num_rounds"] for run in runs
        )

        total_edge = sum(
            run["avg_edge_per_trade"] * run["num_trades"]
            for run in runs
        )

        total_uninformed_arrivals = sum(
            run["uninformed_arrivals"] for run in runs
        )

        total_uninformed_trades = sum(
            run["uninformed_trades"] for run in runs
        )

        avg_num_trades = total_trades / number_of_runs

        avg_edge_per_trade = (
            total_edge / total_trades
            if total_trades else 0.0
        )

        avg_edge_per_round = (
            total_edge / total_rounds
            if total_rounds else 0.0
        )

        uninformed_trade_rate = (
            total_uninformed_trades / total_uninformed_arrivals
            if total_uninformed_arrivals else 0.0
        )

        predicted_trade_rate, predicted_edge_per_round = (
            predicted_spread_results(
                trader_model,
                spread
            )
        )

        summaries.append({
            "trader_model": trader_model,
            "spread": spread,
            "number_of_runs": number_of_runs,
            "avg_num_trades": avg_num_trades,
            "avg_edge_per_trade": avg_edge_per_trade,
            "avg_edge_per_round": avg_edge_per_round,
            "uninformed_trade_rate": uninformed_trade_rate,
            "predicted_uninformed_trade_rate": predicted_trade_rate,
            "predicted_edge_per_round": predicted_edge_per_round,
            "edge_per_round_difference":
                avg_edge_per_round - predicted_edge_per_round,
        })

    return summaries

def write_spread_summary_csv(
    out_path: str,
    results: list[dict]
) -> None:

    fields = [
        "trader_model",
        "spread",
        "number_of_runs",
        "avg_num_trades",
        "avg_edge_per_trade",
        "avg_edge_per_round",
        "uninformed_trade_rate",
        "predicted_uninformed_trade_rate",
        "predicted_edge_per_round",
        "edge_per_round_difference",
    ]

    with open(out_path, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()

        for row in results:
            w.writerow(row)

def write_optimal_spread_summary(
    out_path: str,
    noise_values: list[float]
) -> None:

    fields = [
        "noise",
        "optimal_spread",
        "predicted_edge_per_round",
    ]

    with open(out_path, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()

        for noise in noise_values:
            optimal_spread = find_optimal_spread(noise)

            predicted_edge = (
                predicted_price_sensitive_edge_per_round(
                    optimal_spread,
                    noise
                )
            )

            w.writerow({
                "noise": noise,
                "optimal_spread": optimal_spread,
                "predicted_edge_per_round": predicted_edge,
            })

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 analyze.py <csv1> <csv2> ...")
        sys.exit(1)

    paths = sys.argv[1:]
    results = [analyze(p) for p in paths]

    write_summary_csv("data/summary.csv", results)
    print("Wrote data/summary.csv")

    # Combine repeated runs into one row for each strategy and noise level.
    grid = summarize_repeated_runs(results)

    def sort_key(r: dict):
        return (float(r["noise"]), r["strategy"])

    grid.sort(key=sort_key)

    write_repeated_summary_csv("data/noise_grid_summary.csv", grid)
    print("Wrote data/noise_grid_summary.csv")

    spread_grid = summarize_spread_runs(results)

    def spread_sort_key(r: dict):
        return (float(r["spread"]), r["trader_model"])

    spread_grid.sort(key=spread_sort_key)

    write_spread_summary_csv(
        "data/spread_summary.csv",
        spread_grid
    )

    print("Wrote data/spread_summary.csv")

    noise_values = [0.5, 1.0, 2.0, 4.0]

    write_optimal_spread_summary(
        "data/optimal_spread_summary.csv",
        noise_values
    )

    print("Wrote data/optimal_spread_summary.csv")

if __name__ == "__main__":
    main()
