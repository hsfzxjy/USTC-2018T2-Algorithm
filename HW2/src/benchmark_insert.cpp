// We isolate the testing and initializing phase to get more accurate result

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
        (*initializer)();
        start = clock();
        (*code)();
        end = clock();
        elapsedTime += (end - start)/(double) CLOCKS_PER_SEC;
    }

    return elapsedTime;
}

RBTree<long> *tree = nullptr;
static long rbtreeSize = 1000;
static RBTree<long>::node_t nodeInserted = nullptr;
static long numberToInsert = 0;

void InsertInitRBTree () {
    if (tree != nullptr) {
        tree->Delete(nodeInserted);
        numberToInsert += rbtreeSize / 1000;
        return;
    }
    tree = new RBTree<long>;
    srand(0);
    for (long i = 0; i < rbtreeSize; ++i) {
        tree->Insert(i);
    }
}

void testInsert () {
    nodeInserted = tree->Insert(numberToInsert);
}

static int testTimes = 1000;

int main () {

    ofstream fout;

    fout.open("benchmarks/insert.dat");
    fout << testTimes << endl;
    for (long i = 1000; i < 1000000; i += 1000) {
        delete tree;
        tree = nullptr;
        rbtreeSize = i;
        numberToInsert = 0;
        double time = getDuration(testInsert, InsertInitRBTree, testTimes);
        fout << i << ' ' << time << endl;
        cout << i << ": " << time << endl;
    }
    return 0;
}
