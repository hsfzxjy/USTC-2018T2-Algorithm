#undef PRINT_RESULT

#include <iostream>
#include <ctime>
#include <ostream>
#include <fstream>
#include "graph.hpp"
#include "johnson.hpp"
#include "floyd.hpp"

const int testTimes = 1000;

ofstream floyd_result, johnson_result;

void testGraph (size_t n) {
    Graph G(n);
    generateGraph(G, false);

    clock_t start, end;
    double secs;

    start = clock();
    // for (int i = 0; i < testTimes; i++)
        floyd(G);
    end = clock();


    secs = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "Floyd: " << secs << endl;

    floyd_result << n << ' ' << secs << endl;

    // --------

    start = clock();
    // for (int i = 0; i < testTimes; i++)
        johnson(G);
    end = clock();
    secs = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "Johnson: " << secs << endl;

    johnson_result << n << ' ' << secs << endl;

    G.release();
}

int main () {
    floyd_result.open("benchmark/floyd.dat");
    johnson_result.open("benchmark/johnson.dat");

    for (int i = 10; i <= 1000; i += 5) testGraph(i);

    floyd_result.close();
    johnson_result.close();
    return 0;
}
