import pandas as pd
import matplotlib.pyplot as plt

def plot_errorbar(filename, title):
    df = pd.read_csv(filename, sep=' ', header=None)
    n = df[0]
    mean = df[1]
    std_div = df[2]

    plt.errorbar(n, mean, yerr=std_div, linestyle="None",  fmt="ob",  capsize=3,  ecolor="r", label=title)
    plt.xlabel("Number of Particles")
    plt.ylabel("Execution time (s)")
    plt.title(f"Error Bar {title}")
    plt.legend()
    plt.savefig(f"{title}.png")
    plt.clf()

plot_errorbar("../data/simple", "Simple")
plot_errorbar("../data/reduced", "Reduced")
