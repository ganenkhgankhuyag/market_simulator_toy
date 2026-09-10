import csv
import sys
import re
from typing import Optional

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

def is_noise_grid_row(r: dict) -> bool:

    return r["strategy"] in ("fixed", "skew", "uncert") and r["noise"] != ""

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

    grid = [r for r in results if is_noise_grid_row(r)]

    def sort_key(r: dict):
        return (float(r["noise"]), r["strategy"])

    grid.sort(key=sort_key)

    write_summary_csv("data/noise_grid_summary.csv", grid)
    print("Wrote data/noise_grid_summary.csv")

if __name__ == "__main__":
    main()
