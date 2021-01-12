//
// Created by 邓鑫林 on 2021/1/11.
//

#ifndef ALGORITHM_SP_H
#define ALGORITHM_SP_H

#include <iomanip>

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

class EdgeWeightedDirectedCycle {
private:
    vector<bool> mark_;
    vector<DirectedEdge> edgeTo_;
    vector<bool> onStack_;
    stack<DirectedEdge> cycle_;    // directed cycle (or null if no such cycle)

public:

    explicit EdgeWeightedDirectedCycle(const EdgeWeightedDigraph &G) :
            mark_(G.V(), false), edgeTo_(G.V()), onStack_(G.V()) {
        cycle_ = stack<DirectedEdge>();
        for (int v = 0; v < G.V(); v++)
            if (!mark_[v]) dfs(G, v);
    }

private:

    void dfs(const EdgeWeightedDigraph &G, int v) {
        onStack_[v] = true;
        mark_[v] = true;
        for (DirectedEdge &e : G.adj(v)) {
            int w = e.to();
            // short circuit if directed cycle found
            if (!cycle_.empty()) return;
                // found new vertex, so recur
            else if (!mark_[w]) {
                edgeTo_[w] = e;
                dfs(G, w);
            }
                // trace back directed cycle
            else if (onStack_[w]) {
                DirectedEdge f = e;
                while (f.from() != w) {
                    cycle_.push(f);
                    f = edgeTo_[f.from()];
                }
                cycle_.push(f);
                return;
            }
        }
        onStack_[v] = false;
    }

public:

    bool hasCycle() { return !cycle_.empty(); }

    vector<DirectedEdge> cycle() {
        vector<DirectedEdge> ret;
        stack<DirectedEdge> stack = cycle_;
        while (!stack.empty()) {
            ret.push_back(stack.top());
            stack.pop();
        }
        return ret;
    }
};

class EWDDepthFirstOrder { // 规避头文件交叉引用，重写一遍
private:
    vector<bool> mark;
    vector<int> Pre; //前序
    vector<int> Post; // 后序
    stack<int> ReversePost; // 逆后序

    void dfs(const EdgeWeightedDigraph &G, int v) {
        Pre.push_back(v);
        mark[v] = true;
        for (auto &e : G.adj(v))
            if (!mark[e.to()])
                dfs(G, e.to());
        Post.push_back(v);
        ReversePost.push(v);
    }

public:

    explicit EWDDepthFirstOrder(const EdgeWeightedDigraph &G) : mark(G.V(), false) {
        for (int v = 0; v < G.V(); ++v) {
            if (!mark[v])
                dfs(G, v);
        }
    }

    vector<int> pre() { return Pre; }

    vector<int> post() { return Post; }

    vector<int> reversePost() {
        vector<int> ret;
        stack<int> tmp = ReversePost;
        while (!tmp.empty()) {
            ret.push_back(tmp.top());
            tmp.pop();
        }
        return ret;
    }

};

class EWDTopological {
    vector<int> Order;
public:

    explicit EWDTopological(const EdgeWeightedDigraph &G) {
        EdgeWeightedDirectedCycle cycleFinder(G);
        if (!cycleFinder.hasCycle()) {
            EWDDepthFirstOrder dfs(G);
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

class AcyclicSP { //无环加权有向图
    vector<DirectedEdge> edgeTo_;
    vector<double> distTo_;

    void relax(const EdgeWeightedDigraph &G, int v) {
        for (auto &e : G.adj(v)) {
            int from = e.from(), to = e.to();
            if (distTo_[to] > distTo_[from] + e.weight()) {
                distTo_[to] = distTo_[from] + e.weight();
                edgeTo_[to] = e;
            }
        }
    }

public:
    AcyclicSP(const EdgeWeightedDigraph &G, int s) : edgeTo_(G.V()), distTo_(G.V(), INT_MAX) {
        distTo_[s] = 0.0;
        EWDTopological top(G);
        for (auto &v : top.order())
            relax(G, v);
    }

    double distTo(int v) { return distTo_[v]; }

    bool hasPathTo(int v) { return distTo_[v] < INT_MAX; }

    vector<DirectedEdge> pathTo(int v) {
        vector<DirectedEdge> path;
        stack<DirectedEdge> stack;
        for (DirectedEdge e = edgeTo_[v]; e != DirectedEdge(); e = edgeTo_[e.from()]) {
            stack.push(e);
        }
        while (!stack.empty()) {
            path.push_back(stack.top());
            stack.pop();
        }
        return path;
    }
};

class BellmanFordSP {
    vector<double> distTo_;
    vector<DirectedEdge> edgeTo_;
    vector<bool> onQ;
    queue<int> queue;
    int cost;
    vector<DirectedEdge> cycle_;

    void relax(const EdgeWeightedDigraph &G, int v) {
        for (auto &e : G.adj(v)) {
            int to = e.to();
            if (distTo_[to] > distTo_[v] + e.weight()) {
                distTo_[to] = distTo_[v] + e.weight();
                edgeTo_[to] = e;
                if (!onQ[to]) {
                    queue.push(to);
                    onQ[to] = true;
                }
            }
            if (cost++ % G.V() == 0)
                findNegativeCycle();
        }
    }

    void findNegativeCycle() {
        int V = edgeTo_.size();
        EdgeWeightedDigraph spt(V);
        for (int v = 0; v < V; ++v)
            if (edgeTo_[v] != DirectedEdge())
                spt.addEdge(edgeTo_[v]);
        EdgeWeightedDirectedCycle cf(spt);
        cycle_ = cf.cycle();

    }


public:
    BellmanFordSP(const EdgeWeightedDigraph &G, int s) :
            distTo_(G.V(), INT_MAX), edgeTo_(G.V()), onQ(G.V(), false), cost(0) {
        distTo_[s] = 0.0;
        queue.push(s);
        onQ[s] = true;
        while (!queue.empty() && !hasNegativeCycle()) {
            int v = queue.front();
            queue.pop();
            onQ[v] = false;
            relax(G,v);
        }
    }

    double distTo(int v) const { return distTo_[v]; }

    bool hasPathTo(int v) const { return distTo_[v] < INT_MAX; }

    vector<DirectedEdge> pathTo(int v) {
        vector<DirectedEdge> path;
        stack<DirectedEdge> stack;
        for (DirectedEdge e = edgeTo_[v]; e != DirectedEdge(); e = edgeTo_[e.from()]) {
            stack.push(e);
        }
        while (!stack.empty()) {
            path.push_back(stack.top());
            stack.pop();
        }
        return path;
    }

    bool hasNegativeCycle() const {return !cycle_.empty();}

    vector<DirectedEdge> negativeCycle() const {return cycle_;}


};



void testSP(const string &str) {
    ifstream in(str);
    EdgeWeightedDigraph G(in);

    DijkstraAllPairSP sp(G);
    for (int i = 0; i < G.V(); ++i) {
        for (int j = 0; j < G.V(); ++j) {
            cout << i << " to " << j << "\n";
            for (auto &path :sp.path(i, j))
                cout << path.toString();
            cout << "\ndist: " << sp.dist(i, j) << "\n" << endl;
        }
    }
}

void testSP() {
    testSP("../FILE/tinyEWD.txt");
}

#endif //ALGORITHM_SP_H
