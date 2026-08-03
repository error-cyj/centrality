#include "int_undirected_graph.h"


void read_graph(char *filename, Int_Undirected_Graph &G){
    ifstream ifs(filename);
    int n, m;
    vector<int> a[3];
    for (int i = 0; i < 3; i++)
        a[i].clear();
    ifs >> n >> m;
    //   printf("%d %d\n", n, m);
    for (int i = 0; i < m; i ++){
        int x[3];
        ifs >> x[0] >> x[1] >> x[2];
        //       printf("%d %d %d\n", x[0], x[1], x[2]);
        for (int i = 0; i <3 ; i++)
            a[i].push_back(x[i] -1 );
    }
    printf("start init graph\n");
    G = Int_Undirected_Graph(n, a[0],a[1],a[2]);
    G.edge_initialize();
}

