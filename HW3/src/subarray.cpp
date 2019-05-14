#include <iostream>
#include <tuple>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <ostream>
#include <fstream>
#include <ctime>

using namespace std;

typedef std::tuple<long, long, long> result_t;
const long _inf = LONG_MIN;
const long testTimes = 1;

inline int isDemo () {
    const char* bm = std::getenv("bm");

    if (!bm) {
        return 0;
    } else if (strcmp(bm, "1") == 0) {
        return 1;
    } else {
        return 2;
    }
}

// Both sides included
inline long random (long low, long high) {
    return rand() % (high - low + 1) + low;
}

inline long getLow(result_t r) {
    return std::get<0>(r);
}

inline long getHigh(result_t r) {
    return std::get<1>(r);
}

inline long getSum(result_t r) {
    return std::get<2>(r);
}

inline result_t findMaxCrossingSubarray (long* A, long low, long mid, long high) {
    long leftsum = _inf, sum = 0;
    long maxleft = mid;
    for (long i = mid; i >= low; i--) {
        sum += A[i];
        if (sum > leftsum) {
            leftsum = sum;
            maxleft = i;
        }
    }
    long rightsum = _inf;
    sum = 0;
    long maxright = mid + 1;
    for (long j = mid + 1; j <= high; j++) {
        sum += A[j];
        if (sum > rightsum) {
            rightsum = sum;
            maxright = j;
        }
    }
    return result_t(maxleft, maxright, leftsum + rightsum);
}

result_t findMaxSubarray1 (long* A, long low, long high) {
    if (high == low) return result_t(low, high, A[low]);
    long mid = (low + high) >> 1;
    auto left_result = findMaxSubarray1(A, low, mid);
    auto right_result = findMaxSubarray1(A, mid + 1, high);
    auto cross_result = findMaxCrossingSubarray(A, low, mid, high);

    auto leftsum = getSum(left_result), rightsum = getSum(right_result),
        crosssum = getSum(cross_result);

    if (leftsum >= rightsum && leftsum >= crosssum) {
        return left_result;
    } else if (rightsum >= leftsum && rightsum >= crosssum) {
        return right_result;
    } else {
        return cross_result;
    }
}

result_t findMaxSubarray2 (long* A, long low, long high) {
    long sum = 0;
    long start = low, end = low, curstart = low, curend = low;
    long curmax = _inf;
    for (long i = low; i < high; i++) {
        if (sum >= 0) {
            sum += A[i];
            curend = i;
        } else {
            sum = A[i];
            curstart = curend = i;
        }
        if (curmax < sum) {
            start = curstart;
            end = curend;
            curmax = sum;
        }
    }

    return result_t(start, end, curmax);
}

int main () {
    int demo = isDemo();

    if (demo == 0) {
        long A[] = {13, -3, -25, -20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7};
        long n = sizeof(A) / sizeof(long);
        cout << "Testing on data from Figure 4.3: " << endl << "Data: ";
        for (long i = 0; i < n; i++)
            cout << A[i] << ' ';
        cout << endl;

        cout << "Using Divide and Conquer:" << endl;
        auto dac = findMaxSubarray1(A, 0, n - 1);
        cout << "Range:\t" << getLow(dac) << "..." << getHigh(dac) << endl;
        cout << "Sum:\t" << getSum(dac) << endl;

        cout << "Using Greedy Algorithm:" << endl;
        auto ga = findMaxSubarray2(A, 0, n - 1);
        cout << "Range:\t" << getLow(ga) << "..." << getHigh(ga) << endl;
        cout << "Sum:\t" << getSum(ga) << endl;
    } else if (demo == 1) {

        clock_t start, end;
        double secs;

        srand(0);
        ofstream f1;
        f1.open("benchmarks/subarray1.dat");
        f1 << 1 << endl;
        for (long n = 1000; n <= 3000000; n += 1000) {
            long *A = new long[3000000];
            for (long i = n - 1000; i < n; i++) {
                A[i] = random(-(n >> 1), n >> 1);
            }

            start = clock();
            for (long i = 0; i < 1; i++) findMaxSubarray1(A, 0, n-1);
            end = clock();

            secs = (double)(end - start) / CLOCKS_PER_SEC;
            cout << n << ' ' << secs << endl;
            f1 << n << ' ' << secs << endl;

            // delete[] A;
        }

        f1.close();
    } else {
        clock_t start, end;
        double secs;

        srand(0);
        ofstream f2;

        f2.open("benchmarks/subarray2.dat");
        f2 << 10 << endl;
        for (long n = 1000; n <= 3000000; n += 1000) {
            long *A = new long[3000000];
            for (long i = n - 1000; i < n; i++) {
                A[i] = random(-(n >> 1), n >> 1);
            }

            start = clock();
            for (long i = 0; i < 10; i++) findMaxSubarray2(A, 0, n-1);
            end = clock();

            secs = (double)(end - start) / CLOCKS_PER_SEC;
            cout << n << ' ' << secs << endl;
            f2 << n << ' ' << secs << endl;

            delete[] A;
        }

        f2.close();

    }

    return 0;
}
