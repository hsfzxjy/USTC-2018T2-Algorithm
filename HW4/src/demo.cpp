#define PRINT_RESULT

#include <iostream>
#include <ostream>
#include <fstream>
#include "graph.hpp"
#include "johnson.hpp"
#include "floyd.hpp"

using namespace std;

void saveGraph (const Graph& G) {
    ofstream f;
    f.exceptions(ofstream::badbit);
    try {
        f.open("fig/demo.dot");
    } catch (const ofstream::failure& e) {
        return;
    }

    f << "digraph g {" << endl;
    G.iterateEdges([&f] (vertex_t v, vertex_t u, weight_t w) {
        f << v << " -> " << u << " [label=" << w << "];" << endl;
        return true;
    });
    f << "}";
    return;
}

int main () {

    Graph G(5);

    G.link(0, 1, 4);
    G.link(0, 2, 13);
    G.link(0, 4, 0);
    G.link(1, 3, 0);
    G.link(1, 4, 10);
    G.link(2, 1, 0);
    G.link(3, 2, 0);
    G.link(3, 0, 2);
    G.link(4, 3, 2);

    cout << "We use the graph specified by Fig. 25-6 as a demostration." << endl;
    cout << "===> Now we use Floyd Algorithm <===" << endl;
    floyd(G);

    cout << "===> Now we use Johnson Algorithm <===" << endl;
    johnson(G);

    G.release();

    Graph G2(11);
    srand(1);
    generateGraph(G2, true);

    saveGraph(G2);

    cout << endl << "----------------------------------" << endl;
    cout << "Next, we generate a randomized graph, as visualized in my report." << endl;


    cout << "===> Now we use Floyd Algorithm <===" << endl;
    floyd(G2);
    cout << "===> Now we use Johnson Algorithm <===" << endl;
    if (!johnson(G2)) cout << "There exists a negative circle!" << endl;

    G2.release();

    return 0;
}
