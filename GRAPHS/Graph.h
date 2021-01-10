//
// Created by 邓鑫林 on 2021/1/9.
//

#ifndef ALGORITHM_GRAPH_H
#define ALGORITHM_GRAPH_H

#include <algorithm>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <stack>
#include <queue>
#include <map>

using namespace std;

class Graph {
private:
    int V;
    int E;
    vector<list < int>> adj; // 邻接表
public:
    using Iterator = list<int>::iterator;

    explicit Graph(int v = 0) : V(v), E(0), adj(v) {}

    explicit Graph(istream &in) : V(0), E(0) {
        in >> V;
        adj = vector<list < int>>
        (V);
        int edge;
        in >> edge;
        for (int i = 0; i < edge; ++i) {
            int v1, v2;
            in >> v1 >> v2;
            addEdge(v1, v2);
        }
    }


    int getV() const { return V; }

    int getE() const { return E; }

    void addEdge(int v, int w) {
        ++E;
        adj[v].push_front(w);
        adj[w].push_front(v);
    }

    Iterator begin(int i) {
        return adj[i].begin();
    }

    Iterator end(int i) {
        return adj[i].end();
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

class Search {
private:
    Graph &graph;
    int s;
    vector<bool> mark;
    int counts;

public:
    // 找到和起点s联通的所有起点
    Search(Graph &g, int s_) : graph(g), s(s_), mark(graph.getV(), false), counts(0) {
        dfs(s);
    }

    bool marked(int v) {// v与s是否联通
        return mark[v];
    }

    int count() {// 与s联通的顶点总数
        return counts;
    }

private:
    void dfs(int v) {
        mark[v] = true;
        counts++;
        for (auto i = graph.begin(v); i != graph.end(v); i++) {
            if (!mark[*i])
                dfs(*i);
        }
    }

    void bfs(int v) {
        queue<int> queue;
        vector<bool> marked;
        vector<int> edgeTo;
        marked[v] = true;
        queue.push(v);
        while (!queue.empty()) {
            int temp = queue.front();
            queue.pop();
            for (auto itr = graph.begin(temp); itr != graph.end(temp); itr++) {
                if (!marked[*itr]) {
                    edgeTo[*itr] = temp;
                    marked[*itr] = true;
                    queue.push(*itr);
                }
            }
        }
    }


};

class Path {
private:
    Graph &graph;
    int s; // 起点
    vector<bool> mark; // mark[i]是否调用过dfs()
    vector<int> lastPoint; // 路径上的最后一个点
public:
    Path(Graph &g, int s_) : graph(g), s(s_), mark(graph.getV(), false), lastPoint(graph.getV()) {
        dfs(s);
        printPath();
    }

    bool hasPathTo(int v) {
        return mark[v];
    }

    vector<int> pathTo(int v) {
        vector<int> path;
        if (!hasPathTo(v))
            return path;
        stack<int> stack;
        for (int i = v; i != s; i = lastPoint[i]) {
            stack.push(i);
        }
        stack.push(s);
        while (!stack.empty()) {
            path.push_back(stack.top());
            stack.pop();
        }
        return path;
    }


private:
    void dfs(int v) {
        mark[v] = true;
        for (auto i = graph.begin(v); i != graph.end(v); i++)
            if (!mark[*i]) {
                lastPoint[*i] = v;
                dfs(*i);
            }
    }

    void printPath() {
        for (int i = 0; i < graph.getV(); ++i) {
            if (hasPathTo(i)) {
                cout << s << " to " << i << " : ";
                for (auto x: pathTo(i))
                    if (x == s)
                        cout << x;
                    else
                        cout << " - " << x;
                cout << "\n############################" << endl;
            }
        }
    }
};

class CC {
private:
    Graph &graph;
    vector<bool> marked;
    vector<int> id;
    int count;

    void dfs(int v) {
        marked[v] = true;
        id[v] = count;
        for (auto itr = graph.begin(v); itr != graph.end(v); itr++) {
            if (!marked[*itr])
                dfs(*itr);
        }
    }

public:
    explicit CC(Graph &g) : graph(g), marked(g.getV(), false), id(g.getV()), count(0) {
        for (int i = 0; i < graph.getV(); ++i) {
            if (!marked[i]) {
                dfs(i);
                count++;
            }
        }
    }

    bool connected(int v, int w) const {
        return id[v] == id[w];
    }

    int getId(int v) const {
        return id[v];
    }

    int getCount() const {
        return count;
    }

};

class Cycle {
private:
    vector<bool> marked;
    bool cycled;

    void dfs(Graph &graph, int i, int s) {
        marked[i] = true;
        for (auto itr = graph.begin(i); itr != graph.end(i); i++)
            if (!marked[i])
                dfs(graph, *itr, i);
            else if (*itr != s)
                cycled = true;
    }

public:
    bool hasCycle() const {
        return cycled;
    }

    Cycle(Graph &graph) : marked(graph.getV(), false), cycled(false) {
        for (int i = 0; i < graph.getV(); i++)
            if (!marked[i])
                dfs(graph, i, i);
    }

};

class TwoColor {
    vector<bool> marked;
    vector<bool> color;
    bool twoColor;

    void dfs(Graph &graph, int v) {
        marked[v] = true;
        for (auto itr = graph.begin(v); itr != graph.end(v); ++itr)
            if (!marked[*itr]) {
                color[*itr] = color[v];
                dfs(graph, *itr);
            } else if (color[v] == color[*itr])
                twoColor = false;
    }

public:
    explicit TwoColor(Graph &graph) :
            marked(graph.getV(), false), color(graph.getV(), false), twoColor(false) {
        for (int i = 0; i < graph.getV(); ++i)
            if (!marked[i])
                dfs(graph, i);
    }

    bool isTwoColor() const { return twoColor; }
};

class SymbolGraph {
private:
    map<string, int> st;
    vector<string> keys;
    Graph graph;

    void addEdge(const string &v, const string &w) {
        auto itrV = st.find(v);
        auto itrW = st.find(w);
        int indexV = st.size();
        int indexW = indexV;
        if (itrV == st.end()) {
            st.insert(make_pair(v, indexV));
            keys.push_back(v);
            indexW++;
        }
        if (itrW == st.end()) {
            st.insert(make_pair(w, indexW));
            keys.push_back(w);
        }
        graph.addEdge(indexV, indexW);
    }

public:
    explicit SymbolGraph(istream &in) {
        string v, w;
        while (in >> v >> w) {
            addEdge(v, w);
        }
    }

    bool contains(const string &str) const {
        return st.find(str) != st.end();
    }

    int index(const string &str) const {
        auto itr = st.find(str);
        if (itr == st.end())
            return -1;
        return itr->second;
    }

    string name(int v) const { return keys[v]; }

    Graph G() const { return graph; }
};

void testGraph(const string &str) {
    ifstream in(str);
    Graph graph(in);
    in.close();
    cout << graph.toString() << endl;
}

#include "../FUNDAMENTALS/random.h"

void testDFS(const string &str) {
    Random random;
    random.setSeed(time(nullptr));
    ifstream in(str);
    Graph graph(in);
    in.close();
    Path path(graph, random.uniformInt(graph.getV()));
}


void testGraph() {
//    testGraph("../FILE/tinyG.txt");
//    testGraph("../FILE/mediumG.txt");
//    testGraph("../FILE/largeG.txt");

    testDFS("../FILE/mediumG.txt");
}

#endif //ALGORITHM_GRAPH_H
