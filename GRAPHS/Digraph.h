//
// Created by 邓鑫林 on 2021/1/10.
//

#ifndef ALGORITHM_DIGRAPH_H
#define ALGORITHM_DIGRAPH_H

#include <vector>
#include <list>
#include <iostream>
#include <stack>
#include <string>
#include <queue>

using namespace std;

class Digraph {
    int V;  // 顶点
    int E; // 边数
    vector<list<int>> adj; // 邻接表

public:
    explicit Digraph(int v = 0) : V(v), E(0), adj(v) {}

    explicit Digraph(istream &in) : E(0) {
        in >> V;
        adj = vector<list<int>>(V);
        int e;
        in >> e;
        int u, v;
        for (int i = 0; i < e; ++i) {
            in >> u >> v;
            addEdge(u, v);
        }
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        ++E;
    }

    int getV() const { return V; }

    int getE() const { return E; }

    using Iterator = list<int>::iterator;
    using CIterator = list<int>::const_iterator;

    Iterator begin(int v) { return adj[v].begin(); }

    CIterator begin(int v) const { return adj[v].cbegin(); }

    Iterator end(int v) { return adj[v].end(); }

    CIterator end(int v) const { return adj[v].cend(); }

    Digraph reverse() {
        Digraph digraph(V);
        for (int i = 0; i < V; ++i) {
            for (auto itr = begin(i); itr != end(i); itr++)
                digraph.addEdge(*itr, i);
        }
        return digraph;
    }

    string toString() {
        std::string ret = std::to_string(V) + " vertices, " + std::to_string(E) + "edges\n";
        for (int v = 0; v < V; ++v) {
            ret += std::to_string(v) + ": ";
            for (auto w : adj[v])
                ret += std::to_string(w) + " ";
            ret += '\n';
        }
        return ret;
    }

};

class DirectedDFS {
private:
    vector<bool> mark;

    void dfs(const Digraph &G, int v) {
        mark[v] = true;
        for (auto itr = G.begin(v); itr != G.end(v); itr++)
            if (!mark[*itr])
                dfs(G, *itr);
    }

public:
    DirectedDFS(const Digraph &G, int s) : mark(G.getV(), false) {
        dfs(G, s);
    }

    DirectedDFS(const Digraph &G, const vector<int> &sources) : mark(G.getV(), false) {
        for (auto s:sources)
            if (!mark[s])
                dfs(G, s);
    }

    bool marked(int v) {
        return mark[v];
    }
};

class DirectedCycle {
private:
    vector<bool> mark;
    vector<int> edgeTo;
    stack<int> cycle;
    vector<bool> onStack;

    void dfs(const Digraph &G, int v) {
        onStack[v] = true;
        mark[v] = true;
        for (auto itr = G.begin(v); itr != G.end(v); itr++)
            if (hasCycle())return;
            else if (!mark[*itr]) {
                edgeTo[*itr] = v;
                dfs(G, *itr);
            } else if (onStack[*itr]) {
                for (int x = v; x != *itr; x = edgeTo[x])
                    cycle.push(x);
                cycle.push(*itr);
                cycle.push(v);
            }
        onStack[v] = false;
    }

public:
    explicit DirectedCycle(const Digraph &G) :
            mark(G.getV(), false), edgeTo(G.getV()), onStack(G.getV(), false) {
        for (int i = 0; i < G.getV(); ++i)
            if (!mark[i])
                dfs(G, i);
    }

    bool hasCycle() { return !cycle.empty(); }

    vector<int> Cycle() {
        vector<int> ret;
        while (!cycle.empty()) {
            ret.push_back(cycle.top());
            cycle.pop();
        }
        return ret;
    }
};

class DepthFirstOrder {
private:
    vector<bool> mark;
    vector<int> Pre; //前序
    vector<int> Post; // 后序
    stack<int> ReversePost; // 逆后序
    void dfs(const Digraph &G, int v) {
        Pre.push_back(v);
        mark[v] = true;
        for (auto itr = G.begin(v); itr != G.end(v); itr++)
            if (!mark[*itr])
                dfs(G, *itr);
        Post.push_back(v);
        ReversePost.push(v);
    }

public:
    explicit DepthFirstOrder(const Digraph &G) : mark(G.getV(), false) {
        for (int v = 0; v < G.getV(); ++v) {
            if (!mark[v])
                dfs(G, v);
        }
    }

    vector<int> pre() { return Pre; }

    vector<int> post() { return Post; }

    vector<int> reversePost() {
        vector<int> ret;
        stack<int> tmp = ReversePost;
        while (!tmp.empty()){
            ret.push_back(tmp.top());
            tmp.pop();
        }
        return ret;
    }

};

class Topological {
    vector<int> Order;
    explicit Topological(const Digraph &G) {
        DirectedCycle cyclefinder(G);
        if (!cyclefinder.hasCycle()){
            DepthFirstOrder dfs(G);
            Order = dfs.reversePost();
        }
    }

    bool isDAG() {//有向无环图
        return !Order.empty();
    }

    vector<int> order() {
        return Order;
    }
};
class SCC{
    vector<bool>mark;
    vector<int>id;
    int count{};
    void dfs(const Digraph&G){

    }
public:
    SCC(const Digraph&G):mark(G.getV(),false),id(G.getV()){

    }
};

#endif //ALGORITHM_DIGRAPH_H
