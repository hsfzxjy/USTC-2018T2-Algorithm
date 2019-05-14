#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include <climits>
#include <functional>
#include <cstdlib>

using namespace std;

typedef int vertex_t;
typedef double weight_t;
typedef std::function<bool(vertex_t, vertex_t, weight_t)> edge_functor_t;

const weight_t WMAX = ULONG_MAX / 3;

struct VertexNode {
    vertex_t v;
    VertexNode *prev, *next;
    weight_t weight;
    VertexNode (): prev(nullptr), next(nullptr), weight(0) {};
    VertexNode (vertex_t v_, weight_t w): v(v_), weight(w) {};
};

typedef VertexNode* VertexNodeLinkedList;

// No support for vertex or edge removing, for simplicity.
class Graph {
public:
    typedef vector<VertexNodeLinkedList> vlist_t;
    vlist_t vlist;
public:

    Graph () {};

    Graph (size_t nVertices) {
        vlist.reserve(nVertices);
        for (auto i = 0; i < nVertices; i++) addVertex();
    }

    inline void release (vertex_t u) {
        auto node = vlist[u];
        while (node != nullptr) {
            auto next = node->next;
            delete node;
            node = next;
        }
    }

    inline void release () {

        for (long i = size() - 1; i >= 0; --i)
            release(i);
    }

    inline size_t addVertex () {
        auto linked_list_head = new VertexNode;
        this->vlist.push_back(linked_list_head);
        return this->vlist.size() - 1;
    }
    inline void link(vertex_t u, vertex_t v, weight_t w) {
        auto head = this->vlist[u];
        auto node = new VertexNode(v, w);
        auto head_next = head->next;
        node->next = head_next;
        node->prev = head;
        head->next = node;
        if (head_next != nullptr) head_next->prev = node;
    }

    inline size_t size () const {
        return this->vlist.size();
    }

    inline bool iterateEdges (edge_functor_t f) const {
        auto n = size();
        for (auto i = 0; i < n; ++i) {
            if (!this->iterateEdges(i, f)) return false;
        }
        return true;
    }

    inline bool iterateEdges (vertex_t u, edge_functor_t f) const {
        auto node = this->vlist[u]->next;
        while (node != nullptr) {
            if (!f(u, node->v, node->weight)) {
                return false;
            }
            node = node->next;
        }
        return true;
    }
};


template<typename T>
inline T random (T low, T high) {
    return rand() % (long)(high - low + 1) + low;
}


inline void generateGraph (Graph& G, bool negative) {
    auto n = G.size();
    for (long i = 0; i < n; i++) {
        int count = random<int>(5, 9);
        int j = -1;
        while (count--) {
            j++;
            if (j == i) j++;

            weight_t weight;
            if (negative) {
                weight = random<weight_t>(-2, 17);
            } else {
                weight = random<weight_t>(1, 20);
            }

            G.link(i, j, weight);
        }
    }
}

#endif
