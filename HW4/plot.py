#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

def n2logn(x, a, c, d, e):

    return a * x ** 2 * np.log(x) + c * x ** 2 + d * x + e


n2logn.label = lambda a, c, d, e: f'{a} V^2 log(V) + {c} V^2 + {d} V + {e}'

def n3(x, a, b, c, d):

    return a * x ** 3 + b * x ** 2 + c * x + d


n3.label = lambda a, b, c, d: f'{a} V^3 + {b} V^2 + {c} V + {d}'

def get_arr(fn):

    arr = []
    with open(fn) as f:
        for line in f:
            arr.append(list(map(float, line.strip().split())))
    # print(arr)
    arr = np.array(arr)

    return arr


if __name__ == '__main__':

    f, j = map(get_arr, ['benchmark/floyd.dat', 'benchmark/johnson.dat'])

    x = np.linspace(1, 1050, 1050)
    plt.figure(figsize=(10, 20))
    popt, _ = curve_fit(n3, f[:, 0], f[:, 1])
    ffit = n3(x, *popt)
    flabel = n3.label(*('{:.5g}'.format(x) for x in popt))
    popt, _ = curve_fit(n2logn, j[:, 0], j[:, 1])
    jfit = n2logn(x, *popt)
    jlabel = n2logn.label(*('{:.5g}'.format(x) for x in popt))

    plt.plot(f[:, 0], f[:, 1], c='tab:blue', lw=2.5, label='Floyd-Warshall')
    plt.plot(x, ffit, lw=1.5, ls='--', c='red', label=flabel)

    plt.plot(j[:, 0], j[:, 1], lw=2.5, c='green', label='Johnson')
    plt.plot(x, jfit, lw=1.5, ls='--', c='orange', label=jlabel)
    plt.xlabel('Number of Vertices')
    plt.ylabel('Time / Sec(s)')
    plt.legend()
    plt.grid()

    p = plt.gcf()
    plt.show()
    p.savefig('fig/time.eps')
