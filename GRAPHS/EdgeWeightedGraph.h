//
// Created by 邓鑫林 on 2021/1/11.
//

#ifndef ALGORITHM_EDGEWEIGHTEDGRAPH_H
#define ALGORITHM_EDGEWEIGHTEDGRAPH_H

#include <string>
#include <iomanip>
#include <vector>
#include <iostream>
#include "../SORTING/PQ.h"
#include <climits>

using namespace std;

class Edge {
    int v_, w_;
    double weight_;
public:
    explicit Edge(int v = 0, int w = 0, double weight = 0.0) : v_(v), w_(w), weight_(weight) {}

    double weight() const { return weight_; }

    int either() const { return v_; }

    int other(int vertex) const {
        return vertex == v_ ? w_ : v_;
    }

    bool operator<(const Edge &e) const { return weight_ < e.weight_; }

    bool operator==(const Edge &e) const { return weight_ == e.weight_; }

    bool operator>(const Edge &e) const { return weight_ > e.weight_; }

    bool operator!=(const Edge &e) const { return weight_ != e.weight_; }

    bool operator>=(const Edge &e) const { return weight_ >= e.weight_; }

    bool operator<=(const Edge &e) const { return weight_ <= e.weight_; }

    string toString() const {
        ostringstream ret;
        ret << v_ << "-" << w_ << " " << setprecision(3) << setiosflags(ios::fixed) << weight_;
        return ret.str();
    }
};

class EdgeWeightedGraph {
    int V_;
    int E_;
    vector<vector<Edge>> adj;
public:
    explicit EdgeWeightedGraph(int V) : V_(V), E_(0), adj(V) {}

    explicit EdgeWeightedGraph(istream &in) : E_(0) {
        in >> V_;
        adj = vector<vector<Edge>>(V_);
        int e;
        int x, y;
        double w;
        in >> e;
        for (int i = 0; i < e; ++i) {
            in >> x >> y >> w;
            addEdge(Edge(x, y, w));
        }
    }

    void addEdge(const Edge &edge) {
        int x = edge.either();
        int y = edge.other(x);
        adj[x].push_back(edge);
        adj[y].push_back(edge);
        E_++;
    }

    int V() const { return V_; }

    int E() const { return E_; }

    vector<Edge> edges() const {
        vector<Edge> ret;
        for (int v = 0; v < V_; v++)
            for (auto e : adj[v])
                ret.push_back(e);
        return ret;
    }

    using Iterator = vector<Edge>::iterator;
    using CIterator = vector<Edge>::const_iterator;

    Iterator begin(int v) { return adj[v].begin(); }

    CIterator begin(int v) const { return adj[v].cbegin(); }

    Iterator end(int v) { return adj[v].end(); }

    CIterator end(int v) const { return adj[v].cend(); }

    string toString() const {
        ostringstream ret;
        ret << V_ << " vertices, " << E_ << "edges\n";
        for (int v = 0; v < V_; ++v) {
            ret << v << ": ";
            for (auto &edge : adj[v])
                ret << edge.toString() << " ";
            ret << '\n';
        }
        return ret.str();
    }
};


class LazyPrimMST {
    vector<bool> mark;
    vector<Edge> mst;
    priority_queue<Edge, vector<Edge>, greater<Edge>> pq;
    double weight_;

    void visit(const EdgeWeightedGraph &G, int v) {
        mark[v] = true;
        for (auto itr = G.begin(v); itr != G.end(v); itr++)
            if (!mark[itr->other(v)])
                pq.push(*itr);
    }

public:
    explicit LazyPrimMST(const EdgeWeightedGraph &G) : mark(G.V(), false), weight_(0) {
        visit(G, 0);
        while (!pq.empty()) {
            Edge e = pq.top();
            pq.pop();
            int v = e.either(), w = e.other(v);
            if (mark[v] && mark[w])
                continue;
            mst.push_back(e);
            weight_ += e.weight();
            if (!mark[v])
                visit(G, v);
            if (!mark[w])
                visit(G, w);
        }
    }

    vector<Edge> edges() const {
        return mst;
    }

    double weight() const { return weight_; }
};

class PrimMST {
    vector<Edge> edgeTo; // 距离树最近的边
    vector<double> distTo;
    vector<bool> mark;
    IndexPQ<double, greater<double>> pq;

    void visit(const EdgeWeightedGraph &G, int v) {
        mark[v] = true;
        for (auto itr = G.begin(v); itr != G.end(v); itr++) {
            int w = itr->other(v);
            if (mark[w]) continue;
            if (itr->weight() < distTo[w]) {
                edgeTo[w] = *itr;
                distTo[w] = itr->weight();
                if (pq.contains(w))
                    pq.changeKey(w, distTo[w]);
                else
                    pq.push(w, distTo[w]);
            }
        }
    }

public:
    explicit PrimMST(const EdgeWeightedGraph &G) :
            edgeTo(G.V()), distTo(G.V(), INT_MAX), mark(G.V(), false), pq(G.V()) {
        distTo[0] = 0.0;
        pq.push(0, 0.0);
        while (!pq.isEmpty())
            visit(G, pq.pop());
    }

    double weight() {
        double ret = 0;
        for (auto e : edgeTo)
            ret += e.weight();
        return ret;
    }

    vector<Edge> edges() { return edgeTo; }
};

class KruskalMST {
private:
    vector<Edge> mst;
public:
    explicit KruskalMST(const EdgeWeightedGraph &G) {
        vector<Edge> edges = G.edges();
        priority_queue<Edge, vector<Edge>, greater<Edge>> pq(edges.begin(),edges.end());
        UF uf(G.V());
        while (!pq.empty() && mst.size() < G.V() - 1){
            Edge e = pq.top();
            pq.pop();
            int v = e.either(),w = e.other(v);
            if (uf.weighted_connected(v,w))
                continue;
            uf.weighted_union(v,w);
            mst.push_back(e);
        }
    }
    vector<Edge> edges()const{return mst;}
    double weight()
    {
        double ret = 0;
        for (auto e : mst)
            ret += e.weight();
        return ret;
    }
};

class MST {
private:
    PrimMST primMst;
public:
    explicit MST(const EdgeWeightedGraph &G) : primMst(G) {}

    vector<Edge> edges() { return primMst.edges(); }

    double weight() { return primMst.weight(); }
};

void testEWG(const string &str) {
    ifstream in(str);
    EdgeWeightedGraph G(in);
    MST mst(G);
    for (auto &e : mst.edges())
        cout << e.toString() << "\n";
    cout << mst.weight() << endl;

}

void testEWG() {
    testEWG("../FILE/tinyEWG.txt");
}

#endif //ALGORITHM_EDGEWEIGHTEDGRAPH_H
