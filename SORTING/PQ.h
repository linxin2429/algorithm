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

template<typename T, typename Compare = std::less<T>>
class IndexPQ
{
private:
    int N = 0;
    std::vector<int> pq; //索引二叉堆
    std::vector<int> qp; //逆序：qp[pq[i]]=pq[qp[i]]=i
    std::vector<T> keys;
    //备注：qp[indx]是位置。keys[qp[index]]=key, pq[qp[index]]=index
    Compare comp;
public:
    explicit IndexPQ(int maxN)
            :pq(maxN + 1), qp(maxN + 1), keys(maxN + 1)
    {
        for (int & i : qp)
            i = -1;
    }
    void push(int i, T k)
    {
        if (contains(i))
            return;
        ++N;
        qp[i] = N;
        keys[i] = k;
        pq[N] = i;
        swim(N);
    }
    int indexOfTop()
    {
        return pq[1];
    }
    T top()
    {
        return keys[pq[1]];
    }

    int pop()
    {//删除key值最小的项，返回其对应的索引
        int min = pq[1];
        exch(1, N--);
        sink(1);

        qp[min] = -1;
        pq[N + 1] = -1;
        return min;
    }

    T keyOf(int i)
    {//返回索引所对应的键值
        _ASSERT(contains(i));
        return keys[i];
    }

    void changeKey(int i, T k)
    {
        if (compare(k, keys[i]))
        {
            keys[i] = k;
            sink(qp[i]);
        }
        else
        {
            keys[i] = k;
            swim(qp[i]);
        }
    }

    bool contains(int i) { return qp[i] != -1; }
    void deleteT(int i)
    {
        _ASSERT(contains(i));
        int loc = qp[i];
        exch(loc, N--);
        swim(loc);
        swim(loc);
        qp[i] = -1;
    }

    bool isEmpty() { return N == 0; }
    int size() { return N; }

private:
    bool compare(int i, int j)
    {
        return comp(keys[pq[i]], keys[pq[j]]);
    }

    void exch(int i, int j)
    {
        //两个位置的index互换
        std::swap(pq[i], pq[j]);
        std::swap(qp[pq[i]], qp[pq[j]]);
    }

    void swim(int k)
    {
        while (k > 1 && compare(k >> 1, k))
        {
            exch(k, k >> 1);
            k >>= 1;
        }
    }

    void sink(int k)
    {
        while ((k << 1) <= N)
        {
            int j = k << 1;
            if (j < N && compare(j, j + 1)) ++j;
            if (!compare(k, j))break;
            exch(k, j);
            k = j;
        }
    }
};

#endif //ALGORITHM_PQ_H
