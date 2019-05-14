#ifndef OSTREE_H_
#define OSTREE_H_

#include "RBTree.h"
#include <ostream>

// The internal data type of OS Tree
template <typename T>
struct OSData {
    T value;
    size_t size;

    OSData (T v): value(v), size(0) {};
    OSData (): size(0) {};

    // Override comparison operators for integrity
    bool operator<(const OSData<T>& rhs) {
        return value < rhs.value;
    }

    bool operator<=(const OSData<T>& rhs) {
        return value <= rhs.value;
    }

    bool operator>(const OSData<T>& rhs) {
        return value > rhs.value;
    }

    bool operator>=(const OSData<T>& rhs) {
        return value >= rhs.value;
    }

    bool operator==(const OSData<T>& rhs) {
        return value == rhs.value;
    }

    bool operator!=(const OSData<T>& rhs) {
        return value != rhs.value;
    }

    // For streaming output
    friend std::ostream& operator<<(std::ostream& os, const OSData<T>& obj) {
        os << "v: " << obj.value << "\\ns: " << obj.size;
    }

};

template <typename T>
class OSTree : public RBTree<OSData<T> > {
private:
    typedef RBTree<OSData<T> > base_t;
public:
    typedef OSData<T> data_t;
    typedef T value_t;
    typedef typename RBTree<OSData<T> >::node_t node_t;

    void InsertFixup (node_t z) override {
        auto z_ = z;

        while (z_ != this->nil) {
            z_->data.size++;
            z_ = z_->p;
        }

        base_t::InsertFixup(z);
    }

    // We don't use super directly, since we need to access intermediate variables
    void LeftRotate (node_t x) override {
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
        // above is the same as base_t::LeftRotate

        y->data.size = x->data.size;
        x->data.size = x->left->data.size + x->right->data.size + 1;
    }

    void RightRotate (node_t x) override {
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
        // above is the same as base_t::RightRotate

        y->data.size = x->data.size;
        x->data.size = x->left->data.size + x->right->data.size + 1;
    }

    node_t Select (node_t x, size_t i) {
        auto r = x->left->data.size + 1;
        if (i == r) {
            return x;
        } else if (i < r) {
            return this->Select(x->left, i);
        } else {
            return this->Select(x->right, i - r);
        }

    }

    node_t Select (size_t i) {
        return this->Select(this->root, i);
    }

    size_t Rank (node_t x) {
        auto r = x->left->data.size + 1;
        auto y = x;
        while (y != this->root) {

            if (x == this->nil)
                return 0;

            if (y == y->p->right)
                r += y->p->left->data.size + 1;
            y = y->p;
        }
        return r;
    }
};

#endif
