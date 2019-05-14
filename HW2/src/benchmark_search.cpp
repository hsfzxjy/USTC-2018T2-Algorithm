// We isolate the testing and initializing phase to get more accurate result

#include "RBTree.h"
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

RBTree<long> *tree = new RBTree<long>;
static long rbtreeSize = 1000;
static RBTree<long>::node_t node = nullptr, searchNode = nullptr;
static long num = 0;
static int testTimes = 1000;
static double step = 0;

void initRBTree () {
    num = ceil(num + step);
    // cout << num << endl;
    if (num >= rbtreeSize) num = rbtreeSize - 1;
}

void testSearch () {
    searchNode = tree->Search(num);
}

void testMin () {
    tree->Minimum();
}

void testMax () {
    tree->Maximum();
}

void testPred () {
    tree->Pred(tree->root);
}

void testSucc () {
    tree->Succ(tree->root);
}

int main () {

    ofstream fsearch, fmin, fmax, fpred, fsucc;
    double time;

    fsearch.open("benchmarks/search.dat");
    fmin.open("benchmarks/min.dat");
    fmax.open("benchmarks/max.dat");
    fsucc.open("benchmarks/succ.dat");
    fpred.open("benchmarks/pred.dat");
    fsearch << testTimes << endl;
    fmin << testTimes << endl;
    fmax << testTimes << endl;
    fsucc << testTimes << endl;
    fpred << testTimes << endl;
    for (long i = 1000; i < 10000000; i += 1000) {
        rbtreeSize = i;
        num = 0;
        step = (double) rbtreeSize / testTimes;
        if (!step) step = 1;

        for (long j = rbtreeSize - 1000; j < rbtreeSize; ++j) {
            tree->Insert(j);
        }

        time = getDuration(testSearch, initRBTree, testTimes);
        fsearch << i << ' ' << time << endl;
        cout << "Search\t" << i << ": " << time << endl;
        time = getDuration(testMin, initRBTree, testTimes);
        fmin << i << ' ' << time << endl;
        cout << "Min\t" << i << ": " << time << endl;
        time = getDuration(testMax, initRBTree, testTimes);
        fmax << i << ' ' << time << endl;
        cout << "Max\t" << i << ": " << time << endl;
        time = getDuration(testSucc, initRBTree, testTimes);
        fsucc << i << ' ' << time << endl;
        cout << "Succ\t" << i << ": " << time << endl;
        time = getDuration(testPred, initRBTree, testTimes);
        fpred << i << ' ' << time << endl;
        cout << "Pred\t" << i << ": " << time << endl;
    }
    fsearch.close();
    fmin.close();
    fmax.close();
    fsucc.close();
    fpred.close();
    return 0;
}
