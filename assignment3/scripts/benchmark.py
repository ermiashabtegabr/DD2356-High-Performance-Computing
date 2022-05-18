import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


def benchmark(filename, title):
    # reading from file
    df = pd.read_csv(filename, sep="\t", header=None)
    message_size = df[0]
    different_nodes = df[1]
    same_node = df[2]

    # plotting
    plt.scatter(message_size, same_node, label="Intra-node")
    plt.scatter(message_size, different_nodes, label="Inter-node")
    plt.title(f"Ping pong benchmark for {title} communication")
    plt.ylabel("Time")
    plt.xlabel("Message Size (B)")
    plt.legend(loc="upper left")
    plt.savefig(f"../../reports/report3/{title}.png")
    plt.clf()

    # calculating bandwidth and latency
    pfit_same_node = np.polyfit(message_size, same_node, 1)
    pfit_diff_nodes = np.polyfit(message_size, different_nodes, 1)
    print(f"Polyfit for ping pong on intra-node: {pfit_same_node}")
    print(f"Polyfit for ping pong on inter-node: {pfit_diff_nodes}")

    latency_same_node = pfit_same_node[1] / 10 ** (-6)
    bandwidth_same_node = 1 / pfit_same_node[0] / 10 ** 9
    print(
            f"Intra-node latency: {latency_same_node:.2f} ms, bandwidth: {bandwidth_same_node:.2f} GB/s"
    )

    latency_diff_nodes = pfit_diff_nodes[1] / 10 ** (-6)
    bandwidth_diff_nodes = 1 / pfit_diff_nodes[0] / 10 ** 9
    print(
            f"Inter-node latency: {latency_diff_nodes:.2f} ms, bandwidth: {bandwidth_diff_nodes:.2f} GB/s"
    )

print("--- Benchmark for point-to-point communication ---")
benchmark("data/output_point_to_point.txt", "point-to-point")

print("--- Benchmark for one-sided communication ---")
benchmark("data/output_one_sided.txt", "one-sided")
