#include "RBTree.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

double getDuration (void (*code)(), void (*initializer)(), long times) {
    double elapsedTime = 0.;
    double start, end;

    while (times > 0) {
        --times;
        if (initializer != nullptr)
            (*initializer)();
        start = clock();
        (*code)();
        end = clock();
        elapsedTime += (end - start)/(double) CLOCKS_PER_SEC;
    }

    return elapsedTime;
}

RBTree<long> *tree = new RBTree<long>;
static long rbtreeSize = 1000;
static RBTree<long>::node_t nodeInserted = nullptr;
static long numberToInsert = 0;
static int testTimes = 1000;

void initRBTree () {
    // if (tree != nullptr) {
    //     // numberToInsert += rbtreeSize / testTimes;
    //     return;
    // }
    // tree = new RBTree<long>;
    // srand(0);
    for (long i = rbtreeSize - 1000; i < rbtreeSize; ++i) {
        tree->Insert(i);
    }
}

void testSearch () {
    tree->Maximum();
}


int main () {

    ofstream fout;

    fout.open("benchmarks/min.dat");
    fout << testTimes << endl;
    for (long i = 1000; i < 100000000; i += 1000) {
        rbtreeSize = i;
        numberToInsert = 0;

        for (long j = rbtreeSize - 1000; j < rbtreeSize; ++j) {
            tree->Insert(j);
        }

        double time = getDuration(testSearch, nullptr, testTimes);
        fout << i << ' ' << time << endl;
        cout << i << ": " << time << endl;
    }
    return 0;
}
