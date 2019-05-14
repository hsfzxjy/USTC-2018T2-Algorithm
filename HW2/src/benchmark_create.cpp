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
        if (initializer) (*initializer)();
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

void init () {
    if (tree != nullptr)
        delete tree;
}

void testCreate () {
    srand(0);
    tree = new RBTree<long>;
    for (long i = 0; i < rbtreeSize; i++) {
        tree->Insert(random());
    }
}

static int testTimes = 1;

int main () {

    ofstream fout;

    fout.open("benchmarks/create.dat");
    fout << testTimes << endl;
    for (long i = 1000; i < 500000; i += 1000) {
        rbtreeSize = i;
        double time = getDuration(testCreate, init, testTimes);
        fout << i << ' ' << time << endl;
        cout << i << ": " << time << endl;
    }
    fout.close();
    return 0;
}
