#!/usr/bin/env python3

from math import ceil
import random
from random import randint
from timeit import timeit


def partition(A, p, r, x):
    """
    To partition A[p..r] using pivot with value `x`.
    """

    # Find index of the element whose value equals to `x`
    index = None
    for i_ in range(p, r + 1):
        if A[i_] == x:
            index = i_
            break
    # The caller should promise that `x` falls in A[p..r]
    assert index is not None

    # Ordinary partition procedure
    A[index], A[r] = A[r], A[index]
    x = A[r]
    i = p - 1
    for j in range(p, r):
        if A[j] <= x:
            i += 1
            A[i], A[j] = A[j], A[i]
    A[i + 1], A[r] = A[r], A[i + 1]
    return i + 1


def randomized_partition(A, p, r):
    """
    To partition A[p..r] using a randomized pivot.
    """

    # Randomly choose a pivot index from [p..r]
    index = randint(p, r)
    A[index], A[r] = A[r], A[index]

    # Ordinary partition process
    x = A[r]
    i = p - 1
    for j in range(p, r):
        if A[j] <= x:
            i += 1
            A[i], A[j] = A[j], A[i]
    A[i + 1], A[r] = A[r], A[i + 1]
    return i + 1


def randomized_select(A, p, r, i):
    """
    The algorithm randomized_select (Recursion-free version)
    """

    while p != r:
        q = randomized_partition(A, p, r)
        k = q - p + 1
        if i == k:
            return A[q]
        elif i < k:
            r = q - 1
        else:
            p = q + 1
            i = i - k

    return A[p]


def select(A, p, r, position):
    """
    The algorithm select (semi-recursive version)
    """

    while p + 1 < r:
        N = r - p + 1
        N_medians = ceil(N / 5)

        # Pre-allocate the array to store the medians
        medians = [0] * N_medians

        # The following for-loop finds out then median of each small interval,
        # using naive select-sort algorithm
        # Note that we do it in-place, instead of copying the intervals to some
        # new arrays, to save execution time
        for i in range(N_medians):
            start = p + i * 5
            end = min(start + 5, r + 1)
            N_slice = end - start

            # Naive select-sort
            for j in range(start + 1, end):
                key = A[j]
                k = j - 1
                while k >= start and A[k] > key:
                    A[k + 1] = A[k]
                    k -= 1
                A[k + 1] = key

            medians[i] = A[start + ((N_slice - 1) >> 1)]

        # Recursively call `select` to find out the median of medians
        # Note that this recursion is unavoidable. Expanding it will mess up the code
        m_of_m = select(medians, 0, N_medians - 1, (N_medians + 1) >> 1)

        # Here we simply get the value of `m_of_m`, instead of its index in
        # array A. To do the two jobs simultaneously we must store and track
        # the indices, which is space-consuming and may mess up the code.
        # Alternatively, we leave the job to `partition`.

        # Now we've got the median of medians, stored in `m_of_m`. From here
        # we can apply ordinary partition procedure
        q = partition(A, p, r, m_of_m)
        k = q - p + 1

        if position == k:
            return A[q]
        elif position < k:
            r = q - 1
        else:
            p = q + 1
            position = position - k

    # Return directly if the array slice contains less than 3 elements.
    if p == r:
        return A[p]
    elif p + 1 == r:
        if A[p] > A[r]:
            A[p], A[r] = A[r], A[p]
        return A[p + position - 1]


def benchmark(N, k_nums=10, run_times=1000):

    select_result, randomized_select_result = [], []

    data = [random.random() for _ in range(N)]
    for pos in range(0, N, N // k_nums):
        k = max(pos, 1)
        answer = sorted(data)[k - 1]
        print('Correctness (randomized):\t', randomized_select(data[:], 0, N - 1, k) == answer)
        print('Correctness (unrandomized):\t', select(data[:], 0, N - 1, k) == answer)
        print('N = {}, k = {}'.format(N, k))

        elapsed_time = timeit(
            'select(A, 0, N - 1, k)',
            number=run_times,
            setup='A = data[:]',
            globals={
                'select': randomized_select,
                'data': data,
                'N': N,
                'k': k,
            }
        )
        randomized_select_result.append(elapsed_time)
        print(
            'randomized\t',
            elapsed_time
        )

        elapsed_time = timeit(
            'select(A, 0, N - 1, k)',
            number=run_times,
            setup='A = data[:]',
            globals={
                'select': select,
                'data': data,
                'N': N,
                'k': k,
            }
        )
        select_result.append(elapsed_time)
        print(
            'unrandomized\t',
            elapsed_time
        )

    return randomized_select_result, select_result


if __name__ == '__main__':

    try:
        import argparse
        parser = argparse.ArgumentParser()
        parser.add_argument('-N', dest='N_count', default=10, type=int)
        parser.add_argument('-k', dest='k_nums', default=10, type=int)
        parser.add_argument('-base', dest='base', default=1000, type=int)
        parser.add_argument('-r', dest='run_times', default=1000, type=int)
        options = parser.parse_args()
        N_count = options.N_count
        k_nums = options.k_nums
        base = options.base
        run_times = options.run_times
    except Exception as e:
        print('Something wrong occurred')
        print(e)
        N_count, k_nums, base, run_times = 10, 10, 1000, 1000

    import sys
    if 'pypy' in sys.version.lower():
        sys_version = 'pypy'
    else:
        sys_version = 'cpython'

    print('---> Using', sys_version)

    randomized_select_result, select_result = [], []
    nums = []
    for i in range(1, N_count + 1):
        N = base * i
        nums.append(N)
        result = benchmark(N, k_nums=k_nums, run_times=run_times)
        randomized_select_result.append(result[0])
        select_result.append(result[1])

    try:
        import scipy.io as io
        import os
        os.makedirs('results', exist_ok=True)
        io.savemat('results/{}_result.mat'.format(sys_version), {
            'k_nums': k_nums,
            'rsr': randomized_select_result,
            'sr': select_result,
            'nums': nums,
            'run_times': run_times,
            'sys_version': sys_version
        })
    except ImportError:
        pass
