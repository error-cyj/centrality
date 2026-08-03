#include "graph.h"

double get_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

Graph::Graph() {
    n = m = 0;
    E.clear();
}
Graph::Graph(int _n, vector<pair<int, int> >& a) {
    n = _n;
    m = a.size();
    set<int> _s;
    _s.clear();
    E.clear();
    for (int i = 0; i < n; i++) {
        E.push_back(_s);
    }

    for (int i = 0; i < a.size(); i++) {
        //    cout << i << " " << a[i].first << " " << a[i].second << endl;
        E[a[i].first].insert(a[i].second);
        E[a[i].second].insert(a[i].first);
    }
    D.resize(n);
    for (int i = 0; i < n; i++)
        D[i] = E[i].size();
}
void Graph::edge_initialize() {
    Edge.clear();
    for (int i = 0; i < n; i++) {
        //   cout << i << endl;
        vector< int > Ed;
        Ed.clear();
        for (set<int>::iterator it = E[i].begin(); it != E[i].end(); it++) {
            Ed.push_back(*it);
        }
        Edge.push_back(Ed);
    }
}
bool Graph::is_edge_existed(int u, int v) {
    if (E[u].find(v) == E[u].end())
        return false;
    else return true;
}
void Graph::insert_edge(int u, int v) {
    if (E[u].find(v) != E[u].end()) return;
    E[u].insert(v);
    E[v].insert(u);
    D[u]++;
    D[v]++;
}
void Graph::delete_edge(int u, int v) {
    if (E[u].find(v) == E[u].end()) return;
    E[u].erase(E[u].find(v));
    E[v].erase(E[v].find(u));
    D[u]--;
    D[v]--;
}

