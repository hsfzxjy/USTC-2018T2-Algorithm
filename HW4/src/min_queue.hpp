#ifndef _MIN_QUEUE_H_
#define _MIN_QUEUE_H_

#include <functional>
#include <unordered_map>


template<typename T>
class min_queue {
public:
    typedef T value_t;
    typedef std::function<value_t(size_t)> init_functor_t;
private:
    size_t size;
    size_t capacity;
    value_t *base;
    unordered_map<typename min_queue<T>::value_t::hash_value_t, size_t> reversed_mapping;

    inline size_t left (size_t i) {
        return ((i + 1) << 1) - 1;
    }

    inline size_t right (size_t i) {
        return (i + 1) << 1;
    }

    inline size_t parent (size_t i) {
        return (i - 1) >> 1;
    }

    inline void swap (size_t i, size_t j) {
        reversed_mapping[base[i].hash_value()] = j;
        reversed_mapping[base[j].hash_value()] = i;
        auto tmp = this->base[i];
        this->base[i] = this->base[j];
        this->base[j] = tmp;
    }

    void heapify (size_t i) {
        auto l = left(i);
        auto r = right(i);
        size_t smallest;

        if (l < size && base[l] < base[i]) {
            smallest = l;
        } else smallest = i;
        if (r < size && base[r] < base[smallest]) {
            smallest = r;
        }
        if (smallest != i) {
            swap(i, smallest);
            heapify(smallest);
        }
    }
public:

    inline size_t empty() {
        return size == 0;
    }

    min_queue (size_t capacity, init_functor_t f) {
        this->base = new value_t[capacity];
        this->capacity = this->size = capacity;
        for (size_t i = 0; i < capacity; ++i) {
            auto value = this->base[i] = f(i);
            this->reversed_mapping[value.hash_value()] = i;
        }

        for (int i = (capacity >> 1) - 1; i >= 0; --i) heapify(i);
    }

    ~min_queue () {
        delete[] this->base;
    }

    inline value_t extractMin () {
        auto min = base[0];
        base[0] = base[size - 1];
        --size;
        heapify(0);
        return min;
    }

    inline void decreaseKey (typename min_queue<T>::value_t::hash_value_t h) {
        size_t i = reversed_mapping[h];
        while (i > 0 && base[parent(i)] > base[i]) {
            swap(i, parent(i));
            i = parent(i);
        }
    }


};

#endif
