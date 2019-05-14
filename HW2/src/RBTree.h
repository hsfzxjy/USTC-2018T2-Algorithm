#ifndef RBTREE_H_
#define RBTREE_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

enum Color { RED, BLACK };

template <typename T>
struct Node {
    Color color;
    Node<T> *p, *left, *right;
    T data;
public:
    Node(Color c): color(c) {};
    Node(T data_): data(data_) {};
};

template <typename T>
class RBTree {
public:
    typedef Node<T> *node_t;
    typedef std::vector<node_t> vector_t;
    node_t root;
    node_t nil;
public:

    RBTree () {
        auto nil = this->nil = new Node<T>(Color::BLACK);
        nil->p = nil->left = nil->right = nil;
        this->root = nil;
    }

    // Dispose all nodes while destructing, otherwise memory will blow up
    ~RBTree () {
        Dispose(this->root);
        delete this->nil;
    }

    void Dispose (node_t x) {
        if (x == this->nil) return;
        Dispose(x->left);
        Dispose(x->right);

        delete x;
    }

    virtual void LeftRotate (node_t x) {
        auto y = x->right;
        x->right = y->left;
        if (y->left != this->nil) {
            y->left->p = x;
        }
        y->p = x->p;
        if (x->p == this->nil) {
            this->root = y;
        } else if (x == x->p->left) {
            x->p->left = y;
        } else x->p->right = y;
        y->left = x;
        x->p = y;
    }
    virtual void RightRotate (node_t x) {
        auto y = x->left;
        x->left = y->right;
        if (y->right != this->nil) {
            y->right->p = x;
        }
        y->p = x->p;
        if (x->p == this->nil) {
            this->root = y;
        } else if (x == x->p->right) {
            x->p->right = y;
        } else x->p->left = y;
        y->right = x;
        x->p = y;
    }
    virtual void InsertFixup (node_t z) {
        while (z->p->color == Color::RED) {
            if (z->p == z->p->p->left) {
                auto y = z->p->p->right;
                if (y->color == Color::RED) {
                    z->p->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->p->p->color = Color::RED;
                    z = z->p->p;
                } else {
                    if (z == z->p->right) {
                        z = z->p;
                        this->LeftRotate(z);
                    }
                    z->p->color = Color::BLACK;
                    z->p->p->color = Color::RED;
                    this->RightRotate(z->p->p);
                }
            } else {
                // Symmetric: Left <-> Right
                auto y = z->p->p->left;
                if (y->color == Color::RED) {
                    z->p->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->p->p->color = Color::RED;
                    z = z->p->p;
                } else {
                    if (z == z->p->left) {
                        z = z->p;
                        this->RightRotate(z);
                    }
                    z->p->color = Color::BLACK;
                    z->p->p->color = Color::RED;
                    this->LeftRotate(z->p->p);
                }
            }
        }
        this->root->color = Color::BLACK;
    }
    node_t Insert (T data) {
        auto z = new Node<T>(data);
        auto y = this->nil;
        auto x = this->root;
        while (x != this->nil) {
            y = x;
            if (z->data < x->data) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        z->p = y;
        if (y == this->nil) {
            this->root = z;
        } else if (z->data < y->data) {
            y->left = z;
        } else y->right = z;
        z->left = z->right = this->nil;
        z->color = Color::RED;
        this->InsertFixup(z);
        return z;
    }
    void Transplant (node_t u, node_t v) {
        if (u->p == this->nil) {
            this->root = v;
        } else if (u == u->p->left) {
            u->p->left = v;
        } else u->p->right = v;
        v->p = u->p;
    }

    // We implement `Delete` but don't test it.
    void Delete (node_t z) {
        auto y = z;
        auto y_orig_color = y->color;
        auto x = this->nil;
        if (z->left == this->nil) {
            x = z->right;
            this->Transplant(z, z->right);
        } else if (z->right == this->nil) {
            x = z->left;
            this->Transplant(z, z->left);
        } else {
            y = this->Minimum(z->right);
            y_orig_color = y->color;
            x = y->right;
            if (y->p == z) {
                x->p = y;
            } else {
                this->Transplant(y, y->right);
                y->right = z->right;
                y->right->p = y;
            }
            this->Transplant(z, y);
            y->left = z->left;
            y->left->p = y;
            y->color = z->color;
        }
        if (y_orig_color == Color::BLACK)
            this->DeleteFixup(x);
    }
    void  DeleteFixup (node_t x) {
        while (x != this->root && x->color == Color::BLACK) {
            if (x == x->p->left) {
                auto w = x->p->right;
                if (w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->p->color = Color::RED;
                    this->LeftRotate(x->p);
                    w = x->p->right;
                }
                if (w->left->color == Color::BLACK && w->right->color == Color::BLACK) {
                    w->color = Color::RED;
                    x = x->p;
                }
                else {
                    if (w->right->color == Color::BLACK) {
                        w->left->color = Color::BLACK;
                        w->color = Color::RED;
                        this->RightRotate(w);
                        w = x->p->right;
                    }
                    w->color = x->p->color;
                    x->p->color = Color::BLACK;
                    w->right->color = Color::BLACK;
                    this->LeftRotate(x->p);
                    x = this->root;
                }
            } else {
                // Symetric
                auto w = x->p->left;
                if (w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->p->color = Color::RED;
                    this->RightRotate(x->p);
                    w = x->p->left;
                }
                if (w->right->color == Color::BLACK && w->left->color == Color::BLACK) {
                    w->color = Color::RED;
                    x = x->p;
                }
                else {
                    if (w->left->color == Color::BLACK) {
                        w->right->color = Color::BLACK;
                        w->color = Color::RED;
                        this->LeftRotate(w);
                        w = x->p->left;
                    }
                    w->color = x->p->color;
                    x->p->color = Color::BLACK;
                    w->left->color = Color::BLACK;
                    this->RightRotate(x->p);
                    x = this->root;
                }

            }
        }
        x->color = Color::BLACK;
    }

    node_t Minimum (node_t x) {
        while (x->left != this->nil)
            x = x->left;
        return x;
    }

    node_t Maximum (node_t x) {
        while (x->right != this->nil) {
            x = x->right;
        }
        return x;
    }

    node_t Maximum () {
        return this->Maximum(this->root);
    }

    node_t Minimum () {
        return this->Minimum(this->root);
    }

    void InOrder (vector_t& vec, node_t x) {
        if (x == this->nil) return;
        this->InOrder(vec, x->left);
        vec.push_back(x);
        this->InOrder(vec, x->right);
    }

    void InOrder (vector_t& vec) {
        this->InOrder(vec, this->root);
    }

    node_t Succ (node_t x) {
        if (x->right != this->nil)
            return this->Minimum(x->right);

        auto y = x->p;
        while (y != this->nil && x == y->right) {
            x = y;
            y = y->p;
        }

        return y;
    }

    node_t Pred (node_t x) {
        if (x->left != this->nil)
            return this->Maximum(x->left);

        auto y = x->p;
        while (y != this->nil && x == y->left) {
            x = y;
            y = y->p;
        }

        return y;
    }

    node_t Search (T data) {
        auto x = this->root;
        while (x != this->nil) {
            if (x->data == data) {
                return x;
            } else if (x->data < data) {
                x = x->right;
            } else {
                x = x->left;
            }
        }
        return x;
    }

    void Persist (string filename) {
        // Persist the tree into file, using DOT syntax
        // We firstly traverse the tree and assign each node with a id,
        // Then we traverse in order again and print out the edge information.
        // NIL node is a sentinel.
        vector_t vec;
        this->InOrder(vec);
        unordered_map<node_t, long> map;
        long i = 0;
        for (auto it = vec.begin(); it != vec.end(); ++it, ++i) {
            map[*it] = i;
        }
        map[this->nil] = -1;
        ofstream fout;
        fout.open(filename);

        fout << "digraph T {\n";

        DoPersist(fout, this->root, map);
        fout << "}";


        fout.close();
    }

    void DoPersist (ofstream& f, const node_t node, std::unordered_map<node_t, long>& map) {
        if (map[node] == -1) {
            f << "\t-1 [color=\"black\", label=\"NIL\"];" << endl;
            return;
        }

        string color;

        if (node->color == Color::BLACK) {
            color = "\"black\"";
        } else color = "\"red\"";

        f << '\t' << map[node] << " [color=" << color << ", label=\"" << node->data << "\"];" << endl;


        f << '\t' << map[node] << " -> " << map[node->left] << ";" << endl;
        DoPersist(f, node->left, map);

        f << '\t' << map[node] << " -> " << map[node->right] << ";" << endl;
        DoPersist(f, node->right, map);

    }

};

#endif
