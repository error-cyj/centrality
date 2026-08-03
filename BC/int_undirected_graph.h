#ifndef INT_UNDIRECTED_GRAPH_H_
#define INT_UNDIRECTED_GRAPH_H_

#include "graph.h"


class Int_Undirected_Graph: public Graph{
public:
    // vertex number from 0 to n - 1;
	int n, m;
	//vector<int> V;
	vector< map<int, int> > E;
	vector< vector< pair<int, int> > > Edge;
	vector<int> D;
	Int_Undirected_Graph();
    Int_Undirected_Graph(int _n, vector<int> &X, vector<int> &Y, vector<int> &Z);
	void edge_initialize();
	bool is_edge_existed(int u, int v);
	void insert_edge(int u, int v, int k);
	void delete_edge(int u, int v);
    void print();
    
};


#endif
