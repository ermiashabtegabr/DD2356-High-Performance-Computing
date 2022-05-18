import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


def benchmark_ping_pong(filename, title, folder):
    # reading from file
    df = pd.read_csv(filename, sep="\t", header=None)
    message_size = df[0]
    different_nodes = df[1]
    same_node = df[2]

    # plotting
    plt.scatter(message_size, same_node, label="Intra-node")
    plt.scatter(message_size, different_nodes, label="Inter-node")
    plt.title(f"Ping pong benchmark for pi {title} ")
    plt.ylabel("Time (sec)")
    plt.xlabel("Message Size (B)")
    plt.legend(loc="upper left")
    plt.savefig(folder)
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

def benchmark_pi(filename, title, folder):

    # reading from file
    df = pd.read_csv(filename, sep=' ', header=None)
    proc_num = df[0]
    exec_time = df[1]

    # plotting
    plt.plot(proc_num, exec_time)
    plt.title(f"PI benchmark - {title}")
    plt.ylabel("Time (sec)")
    plt.xlabel("Proc num")
    plt.savefig(folder)
    plt.clf()


# print("--- Benchmark for point-to-point communication ---")
# benchmark_ping_pong("data/output_point_to_point.txt", "point-to-point", "../../../reports/report3/figures")

# print("--- Benchmark for one-sided communication ---")
# benchmark_ping_pong("data/output_one_sided.txt", "one-sided", "../../../reports/report3/figures")

# benchmark_pi("../data/output_pi_linear.txt", "blocking linear reduction", "../../../reports/report3/figures/linear_pi.png")
# benchmark_pi("../data/output_pi_binary.txt", "blocking binary tree reduction", "../../../reports/report3/figures/binary_pi.png")
# benchmark_pi("../data/output_pi_linear_non_blocking.txt", "non-blocking linear reduction", "../../../reports/report3/figures/non_blocking_linear.png")
benchmark_pi("../data/output_pi_reduce.txt", "mpi reduce", "../../../reports/report3/figures/mpi_reduce.png")

