#include "int_undirected_graph.h"

Int_Undirected_Graph::Int_Undirected_Graph() {
    n = m = 0;
    E.clear();
}
Int_Undirected_Graph::Int_Undirected_Graph(int _n, vector<int>& X, vector<int>& Y, vector<int>& Z) {
    n = _n;
    m = X.size();
    map<int, int> _s;
    _s.clear();
    E.clear();
    for (int i = 0; i < n; i++)
        E.push_back(_s);
    // printf("x size %d y size %d z size %d\n", X.size(), Y.size(), Z.size());
    for (int i = 0; i < m; i++) {
        // printf("x[i] %d Y[i] %d Z[i] %d \n",X[i],Y[i],Z[i]);

        E[X[i]].insert(make_pair(Y[i], Z[i]));

    }
    D.resize(n);
    for (int i = 0; i < n; i++)
        D[i] = E[i].size();

}

void Int_Undirected_Graph::edge_initialize() {

    Edge.clear();
    for (int i = 0; i < n; i++) {
        vector< pair<int, int> > Ed;
        Ed.clear();
        for (map<int, int>::iterator it = E[i].begin(); it != E[i].end(); it++) {
            Ed.push_back(*it);
        }
        Edge.push_back(Ed);
    }

}
void Int_Undirected_Graph::delete_edge(int u, int v) {
    if (E[u].find(v) == E[u].end()) return;
    E[u].erase(E[u].find(v));

    D[u]--;

}
void Int_Undirected_Graph::insert_edge(int u, int v, int k) {
    if (E[u].find(v) != E[u].end()) {
        E[u][v] += k;

        return;
    }
    E[u].insert(make_pair(v, k));

    D[u]++;

}
bool Int_Undirected_Graph::is_edge_existed(int u, int v) {
    if (E[u].find(v) == E[u].end())
        return false;
    else return true;
}

void Int_Undirected_Graph::print() {
    for (int i = 0; i < n; i++) {
        for (map<int, int>::iterator it = E[i].begin(); it != E[i].end(); it++)
            cout << "(" << (*it).first << "," << (*it).second << ") ";
        cout << endl;
    }
}
