//
// Created by 邓鑫林 on 2021/1/8.
//

#ifndef ALGORITHM_HASHST_H
#define ALGORITHM_HASHST_H

#include <vector>
#include <utility>
#include <forward_list>
#include "../utils/utils.h"
#include <functional>

using namespace std;

template<typename Key, typename Value>
class SequentialSearchST {
private:
    forward_list<pair<Key, Value>> list;
    using Iterator = typename forward_list<pair<Key, Value>>::iterator;
    int N;
public:
    Value get(const Key &key) {
        auto prev = list.before_begin();
        for (auto itr = list.begin(); itr != list.end(); itr++, prev++)
            if (itr->first == key) {
                auto ret = *itr;
                list.erase_after(prev);
                list.push_front(ret);
                return ret.second;
            }
        return Value();
    }

    void put(const Key &key, const Value &value) {
        for (auto itr = list.begin(); itr != list.end(); itr++)
            if (itr->first == key) {
                itr->second = value;
                return;
            }
        list.push_front(make_pair(key, value));
        ++N;
    }

    void deleteKey(const Key &key) {
        auto prev = list.before_begin();
        for (auto itr = list.begin(); itr != list.end(); itr++, ++prev)
            if (itr->first == key) {
                list.erase_after(prev);
                --N;
                return;
            }
    }

    Iterator begin() {
        return list.begin();
    }

    Iterator end() {
        return list.end();
    }

    int size() {
        return N;
    }

    bool isEmpty() {
        return N == 0;
    }

    bool contains(const Key &key) {
        return nullptr != get(key);
    }

};


template<typename Key, typename Value, typename HashFunc = std::hash<Key>>
class SequentialChainingHashST {
private:
//    MyPrimer primer;
    int N;//键值对数
    int M;//散列表大小
    vector<SequentialSearchST<Key, Value>> hashTable;
    static const int coff = 10; // N/M均值

    int hash(const Key &key) {
        return (HashFunc{}(key) & 0x7fffffff) % M;
    }

    void reHash(int capacity) {
        auto temp = toArray();
        hashTable.clear();
        hashTable.resize(capacity);
        M = capacity;
        N = 0;
        for (auto &item : temp) {
            put(item.first, item.second);
        }
    }

public:
    explicit SequentialChainingHashST(int capacity = 997) :
            M(capacity), N{0}, hashTable(M, SequentialSearchST<Key, Value>()) {}


    Value get(const Key &key) {
        return hashTable[hash(key)].get(key);
    }

    void put(const Key &key, const Value &value) {
        if (N == M * coff)
            reHash(2 * M - 1);
        int i = hash(key);
        int old = hashTable[i].size();
        hashTable[i].put(key, value);
        if (old != hashTable[i].size())
            ++N;
    }

    void deleteKey(const Key &key) {
        int i = hash(key);
        int old = hashTable[i].size();
        hashTable[i].deleteKey(key);
        if (old != hashTable[i].size())
            --N;
    }

    int size() { return N; }

    bool isEmpty() { return N == 0; }

    bool contains(const Key &key) { return get(key) != Value(); }

    std::vector<pair<Key, Value>> toArray() {
        std::vector<pair<Key, Value>> ret;

        for (int i = 0; i < M; ++i)
            for (auto &item : hashTable[i])
                ret.push_back(item);

        return ret;
    }
};

template<typename Key, typename Value, typename Func = std::hash<Key>>
class LinearProbingHashST {
private:
    int N;
    int M;
    vector<pair<Key, Value>> st;

    void resize(int cap) {
        LinearProbingHashST<Key, Value> t(cap);
        for (int i = 0; i < M; ++i) {
            if (st[i].first != Key())
                t.put(st[i].first, st[i].second);
        }
        M = t.M;
        st = std::move(t.st);
    }

public:
    LinearProbingHashST(int capacity = 16) : N(0), M(capacity), st(M) {}

    int hash(const Key &key) {
        return (Func{}(key) & 0x7fffffff) % M;
    }

    void put(const Key &key, const Value &value) {
        if (N >= M / 2)resize(2 * M);
        int i;
        for (i = hash(key); st[i].first != Key(); i = (i + 1) % M)
            if (st[i].first == key) {
                st[i].second = value;
                return;
            }
        st[i] = make_pair(key, value);
        N++;
    }

    Value get(const Key &key) {
        for (int i = hash(key); st[i].first != Key(); i = (i + 1) % M)
            if (st[i].first == key)
                return st[i].second;
        return Value();
    }

    bool isEmpty() {
        return N == 0;
    }

    int size(){return N;}

    bool contains(const Key &key) {
        return get(key) != Value();
    }

    void deleteKey(const Key &key) {
        if (!contains(key))
            return;
        int i = hash(key);
        while (key != st[i].first)
            i = (i + 1) % M;
        st[i] = make_pair(Key(), Value());
        i = (i + 1) % M;
        while (st[i].first != Key()) {
            pair<Key, Value> tmp = st[i];
            st[i] = {Key(), Value()};
            N--;
            put(tmp.first, tmp.second);
            i = (i + 1) % M;
        }
        if (N > 0 && N == M / 8)resize(M / 2);
    }

    vector<pair<Key,Value>> toArray(){
        vector<pair<Key,Value>> array;
        for (int i = 0; i < M; ++i) {
            if (st[i].first != Key() )
                array.push_back(st[i]);
        }
        return array;
    }

};

#include "../FUNDAMENTALS/random.h"


void testHash(int N) {
    Random random;
    //SequentialChainingHashST<int, double> st;
    LinearProbingHashST<int,double> st;
    vector<double> ran(N+1);
    for (int i = 1; i <= N; ++i) {
        ran[i] = random.uniform();
    }
    for (int i = 1; i <= N; ++i) {
        st.put(i, ran[i]);
    }
    cout << st.size() << endl;
    for (int i = 0; i < N; ++i) {
        int j = random.uniformInt(1,N );
        if (!st.contains(j)) {
            cout << j << "error" << endl;
            return;
        } else if (ran[j] != st.get(j)) {
            cout << "error" << endl;
            return;
        }
    }
}

#endif //ALGORITHM_HASHST_H
