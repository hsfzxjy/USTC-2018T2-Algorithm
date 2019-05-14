// We isolate the testing and initializing phase to get more accurate result

#include "OSTree.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>

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

OSTree<long> *tree = new OSTree<long>;
static long rbtreeSize = 1000;
static OSTree<long>::node_t node = nullptr;
static long num = 0;
static int testTimes = 5000;
static double step = 0;

void initRBTree () {
    num = ceil(num + step);
    if (num >= rbtreeSize) num = rbtreeSize - 1;
}

void testSelect () {
    node = tree->Select(num);
}

void testRank () {
    tree->Rank(node);
}

int main () {

    ofstream fselect, frank;

    fselect.open("benchmarks/select.dat");
    frank.open("benchmarks/rank.dat");
    fselect << testTimes << endl;
    frank << testTimes << endl;
    for (long i = 1000; i < 10000000; i += 1000) {
        rbtreeSize = i;
        num = 0;
        step = (double) rbtreeSize / testTimes;
        if (!step) step = 1;

        for (long j = rbtreeSize - 1000; j < rbtreeSize; ++j) {
            tree->Insert(j);
        }

        double time = getDuration(testSelect, initRBTree, testTimes);
        fselect << i << ' ' << time << endl;
        cout << "Select\t" << i << ": " << time << endl;
        time = getDuration(testRank, initRBTree, testTimes);
        frank << i << ' ' << time << endl;
        cout << "Rank\t" << i << ": " << time << endl;
    }
    fselect.close();
    frank.close();
    return 0;
}
