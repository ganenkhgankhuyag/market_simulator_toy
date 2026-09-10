import csv
import sys
import re
import math
from typing import Optional

# Baseline model parameters used in the fixed spread experiment.
BASE_HALF_SPREAD = 1.0
P_INFORMED = 0.40
P_TRADE_NOISE = 0.30

def normal_density(z: float) -> float:
    return (
        1.0 / math.sqrt(2.0 * math.pi)
    ) * math.exp(-(z * z) / 2.0)

def normal_cdf(z: float) -> float:
    return 0.5 * (1.0 + math.erf(z / math.sqrt(2.0)))

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

def parse_file_info(path: str) -> tuple[str, Optional[float], Optional[int]]:
    """
    Examples:
      data/fixed_noise_2_run_1.csv    -> ("fixed", 2.0, 1)
      data/skew_noise_0.5_run_12.csv  -> ("skew", 0.5, 12)
      data/uncert_noise_4_run_50.csv  -> ("uncert", 4.0, 50)
    """

    base = path.split("/")[-1].lower()

    # Determine which strategy produced this file.
    if base.startswith("fixed"):
        strategy = "fixed"
    elif base.startswith("skew"):
        strategy = "skew"
    elif base.startswith("uncert"):
        strategy = "uncert"
    else:
        strategy = "unknown"

    # Extract the noise level and repeated run number from the filename.
    match = re.search(
        r"_noise_([0-9]+(?:\.[0-9]+)?)_run_([0-9]+)\.csv$",
        base
    )

    if match:
        noise = float(match.group(1))
        run = int(match.group(2))
    else:
        noise = None
        run = None

    return strategy, noise, run

def analyze(path: str) -> dict:
    trades = 0
    max_abs_inv = 0
    last_risk_adj = None
    last_pnl = None

    edge_sum = 0.0
    edge_count = 0

    with open(path, newline="") as f:
        r = csv.DictReader(f)
        for row in r:
            inv = int(row["inventory"])
            max_abs_inv = max(max_abs_inv, abs(inv))

            side = row["trade_side"]
            tv = float(row["true_value"])
            price = float(row["trade_price"])

            last_pnl = float(row["pnl_mark_to_true"])
            last_risk_adj = float(row["risk_adj_pnl"])

            if side != "none":
                trades += 1

                if side == "buy_from_mm":     
                    edge = price - tv
                elif side == "sell_to_mm":     
                    edge = tv - price
                else:
                    edge = 0.0

                edge_sum += edge
                edge_count += 1

    avg_edge = edge_sum / edge_count if edge_count else 0.0

    strategy, noise, run = parse_file_info(path)

    return {
        "file": path,
        "strategy": strategy,
        "noise": "" if noise is None else noise,
        "run": "" if run is None else run,
        "final_pnl": last_pnl,
        "final_risk_adj_pnl": last_risk_adj,
        "max_abs_inventory": max_abs_inv,
        "num_trades": trades,
        "avg_edge_per_trade": avg_edge,
    }

def write_summary_csv(out_path: str, results: list[dict]) -> None:
    fields = [
        "file",
        "strategy",
        "noise",
        "run",
        "final_pnl",
        "final_risk_adj_pnl",
        "max_abs_inventory",
        "num_trades",
        "avg_edge_per_trade",
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

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 analyze.py <csv1> <csv2> ...")
        sys.exit(1)

    paths = sys.argv[1:]
    results = [analyze(p) for p in paths]

    for r in results:
        print(r)

    write_summary_csv("data/summary.csv", results)
    print("Wrote data/summary.csv")

    # Combine repeated runs into one row for each strategy and noise level.
    grid = summarize_repeated_runs(results)

    def sort_key(r: dict):
        return (float(r["noise"]), r["strategy"])

    grid.sort(key=sort_key)

    write_repeated_summary_csv("data/noise_grid_summary.csv", grid)
    print("Wrote data/noise_grid_summary.csv")

if __name__ == "__main__":
    main()
