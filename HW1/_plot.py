import scipy.io as io
import numpy as np
import matplotlib.pyplot as plt


def get_error(data, avg):

    avg = avg.reshape((avg.shape[0], 1))
    print(avg, k_nums.item())
    avg = avg.repeat(k_nums, axis=1)
    error = (data - avg) / avg
    return np.average(error, axis=0)


def maximize():

    manager = plt.get_current_fig_manager()
    manager.resize(*manager.window.maxsize())


def _plot():

    plt.ion()

    rsr_avg = np.average(randomized_select_result, axis=1)
    sr_avg = np.average(select_result, axis=1)
    rsr_std = np.std(randomized_select_result, axis=1) / rsr_avg
    sr_std = np.std(select_result, axis=1) / sr_avg

    rsr_error = get_error(randomized_select_result, rsr_avg)
    sr_error = get_error(select_result, sr_avg)

    plt.figure(figsize=(12, 6))
    plt.plot(nums, rsr_avg, label='randomized_select')
    plt.plot(nums, sr_avg, label='select')
    plt.xticks(nums, [f'{x//1000}' for x in nums])
    plt.grid(True)
    plt.xlabel('Number of Elements / x1e3')
    plt.ylabel('Average Time of {} Runs / sec(s)'.format(dct['run_times'].item()))
    plt.legend()
    maximize()
    plt.show()
    plt.savefig(f'figures/{ver}_fig1.eps')

    plt.figure(figsize=(12, 6))
    plt.plot(nums, rsr_std, label='randomized_select')
    plt.plot(nums, sr_std, label='select')
    plt.xticks(nums, [f'{x//1000}' for x in nums])
    plt.grid(True)
    plt.xlabel('Number of Elements / x1e3')
    plt.ylabel('Variant Coefficient')
    maximize()
    plt.legend()
    plt.show()
    plt.savefig(f'figures/{ver}_fig2.eps')

    plt.figure(figsize=(12, 6))
    plt.plot(range(k_nums), rsr_error, label='randomized_select')
    plt.plot(range(k_nums), sr_error, label='select')
    plt.xticks(range(k_nums), ['0'] + [f'{x}/10' for x in range(1, 10)])
    plt.grid(True)
    plt.xlabel('Quantile')
    plt.ylabel('Relative Running Time')
    maximize()
    plt.legend()
    plt.show()
    plt.savefig(f'figures/{ver}_fig3.eps')


import argparse
import os
os.makedirs('figures', exist_ok=True)
parser = argparse.ArgumentParser()
parser.add_argument('F')
dct = io.loadmat(parser.parse_args().F)
randomized_select_result = dct['rsr']
select_result = dct['sr']
nums = dct['nums'].squeeze()
k_nums = dct['k_nums'].squeeze()
ver = dct['sys_version'][0]
print(ver)
_plot()
