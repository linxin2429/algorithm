//
// Created by 邓鑫林 on 2021/1/6.
//

#ifndef ALGORITHM_PQ_H
#define ALGORITHM_PQ_H

#include <vector>
#include <functional>
#include <algorithm>

template<typename Type, typename Compare = std::less<Type>>
class PQ {
private:
    vector<Type> aux;
    Compare compare;

    void makeHeap() {
        for (int i = (aux.size() - 2) >> 1; i >= 0; --i) {
            sink(i);
        }
    }

    void swim(int k) {
        auto key = std::move(aux[k]);
        while (k > 0 && compare(aux[(k - 1) >> 1], key)) {
            aux[k] = std::move(aux[(k - 1) >> 1]);
            k = (k - 1) >> 1;
        }
        aux[k] = std::move(key);
    }

    void sink(int k) {
        auto key = std::move(aux[k]);
        int N = aux.size();
        while ( (k << 1) + 1 < N) {
            int i = (k<<1)+1;
            if (i < N - 1 && compare(aux[i], aux[i + 1]))
                ++i;
            if (!compare(key, aux[i]))
                break;
            aux[k] = std::move(aux[i]);
            k = i;
        }
        aux[k] = std::move(key);
    }

public:
    // 父：i, 子：2i+1 2i+2
    // 子：i, 父：(i-1)/2
    explicit PQ(const Compare &x = Compare()) : compare(x) {}

    template<class Itr>
    PQ(Itr begin, Itr end, const Compare &x = Compare()):aux(begin, end), compare(x) {
        makeHeap();
    }

    void push(Type &x) {
        aux.push_back(x);
        swim(aux.size() - 1);
    }

    Type pop() {
        Type retVal = std::move(aux[0]);
        aux[0] = std::move(aux[aux.size() - 1]);
        aux.pop_back();
        if (!aux.empty())
            sink(0);
        return retVal;
    }

    Type top(){
        return aux[0];
    }

    bool isEmpty(){
        return aux.empty();
    }
    int size(){
        return aux.size();
    }
};



#endif //ALGORITHM_PQ_H
