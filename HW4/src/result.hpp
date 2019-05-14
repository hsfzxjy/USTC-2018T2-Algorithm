#ifndef _RESULT_H_
#define _RESULT_H_

#include "graph.hpp"

struct ResultNode {
    weight_t d;
    vertex_t pred;

    ResultNode (): d(WMAX), pred(-1) {};
};

typedef ResultNode* result_t;

inline void printResult (vertex_t u, result_t r, size_t n) {
    for (auto v = 0; v < n; ++v) {
        if (v == u) continue;

        if (r[v].d >= WMAX) {
            cout << v << " <~~ " << u << " (inf): NOT CONNECTED" << endl;
            continue;
        }

        cout << v << " <~~ " << u << " (" << r[v].d << "):\t";

        auto t = v;
        cout << t;
        while (t != u) {
            t = r[t].pred;
            cout << " <- " << t;
        }
        cout << endl;
    }
    cout << endl;
}


#endif
