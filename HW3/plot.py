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


nlogn.label = lambda a, b, c, d: f'{c}N log(N + {b}) + {d} N + {a}'

def n(x, a, b):

    return a * x + b


n.label = lambda a, b: f'{a}N + {b}'

def n3(x, a, b, c, d):

    return a * x * x * x + b * x * x + c * x + d


n3.label = lambda a, b, c, d: f'{a} N^3 + {b} N^2 + {c} N + {d}'

# def label(f, *args):  # a, b, c, d=None):

#     return ''

#     s = '{:.5g} * {}ln(N + {:.5g}) + {:.5g}{}'.format(
#         c,
#         'N * ' if f == nlogn else '',
#         b,
#         a,
#         ' + {:.5g}N'.format(d) if d is not None else ''
#     )
#     return s

def label(*args):
    return ''


with open(options.FILE, 'r') as f:
    lines = f.readlines()
    data = np.array([list(map(float, x.split())) for x in lines[1:]])
    times = int(lines[0].strip())
    x = data[:, 0]
    data[:, 1] = data[:, 1] / times * 1e4
    if 'subarray1' in options.FILE:
        func = nlogn
    elif 'matmul' in options.FILE:
        func = n3
    else:
        func = n
    popt, pcov = curve_fit(func, x, data[:, 1])

    plt.figure(figsize=(10, 5))
    plt.grid(True)
    plt.xlabel('N')
    plt.ylabel(f'Average Time Per Run / x 1e-4 sec')
    plt.scatter(data[:, 0], data[:, 1], s=2, color='black', label='data')
    plt.plot(x, func(x, *popt), label=func.label(*map('{:.6g}'.format , popt)), color='red', lw=2)
    plt.legend()
    if options.OUT != '0':
        plt.savefig(options.OUT)
    else:
        plt.show()
