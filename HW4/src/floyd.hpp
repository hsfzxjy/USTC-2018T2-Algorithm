#ifndef _FLOYD_H_
#define _FLOYD_H_

#include "graph.hpp"
#include "result.hpp"

using namespace std;

typedef result_t floyd_result_t;

void floyd (const Graph& G) {
    auto n = G.size();

    auto R = new result_t[n];

    for (auto i = 0; i < n; i++) {

        R[i] = new ResultNode[n];

        G.iterateEdges(i, [&R](vertex_t u, vertex_t v, weight_t w) {
            R[u][v].d = w;
            R[u][v].pred = u;
            return true;
        });
    }

    for (auto k = 0; k < n; k++)
        for (auto i = 0; i < n; i++)
            for (auto j = 0; j < n; j++) {
                auto cur = R[i][k].d + R[k][j].d;
                if (cur < R[i][j].d) {
                    R[i][j].d = cur;
                    R[i][j].pred = R[k][j].pred;
                }
            }

#ifdef PRINT_RESULT
    for (auto i = 0; i < n; i++)
        printResult(i, R[i], n);
#endif

    for (auto i = 0; i < n; i++) delete[] R[i];
    delete[] R;
}

#endif
