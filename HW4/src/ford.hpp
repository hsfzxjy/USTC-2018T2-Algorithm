#ifndef _FORD_H_
#define _FORD_H_

#include "graph.hpp"

typedef weight_t* ford_result_t;

inline bool ford (const Graph& G, vertex_t s, ford_result_t& d) {
    auto n = G.size();
    d = new weight_t[n];
    for (auto i = 0; i < n; i++) {
        d[i] = WMAX;
    }
    d[s] = 0;

    for (auto _ = 0; _ < n - 1; _++) {

        G.iterateEdges([&d](vertex_t u, vertex_t v, weight_t w) {
            if (d[v] > d[u] + w) {
                d[v] = d[u] + w;
            }
            return true;
        });
    }

    auto hasNegativeCircle = !G.iterateEdges([&d](vertex_t u, vertex_t v, weight_t w) {
        return d[v] <= d[u] + w;
    });

    if (hasNegativeCircle) {
        delete[] d;
        d = nullptr;
        return false;
    }

    return true;
}


#endif
