#ifndef _DIJKSTRA_H_
#define _DIJKSTRA_H_

#include "graph.hpp"
#include "ford.hpp"
#include "min_queue.hpp"
#include "result.hpp"

typedef result_t dijkstra_result_t;

struct QueueNode {

    typedef vertex_t hash_value_t;

    vertex_t v;
    ResultNode *result_node;

    bool operator< (QueueNode& rhs) {
        return result_node->d < rhs.result_node->d;
    }

    bool operator> (QueueNode& rhs) {
        return result_node->d > rhs.result_node->d;
    }

    inline hash_value_t hash_value () {
        return v;
    }

    QueueNode (vertex_t v_, ResultNode* r): v(v_), result_node(r) {};
    QueueNode (): v(0), result_node(nullptr) {};
};

// w'(u, v) = w(u, v) + h(u) - h(v)
inline dijkstra_result_t dijkstra (const Graph& G, vertex_t s, ford_result_t h) {
    long n = G.size();
    auto result = new ResultNode[n];
    result[s].d = 0;

    min_queue<QueueNode> Q(n, [&result](size_t i) {
        return QueueNode(i, &result[i]);
    });

    while (!Q.empty()) {
        auto qn = Q.extractMin();

        G.iterateEdges(qn.v, [&qn, &Q, &result, &h](vertex_t u, vertex_t v, weight_t w) {
            auto new_w = result[u].d + w + h[u] - h[v];
            if (result[v].d > new_w) {
                result[v].d = new_w;
                result[v].pred = u;
                Q.decreaseKey(v);
            }
            return true;
        });
    }

    for (auto i = 0; i < n; i++)
        result[i].d += h[i] - h[s];

    return result;
}

#endif
