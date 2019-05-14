#ifndef _JOHNSON_H_
#define _JOHNSON_H_

#include "graph.hpp"
#include "ford.hpp"
#include "dijkstra.hpp"

inline vertex_t preprocess (const Graph& G, Graph& newG) {
    newG.vlist = G.vlist;
    auto vid = newG.addVertex();
    for (auto i = 0; i < vid; i++) {
        newG.link(vid, i, 0);
    }
    return vid;
}

// Instead of returning results to caller, we simply print out them.
inline bool johnson (const Graph& G) {
    Graph newG;
    auto s = preprocess(G, newG);

    ford_result_t ford_result;
    if (!ford(newG, s, ford_result)) {
        return false;
    }

    for (vertex_t u = 0; u < G.size(); ++u) {
        auto r = dijkstra(G, u, ford_result);

#ifdef PRINT_RESULT
        printResult(u, r, G.size());
#endif

        delete[] r;
    }

    delete[] ford_result;
    newG.release(newG.size() - 1);

    return true;
}

#endif
