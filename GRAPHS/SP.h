//
// Created by 邓鑫林 on 2021/1/11.
//

#ifndef ALGORITHM_SP_H
#define ALGORITHM_SP_H


class DirectedEdge {
    int v_, w_;
    double weight_;
public:
    explicit DirectedEdge(int v = 0, int w = 0, double weight = 0.0) : v_(v), w_(w), weight_(weight) {}

    double weight() const { return weight_; }

    int from() const { return v_; }

    int to() const { return w_; }

    bool operator<(const DirectedEdge &e) const { return weight_ < e.weight_; }

    bool operator==(const DirectedEdge &e) const { return weight_ == e.weight_; }

    bool operator>(const DirectedEdge &e) const { return weight_ > e.weight_; }

    bool operator!=(const DirectedEdge &e) const { return weight_ != e.weight_; }

    bool operator>=(const DirectedEdge &e) const { return weight_ >= e.weight_; }

    bool operator<=(const DirectedEdge &e) const { return weight_ <= e.weight_; }

    string toString() const {
        ostringstream out;
        out << v_ << " -> " << w_ << "   "
            << setprecision(3) << setiosflags(ios::fixed)
            << weight_ << endl;
        return out.str();
    }
};

class EdgeWeightedDigraph {
    int V_;
    int E_;
    vector<vector<DirectedEdge>> adj_;
public:
    explicit EdgeWeightedDigraph(int V) : V_(V), E_(0), adj_(V) {}

    explicit EdgeWeightedDigraph(istream &in) : E_(0) {
        in >> V_;
        adj_ = vector<vector<DirectedEdge>>(V_);
        int e;
        int x, y;
        double w;
        in >> e;
        for (int i = 0; i < e; ++i) {
            in >> x >> y >> w;
            addEdge(DirectedEdge(x, y, w));
        }
    }

    void addEdge(const DirectedEdge &edge) {
        adj_[edge.from()].push_back(edge);
        E_++;
    }

    int V() const { return V_; }

    int E() const { return E_; }

    vector<DirectedEdge> edges() const {
        vector<DirectedEdge> ret;
        for (int v = 0; v < V_; v++)
            for (auto e : adj_[v])
                ret.push_back(e);
        return ret;
    }

    vector<DirectedEdge> adj(int v) const {
        return adj_[v];
    }

    string toString() const {
        ostringstream ret;
        ret << V_ << " vertices, " << E_ << "edges\n";
        for (int v = 0; v < V_; ++v) {
            ret << v << ": ";
            for (auto &edge : adj_[v])
                ret << edge.toString() << " ";
            ret << '\n';
        }
        return ret.str();
    }
};

class DijkstraSP {
    vector<DirectedEdge> edgeTo_;
    vector<double> distTo_;
    IndexPQ<double, greater<double>> pq;

    void relax(const EdgeWeightedDigraph &G, int v) {
        for (auto &e : G.adj(v)) {
            int w = e.to();
            if (distTo_[w] > distTo_[v] + e.weight()) {
                distTo_[w] = distTo_[v] + e.weight();
                edgeTo_[w] = e;
                if (pq.contains(w)) pq.changeKey(w, distTo_[w]);
                else pq.push(w, distTo_[w]);
            }
        }
    }

public:
    DijkstraSP(const EdgeWeightedDigraph &G, int s) : edgeTo_(G.V()), distTo_(G.V(), INT_MAX), pq(G.V()) {
        distTo_[s] = 0.0;
        pq.push(s, 0.0);
        while (!pq.isEmpty())
            relax(G, pq.pop());
    }

    double distTo(int v) const { return distTo_[v]; }

    bool hasPathTo(int v) const { return distTo_[v] < INT_MAX; }

    vector<DirectedEdge> pathTo(int v) const {
        if (!hasPathTo(v))
            return vector<DirectedEdge>();
        stack<DirectedEdge> stack;
        for (DirectedEdge e = edgeTo_[v]; e != DirectedEdge(); e = edgeTo_[e.from()]) {
            stack.push(e);
        }
        vector<DirectedEdge> ret;
        while (!stack.empty()) {
            ret.push_back(stack.top());
            stack.pop();
        }
        return ret;
    }
};

class DijkstraAllPairSP {
    vector<DijkstraSP> all;
public:
    explicit DijkstraAllPairSP(const EdgeWeightedDigraph &G) {
        for (int i = 0; i < G.V(); ++i) {
            all.emplace_back(G, i);
        }
    }

    vector<DirectedEdge> path(int s, int v) const {
        return all[s].pathTo(v);
    }

    double dist(int s, int v) const {
        return all[s].distTo(v);
    }

};

void testSP(const string &str) {
    ifstream in(str);
    EdgeWeightedDigraph G(in);

    DijkstraAllPairSP sp(G);
    for (int i = 0; i < G.V(); ++i) {
        for (int j = 0; j < G.V(); ++j) {
            cout << i << " to " << j << "\n";
            for (auto &path :sp.path(i, j))
                cout << path.toString() ;
            cout << "\ndist: " << sp.dist(i, j) << "\n"<<endl;
        }
    }
}

void testSP() {
    testSP("../FILE/tinyEWD.txt");
}

#endif //ALGORITHM_SP_H
