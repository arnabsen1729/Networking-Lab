from random import randint
from matplotlib import pyplot as plt
import numpy as np
import subprocess
import os
from time import sleep


def read_file(filename):
    with open(filename) as f:
        data = [lines.split(', ') for lines in f.readlines()]
        return {int(x): float(y) for x, y in data}


def plot():
    system_times = read_file('system_time.csv')
    response_times = read_file('response_time.csv')

    x = list(system_times.keys())
    y = [[system_times[id] for id in x], [response_times[id] for id in x]]

    X_axis = np.arange(len(x))
    plt.bar(X_axis - 0.2, y[0], 0.4, label='System Time')
    plt.bar(X_axis + 0.2, y[1], 0.4, label='Response Time')
    plt.xticks(X_axis, x)
    plt.xlabel("Client Ports")
    plt.ylabel("Time (in ms)")
    plt.title("Plot of System and Response Times")
    plt.legend()
    plt.show()


def generate_input():
    arr = [str(randint(0, 100)) for _ in range(10)]
    return ' '.join(arr)


def run_processes():
    os.remove('response_time.csv')
    for _ in range(10):
        arg = generate_input()
        subprocess.run(['./client_tcp', arg])
        sleep(1)


def main():
    run_processes()
    plot()


if __name__ == "__main__":
    main()
