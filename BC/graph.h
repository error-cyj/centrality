#ifndef GRAPH_H_
#define GRAPH_H_

#include<cstdio>
#include<cstring>
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<set>
#include<stack>
#include<map>
#include<unordered_map>
#include<unordered_set>
#include<queue>
#include<algorithm>
#include<ctime>
#include <sys/time.h>
#include <xmmintrin.h>
#include <iomanip>
using namespace std;
const int INT_INFINITY = 999999999;
const int SIZEOFINT = sizeof(int);

double get_time(void);

class Graph{
public:
    // vertex number from 0 to n - 1;
	int n, m;
	//vector<int> V;
	vector< set<int> > E;
	vector< vector< int > > Edge;
	vector<int> D;
	Graph();
    Graph(int _n, vector<pair<int, int> > &a);
	void edge_initialize();
	bool is_edge_existed(int u, int v);
	void insert_edge(int u, int v);
	void delete_edge(int u, int v);
};


// priority element type
struct PT{
   	int dis;
	int x;
	PT(){
	}
	PT(int _dis, int _x){
		dis = _dis;
		x = _x;
	}
	// –°∏˘∂— 
	bool operator < (const PT _pt) const{
		if (dis == _pt.dis)
			return x > _pt.x;
		return dis > _pt.dis;
	}
};

#endif
