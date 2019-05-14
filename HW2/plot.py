import argparse
import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit
parser = argparse.ArgumentParser()
parser.add_argument('FILE')
parser.add_argument('OUT')

options = parser.parse_args()

def nlogn(x, a, b, c, d):

    return a + x * np.log(b + x) * c + d * x

def logn(x, a, b, c):

    return a + np.log(b + x) * c


def label(f, a, b, c, d=None):

    s = '{:.5g} * {}ln(N + {:.5g}) + {:.5g}{}'.format(
        c,
        'N * ' if f == nlogn else '',
        b,
        a,
        ' + {:.5g}N'.format(d) if d is not None else ''
    )
    return s


with open(options.FILE, 'r') as f:
    lines = f.readlines()
    data = np.array([list(map(float, x.split())) for x in lines[1:]])
    times = int(lines[0].strip())
    x = data[:, 0]
    data[:, 1] = data[:, 1] / times * 1e4
    if 'create' in options.FILE:
        func = nlogn
    else:
        func = logn
    popt, pcov = curve_fit(func, x, data[:, 1])

    plt.figure(figsize=(10, 5))
    plt.grid(True)
    plt.xlabel('N')
    plt.ylabel(f'Average Time Per Run / x 1e-4 sec')
    plt.scatter(data[:, 0], data[:, 1], s=2, color='black', label='data')
    plt.plot(x, func(x, *popt), label=label(func, *popt), color='red', lw=2)
    plt.legend()
    if options.OUT != '0':
        plt.savefig(options.OUT)
    else:
        plt.show()
