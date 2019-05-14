#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ostream>
#include <fstream>
#include <ctime>

using namespace std;

typedef unsigned long type;
const type inf = 0x3f3f3f3f;
const long testTimes = 5;
bool echo_on = true;

inline bool isDemo () {
    const char* bm = std::getenv("bm");
    return !bm;
}

// Both sides included
inline type random (type low, type high) {
    return rand() % (high - low + 1) + low;
}


void printOptimalParens (type** s, type i, type j) {
    if (i == j)
        cout << 'A' << i;
    else {
        cout << '(';
        printOptimalParens(s, i, s[i][j]);
        printOptimalParens(s, s[i][j] + 1, j);
        cout << ')';
    }
}


void matrixChainOrder (type* p, type n) {
    type** m = new type*[n + 1], **s = new type*[n + 1];
    for (type i = 0; i <= n; i++) {
        m[i] = new type[n + 1];
        s[i] = new type[n + 1];
    }

    for (type i = 0; i <= n; i++) {
        m[i][i] = 0;
    }

    for (type l = 2; l <= n; l++) {
        for (type i = 1; i <= n - l + 1; i++) {
            type j = i + l - 1;
            m[i][j] = inf;
            for (type k = i; k <= j - 1; k++) {
                type q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (q < m[i][j]) {
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }

    if (echo_on) {
        cout << "Optimal Operations Count:\t" << m[1][n] << endl;
        printOptimalParens(s, 1, n);
        cout << endl;
    }

    for (type i = 0; i <= n; i++) {
        delete[] m[i];
        delete[] s[i];
    }

    delete[] m, s;
}


int main () {
    if (isDemo()) {
        type p[] = {30, 35, 15, 5, 10, 20, 25};
        type n = (sizeof(p)) / (sizeof(type)) - 1;
        matrixChainOrder(p, n);
    } else {

        echo_on = false;

        clock_t start, end;
        double secs;

        srand(0);
        ofstream f;

        f.open("benchmarks/matmul.dat");
        f << testTimes << endl;
        for (long n = 2; n <= 1000; n += 1) {
            type *A = new type[n];
            for (long i = 0; i < n; i++) {
                A[i] = random(1, 20);
            }

            start = clock();
            for (long i = 0; i < testTimes; i++) matrixChainOrder(A, n - 1);
            end = clock();

            secs = (double)(end - start) / CLOCKS_PER_SEC;
            cout << n << ' ' << secs << endl;
            f << n << ' ' << secs << endl;

            delete[] A;
        }

        f.close();
    }

    return 0;
}
