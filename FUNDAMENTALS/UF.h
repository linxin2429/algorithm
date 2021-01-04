//
// Created by 邓鑫林 on 2021/1/3.
//

#ifndef ALGORITHM_UF_H
#define ALGORITHM_UF_H

#include <vector>
#include <iostream>
#include <ctime>
#include <Windows.h>
#include "../utils/utils.h"

using namespace std;

class UF {
private:
    vector<int> id;
    int count;//分量数
    vector<int> size;
public:
    // 以整数标识(0~N-1)初始化N个触点
    UF(int N) : count{N}, id(N), size(N, 1) {
        for (int i = 0; i < count; ++i) {
            id[i] = i;
        }
    }

    // 在p、q之间添加一条连接
    void quickFind_union(int p, int q) {
        int pid = quickFind_find(p);
        int qid = quickFind_find(q);
        if (pid == qid)
            return;

        for (int i = 0; i < id.size(); ++i)
            if (id[i] == qid)
                id[i] = pid;

        count--;
    }

    //p所在的分量的标识符
    int quickFind_find(int p) const {
        return id[p];
    }

    void quickUnion_union(int p, int q) {
        int pRoot = quickUnion_find(p);
        int qRoot = quickUnion_find(q);
        if (pRoot == qRoot)
            return;
        id[qRoot] = pRoot;
        count--;
    }

    int weighted_find(int p) const {
        while (p != id[p])
            p = id[p];
        return p;
    }

    // 把小的接到大的上去
    void weighted_union(int p, int q) {
        int pRoot = weighted_find(p);
        int qRoot = weighted_find(q);
        if (pRoot == qRoot)
            return;
        if (size[pRoot] > size[qRoot]) {
            id[qRoot] = pRoot;
            size[pRoot] += size[qRoot];
        } else {
            id[pRoot] = qRoot;
            size[qRoot] += size[pRoot];
        }
        count--;
    }

    int quickUnion_find(int p) const {
        while (p != id[p])
            p = id[p];
        return p;
    }

    // 如果pq存在同一个分量中则返回true
    bool quickFind_connected(int p, int q) {
        return quickFind_find(p) == quickFind_find(q);
    }

    bool quickUnion_connected(int p, int q) {
        return quickUnion_find(p) == quickUnion_find(q);
    }

    bool weighted_connected(int p, int q) {
        return weighted_find(p) == weighted_find(q);
    }

    void clear() {
        for (int i = 0; i < id.size(); ++i) {
            id[i] = i;
            size[i] = 1;
        }
        count = id.size();
    }

    // 连接分量的数量
    int getCount() const {
        return count;
    }

    static const vector<pair<int, int>> readUF(ifstream &in, int &n) {
        stringstream ss;
        ss << in.rdbuf();
        ss >> n;
        vector<pair<int, int>> unionVec;
        int p, q;
        while (ss >> p && ss >> q) {
            unionVec.emplace_back(p, q);
        }
        return unionVec;

    }

    static void test(const string &s) {
        // time_t start, end;
        ifstream fin(s);
        int n;
        auto vec = readUF(fin, n);
        UF uf(n);

        // start = clock();
        Stopwatch stopwatch;
        for (auto &p:vec) {
            if (uf.quickFind_connected(p.first, p.second))
                continue;
            uf.quickFind_union(p.first, p.second);
        }
        // end = clock();
        //cout << "Elapsed time quick_find: " << end - start<<"  " << double(end - start) / CLOCKS_PER_SEC << endl;

        stopwatch.elapsedTime();

        cout << "coutNum   " << uf.getCount() << endl;
        uf.clear();

        stopwatch.restart();
        for (auto &p:vec) {

            if (uf.quickUnion_connected(p.first, p.second))
                continue;
            uf.quickUnion_union(p.first, p.second);
        }
        stopwatch.elapsedTime();
        cout << "coutNum   " << uf.getCount() << endl;
        uf.clear();


        stopwatch.restart();
        for (auto &p:vec) {

            if (uf.weighted_connected(p.first, p.second))
                continue;
            uf.weighted_union(p.first, p.second);
            //cout << p.first<<"++"<<p.second<<endl;
        }
        stopwatch.elapsedTime();
        cout << "coutNum   " << uf.getCount() << endl;
        uf.clear();
    }


    static void test() {
        cout << ">>tinyUF.txt<<" << endl;
        test("../FILE/tinyUF.txt");
        cout << ">>mediumUF.txt<<" << endl;
        test("../FILE/mediumUF.txt");
        //cout << ">>largeUF.txt<<" << endl;
        //test("../FILE/largeUF.txt");
    }

};

#endif //ALGORITHM_UF_H
