#include "TD.h"
vector<int> extd_num;

Node::Node() {
    vert.clear();
    leng.clear();
    pos.clear();
    dis.clear();
    ch.clear();
    pa = -1;
    unique_vertex = -1;
    height = 0;
}

Shortcut::Shortcut() {
}
Shortcut::Shortcut(int w, int _u, int _v) {
    //int v =
    weight = w;
    count = 1;
    uid = _u;
    vid = _v;
    L.clear(); R.clear();
    L.push_back(make_pair(-1, -1));
    R.push_back(make_pair(-1, -1));
}

Shortcut_Graph::Shortcut_Graph() {
}
Shortcut_Graph::Shortcut_Graph(Int_Undirected_Graph g) {

    D = g.D;
    n = g.n;
    m = g.m;
    E.clear();
    map<int, Shortcut> tmp_E;
    tmp_E.clear();
    for (int i = 0; i < n; i++) {
        E.push_back(tmp_E);
        for (map<int, int>::iterator it = g.E[i].begin(); it != g.E[i].end(); it++) {
            E[i].insert(make_pair((*it).first, Shortcut((*it).second, i, (*it).first)));
        }
    }

    // vector<pair<int, Shortcut> > tmp_Edge;
    // tmp_Edge.clear();
    // Edge.clear();
    // for (int i = 0; i < n; i++) {
    //     Edge.push_back(tmp_Edge);
    //     for (int j = 0; j < Edge[i].size(); j++) {
    //         Edge[i].push_back(make_pair(g.Edge[i][j].first, Shortcut(g.Edge[i][j].second, i, g.Edge[i][j].first)));
    //     }
    // }
}

void Shortcut_Graph::insert_edge(int u, int v, Shortcut k) {
    if (E[u].find(v) != E[u].end()) {
        E[u][v].count += k.count;
        E[v][u].count += k.count;
        E[u][v].L.insert(E[u][v].L.end(), k.L.begin(), k.L.end());
        E[u][v].R.insert(E[u][v].R.end(), k.R.begin(), k.R.end());
        E[v][u].L.insert(E[v][u].L.end(), k.L.begin(), k.L.end());
        E[v][u].R.insert(E[v][u].R.end(), k.R.begin(), k.R.end());
        return;
    }
    E[u].insert(make_pair(v, k));
    E[v].insert(make_pair(u, k));
    D[u]++;
    D[v]++;
}

void Shortcut_Graph::delete_edge(int u, int v) {
    if (E[u].find(v) == E[u].end()) return;
    E[u].erase(E[u].find(v));
    E[v].erase(E[v].find(u));
    D[u]--;
    D[v]--;
}

bool Shortcut_Graph::is_edge_existed(int u, int v) {
    if (E[u].find(v) == E[u].end())
        return false;
    else return true;
}

Shortcut Shortcut::operator + (const Shortcut& sc) const { //don't exsit
    Shortcut r;
    r.weight = weight + sc.weight;
    r.count = count * sc.count;
    r.L.clear();
    r.R.clear();
    r.L.push_back(make_pair(uid, vid));
    r.R.push_back(make_pair(sc.uid, sc.vid));
    return r;
}

void Shortcut::merge(Shortcut const& sc) {//already exist
    count += sc.count;
    L.insert(L.end(), sc.L.begin(), sc.L.end());
    R.insert(R.end(), sc.R.begin(), sc.R.end());
}



void Tree_Decomposition::reduce() {
    printf("start reduce \n");
    deg.clear();
    neighbor.clear();
    length.clear();
    fneighbor.clear();
    flength.clear();

    vector<int> vectmp;
    vectmp.clear();
    vector<Shortcut> sc_tmp;
    sc_tmp.clear();

    for (int i = 0; i <= SG.n; i++) {
        neighbor.push_back(vectmp);
        length.push_back(sc_tmp);
        fneighbor.push_back(vectmp);
        flength.push_back(sc_tmp);
    }
    SelEle se;
    se.DD.resize(SG.n + 1);
    se.DD2.resize(SG.n + 1);
    for (int i = 0; i < SG.n; i++) {
        se.DD[i] = SG.D[i];
        se.DD2[i] = SG.D[i];
        deg.insert(SelEle(i));
    }
    ord.clear();
    int cnt = 0;
    while (!deg.empty()) {
        cnt++;

        int x = (*deg.begin()).x;

        ord.push_back(x);
        deg.erase(deg.begin());
        vector<int> neigh;
        vector<Shortcut> leng;
        neigh.clear();
        leng.clear();

        for (map<int, Shortcut>::iterator it = SG.E[x].begin(); it != SG.E[x].end(); it++) {
            int y = (*it).first;
            neigh.push_back(y);
            leng.push_back((*it).second);
        }

        int k = -1;

        for (int i = 0; i < neigh.size(); i++) {

            int y = neigh[i];
            deg.erase(SelEle(y));
            SG.delete_edge(x, y);

            se.DD[y]--;
        }
        for (int pu = 0; pu < neigh.size(); pu++)
            for (int pv = pu + 1; pv < neigh.size(); pv++) {
                int u = neigh[pu], v = neigh[pv];
                Shortcut sc = leng[pu] + leng[pv];

                if (SG.is_edge_existed(u, v)) {
                    if (SG.E[u][v].weight > sc.weight) {
                        SG.E[u][v] = sc;
                        SG.E[v][u] = sc;
                        RSG.E[u][v] = sc;
                        RSG.E[v][u] = sc;
                    }
                    else if (SG.E[v][u].weight == sc.weight) {
                        SG.E[u][v].merge(sc);
                        SG.E[v][u].merge(sc);
                        RSG.E[u][v].merge(sc);
                        RSG.E[v][u].merge(sc);
                    }
                }
                else {
                    SG.insert_edge(u, v, sc);
                    RSG.insert_edge(u, v, sc);//bidirection
                    se.DD[u]++;
                    se.DD[v]++;
                    se.DD2[u]++;
                    se.DD2[v]++;
                }
            }
        for (int i = 0; i < neigh.size(); i++) {
            int y = neigh[i];
            deg.insert(SelEle(y));
        }
        neighbor[x] = neigh;
        length[x] = leng;
    }
    for (int i = 0; i < G.n; i++) {
        for (int j = 0; j < neighbor[i].size(); j++) {
            fneighbor[neighbor[i][j]].push_back(i);
            flength[neighbor[i][j]].push_back(length[i][j]);
        }
    }
}


int Tree_Decomposition::match(int x, vector<int>& neigh) {

    int nearest = neigh[0];
    for (int i = 1; i < neigh.size(); i++)
        if (rank[neigh[i]] > rank[nearest])
            nearest = neigh[i];
    int p = belong[nearest];
    vector<int> a = tree[p].vert;
    if (tree[p].unique_vertex >= 0) {
        a.push_back(tree[p].unique_vertex);
    }
    sort(a.begin(), a.end());
    int i, j = 0;
    for (; (i < neigh.size()) && (j < a.size()); ) {
        if (neigh[i] == a[j]) {
            i++; j++;
        }
        else if (neigh[i] < a[j])
            break;
        else j++;
    }
    if (i >= neigh.size()) {
        return p;
    }
    printf("no match!\n");
    return -1;

}

void Tree_Decomposition::update_dfs(int p) {
    int x = tree[p].unique_vertex;
    for (int i = 0; i < neighbor[x].size(); i++) {
        int y = neighbor[x][i];
        for (int j = 0; j < neighbor[x].size(); j++) {
            if (i == j)
                continue;
            int z = neighbor[x][j];
            int _y = y, _z = z;
            // if (i > j) {
            if (tree[belong[y]].height < tree[belong[z]].height) {
                int t = _y; _y = _z; _z = t;
            }

            int dis = INTMAX;
            for (int k = 0; k < neighbor[_y].size(); k++) {
                if (neighbor[_y][k] == _z)
                    dis = length[_y][k].weight;
            }

            if (length[x][i].weight > length[x][j].weight + dis) {

#ifdef PRINT
                printf("updated  weight x %d y %d z %d before %d dis %d after %d\n", x, _y, _z, length[x][i].weight, dis, length[x][j].weight + dis);
#endif
                length[x][i].weight = length[x][j].weight + dis;

            }
        }
    }
    for (int i = 0; i < tree[p].ch.size(); i++)
        update_dfs(tree[p].ch[i]);
}

void Tree_Decomposition::update_tree_distance() {
    printf("update tree distance \n");
    // update distance from top
    update_dfs(root);

}
void Tree_Decomposition::make_tree() {
    cout << "start make_tree" << endl;
    belong.resize(H.n + 1);
    rank.resize(H.n + 1);
    int len = ord.size() - 1;
    Node rootn;
    tree.clear();

    int x = ord[len];
    rootn.vert = neighbor[x];
    rootn.leng = length[x];
    rootn.unique_vertex = x;
    rootn.pa = -1;
    rootn.height = 1;
    rank[x] = 0;
    belong[x] = 0;
    tree.push_back(rootn);
    len--;
    int* count_width;
    count_width = (int*)malloc(sizeof(int) * (H.n + 1));
    for (int i = 0; i < H.n; i++)
        count_width[i] = 0;
    int max_width = 0;
    for (; len >= 0; len--) {
        int x = ord[len];
        Node nod;
        nod.vert = neighbor[x];
        count_width[neighbor[x].size() + 1]++;
        if (neighbor[x].size() + 1 > max_width)
            max_width = neighbor[x].size() + 1;
        nod.leng = length[x];
        nod.unique_vertex = x;
        int pa = match(x, neighbor[x]);
        tree[pa].ch.push_back(tree.size());
        nod.pa = pa;
        nod.height = tree[pa].height + 1;

        rank[x] = tree.size();
        belong[x] = tree.size();
        tree.push_back(nod);

    }

    root = 0;
}



void Tree_Decomposition::get_dfs_order(int p, vector<int>& a) {

    a.push_back(p);
    for (int i = 0; i < tree[p].ch.size(); i++)
        get_dfs_order(tree[p].ch[i], a);
}

void Tree_Decomposition::get_search_order() {
    stack<int> q;
    q.push(0);//  0一定是root
    search_order.clear();
    while (!q.empty()) {
        int u = q.top();
        q.pop();
        search_order.push_back(u);
        for (int i = 0;i < tree[u].ch.size();i++) {
            q.push(tree[u].ch[i]);
        }
    }
}

void Tree_Decomposition::TD_DFS(int i) {
    int origin = tree[i].unique_vertex;
    max_dis[origin] = 0;

    int p = i;
    int last_child = -1;
    int ch = tree[i].height - 1;
    while (p >= 0) {

        int x = tree[p].unique_vertex;

        pecc[ch][x] = max(pecc[ch][x], dis[ch][x]);

        for (int i_ = 0;i_ < tree[p].ch.size();i_++) {
            int child = tree[p].ch[i_];
            if (child != last_child) {
                // tmp_dfs_time = get_time();

                // if (scaned_vertices[origin][tree[child].unique_vertex]) {
                TD_SPT_DFS(child, origin);
                // }


                // dfs_time += get_time() - tmp_dfs_time;
            }
            pecc[ch][x] = max(pecc[ch][x], pecc[ch][tree[child].unique_vertex]);
        }
        max_dis[origin] = max(max_dis[origin], pecc[ch][x]);
        last_child = p;
        p = tree[p].pa;
    }
}



//origin = extend_node's current origin
void Tree_Decomposition::extend_node(int p, int origin, int origin_origin) {

    extd_num[origin]++;

    cnt++;
    int ch = tree[belong[origin]].height - 1;
    int  x = tree[p].unique_vertex;

    for (int i = 0; i < neighbor[x].size(); i++) {
        int y = neighbor[x][i], l = length[x][i].weight, label = lab[ch][y];
        //---iteratively extend-----------
        scan_cnt_edge++;
        if (tree[p].pa >= 0 && label >= 0) {
            // tmp_extend_part1 = get_time();
            int label_height = tree[belong[label]].height - 1;
            if (cover[label_height][x]) {

                int _origin = label;
                cover[label_height][x] = 0;

                for (int j = 0; j < tree[p].ch.size(); j++) {
                    int q = tree[tree[p].ch[j]].unique_vertex;
                    cover[label_height][q] = 1;
                    if (changed_vertices_stamp[label_height][q] != label) {
                        changed_vertices[label_height].push_back(q);
                        changed_vertices_stamp[label_height][q] = label;
                    }
                }
                if (_origin != origin) {
                    extend_node(p, _origin, origin_origin);
                }
            }
        }
        if (dis[ch][x] > dis[ch][y] + l) {
            if (changed_vertices_stamp[ch][x] != origin) {
                changed_vertices[ch].push_back(x);
                changed_vertices_stamp[ch][x] = origin;
            }
            dis[ch][x] = dis[ch][y] + l;
            if (y != origin) {
                if (label < 0) {
                    lab[ch][x] = -1;
                }
                else {
                    lab[ch][x] = label;
                }
            }
            else {//y==origin
                lab[ch][x] = -1;
            }
        }
    }

    if (origin != origin_origin) {
        // tmp_extend_part3 = get_time();
        int label = lab[ch][x];

        if (label == x) {
            if (pecc[ch][x] == 0) {
                pecc[ch][x] = max(pecc[ch][x], pecc[tree[belong[label]].height - 1][x]);
            }
        }
        else {
            if (pecc[ch][x] == 0) {
                pecc[ch][x] = max(pecc[ch][x], pecc[tree[belong[label]].height - 1][x] + dis[tree[belong[origin]].height - 1][label]);
            }
        }
        // extend_part3 += get_time() - tmp_extend_part3;
    }

}


void Tree_Decomposition::TD_SPT_DFS(int p, int origin) {

    int ch = tree[belong[origin]].height - 1;

    int  x = tree[p].unique_vertex;

    if (changed_vertices_stamp[ch][x] != origin) {
        changed_vertices[ch].push_back(x);
        changed_vertices_stamp[ch][x] = origin;
    }



    comp[ch][x] = stamp;



    extend_node(p, origin, origin);



    if (max_dis[origin] < dis[ch][x]) {
        max_dis[origin] = dis[ch][x];
    }

    int label = lab[ch][x];
    bool ok = true;

    if (label < 0) {
        ok = false;
    }
    else {
        for (int i = 0; i < tree[p].vert.size(); i++) {
            // int y = tree[p].vert[i];
            if (lab[ch][tree[p].vert[i]] != label) {
                ok = false;
                break;
            }
        }
    }

    int current_dis = dis[ch][x];

    if (ok) {
        // ! 0.768148
        // TODO optimise 11/13
        // tmp_dfs_part3 = get_time();

        for (int i = 0; i < tree[p].ch.size();i++) {
            int q = tree[p].ch[i];


            cover[ch][tree[q].unique_vertex] = 1;
            // tmp_push_back_time = get_time();
            if (changed_vertices_stamp[ch][tree[q].unique_vertex] != origin) {
                changed_vertices[ch].push_back(tree[q].unique_vertex);
                changed_vertices_stamp[ch][tree[q].unique_vertex] = origin;
            }


        }

        int _dis = 0, __dis;

        _dis = dis[ch][label];

        __dis = pecc[tree[belong[label]].height - 1][x] + _dis;

        pecc[ch][x] = max(__dis, pecc[ch][x]);

        if (max_dis[origin] < __dis && tree[p].ch.size() != 0) {

            current_dis = __dis;

            max_dis[origin] = max(max_dis[origin], __dis);
        }

    }
    else {

        for (int i = 0; i < tree[p].ch.size(); i++)
        {
            int child = tree[p].ch[i];
            if (comp[ch][tree[child].unique_vertex] != stamp) {


                TD_SPT_DFS(child, origin);



                int _dis = pecc[ch][tree[child].unique_vertex];

                if (current_dis < _dis) {
                    current_dis = _dis;
                }
            }
        }
    }





    // tmp_dfs_part4 = get_time();
    if (tree[p].ch.size() != 0) {
        pecc[ch][x] = max(pecc[ch][x], current_dis);
        max_dis[origin] = max(max_dis[origin], pecc[ch][x]);
    }
    else {
        pecc[ch][x] = dis[ch][x];
        max_dis[origin] = max(max_dis[origin], pecc[ch][x]);
    }
    // dfs_part4 += get_time() - tmp_dfs_part4;

}

void Tree_Decomposition::compute_TD_SPT() {
    init_time = get_time();
    int check = 0;
    long long total_check = 0ll;

    cover.resize(G.n + 1);

    origin_stamp.resize(G.n + 1);
    extend.clear();
    Get_Subtree_Nodes_();





    int max_h = 0;
    for (int i = G.n; i >= 0; i--) {
        int p = i, h = tree[i].height - 1;

        if (max_h < h)
            max_h = h;
    }

    scan_log.resize(G.n);

    dis.resize(max_h + 1);
    lab.resize(max_h + 1);
    comp.resize(max_h + 1);
    pecc.resize(max_h + 1);
    cover.resize(max_h + 1);
    changed_vertices.resize(max_h + 1);
    changed_vertices_stamp.resize(max_h + 1);

    for (int i = 0; i <= max_h; i++) {
        dis[i].resize(G.n + 1);
        lab[i].resize(G.n + 1);
        comp[i].resize(G.n + 1);
        pecc[i].resize(G.n + 1);
        cover[i].resize(G.n + 1);
        changed_vertices[i].reserve(10 * G.n + 1);
        changed_vertices_stamp[i].resize(G.n + 1);
        for (int j = 0; j < G.n + 1; j++) {
            changed_vertices_stamp[i][j] = -1;
            dis[i][j] = INTMAX;
            lab[i][j] = -1;
            comp[i][j] = 0;
            pecc[i][j] = 0;
            cover[i][j] = 0;
        }
    }

    max_dis.resize(G.n + 1);
    release_num.resize(G.n + 1);

    for (int i = G.n; i >= 0; i--) {
        release_num[i].resize(neighbor[i].size());
        cover[i].clear();
        for (int j = 0; j < neighbor[i].size(); j++)
            release_num[i][j] = 0;
    }

    vector<int> dfs_order;
    dfs_order.clear();



    get_dfs_order(root, dfs_order);

    extd_num.resize(G.n + 1);
    // degree
    degree.resize(G.n);
    for (int i = 0;i < G.n;i++) {
        degree[i] = H.E[i].size();
    }
    // printf("init complete\n");
    init_time = get_time() - init_time;

    tmp_spt_time = get_time();
    for (int _i = 0; _i < G.n; _i++) {
        if (G.n > 1000) {
            if (_i % (G.n / 100) == 0) {
                printf("%d / %d\n", _i, G.n);
            }
        }
        int i = dfs_order[_i];

        int p = i;

        int ch = tree[i].height - 1;

        if (last_origin >= 0 && tree[last_origin].ch.size() == 0) {

            for (int h = ch;h <= tree[last_origin].height - 1;h++) {

                for (auto it : changed_vertices[h]) {
                    dis[h][it] = INTMAX;
                    lab[h][it] = -1;
                    comp[h][it] = 0;
                    pecc[h][it] = 0;
                    cover[h][it] = 0;
                }

                changed_vertices[h].clear();

            }
        }
        // spt_init_time += get_time() - tmp_spt_init_time;
        stamp++;
        extd_num[i] = 0;
        origin_stamp[tree[i].unique_vertex] = stamp;
        dis[ch][tree[i].unique_vertex] = 0;

        // first_ch[ch] = 1; 
        cnt = 0;
        tmp_up_search = get_time();
        upward_search(i, ch);
        upward_time += get_time() - tmp_up_search;





        min_scan = 0;
        p = i;
        cv = i;
        cuts = 0;
        max_dis[tree[i].unique_vertex] = 0;

        int last = p;
        // spt_time += get_time() - tmp_spt_time;
        tmp_td_dfs_time = get_time();

        TD_DFS(i);



        td_dfs_time += get_time() - tmp_td_dfs_time;

        // tmp_spt_time = get_time();



        last_origin = i;
#ifdef debug 
        printf("i, tree[i].unique_vertex, max_dis[tree[i].unique_vertex]: %d %d %d\n", i, tree[i].unique_vertex, max_dis[tree[i].unique_vertex]);
#endif
        // ! scan cnt
        // total_cuts += cuts;
        scan_cnt += (long long)cnt;
        scan_log[i] = (long long)cnt;

#ifdef debug
        printf("ratio: %.6lf\n", (double)scan_cnt / (double)((long long)(i + 1) * (long long)G.n));

        printf("~~~~~~~~~~~~dis %d~~~~~~~~~~~~\ni   :", tree[i].unique_vertex);
        for (int ii = 0;ii < G.n;ii++) {
            printf("%2d| ", ii);
        }
        printf("\ndis :");
        for (int ii = 0;ii < G.n;ii++) {
            if (dis[tree[i].height - 1][ii] > 99) {
                printf("%2d| ", 99);
            }
            else {
                printf("%2d| ", dis[tree[i].height - 1][ii]);
            }
        }
        printf("\npecc:");
        for (int ii = 0;ii < G.n;ii++) {
            if (pecc[tree[i].height - 1][ii] > 99) {
                printf("%2d| ", 99);
            }
            else {
                printf("%2d| ", pecc[tree[i].height - 1][ii]);
            }
        }
        printf("\nlab :");
        for (int ii = 0;ii < G.n;ii++) {
            printf("%2d| ", lab[tree[i].height - 1][ii]);
        }
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~\n");
#endif

    }

}

void Tree_Decomposition::print_tree() {
    queue<int> q;
    q.push(0);
    while (!q.empty()) {
        vector<int> q2;
        q2.clear();
        int n = q.size();

        while (n--) {
            int u = q.front();
            q2.push_back(u);
            q.pop();
            for (int i = 0;i < tree[u].ch.size();i++) {
                q.push(tree[u].ch[i]);
            }
        }
        for (int i = 0;i < q2.size();i++) {
            printf("i %d pa %d u_v %d|", q2[i], tree[q2[i]].pa, tree[q2[i]].unique_vertex + 1);
            for (int j = 0;j < tree[q2[i]].vert.size();j++) {
                printf("%d,", tree[q2[i]].vert[j] + 1);
            }
            printf("  ");
        }
        printf("\n");

    }

}

void Tree_Decomposition::upward_search(int i, int ch) {
    int p = i;
    int origin = tree[i].unique_vertex;
    while (p >= 0) {
        cnt++;
        int x = tree[p].unique_vertex;
        comp[ch][x] = stamp;

        changed_vertices[ch].push_back(x);
        changed_vertices_stamp[ch][x] = x;


        for (int j = 0; j < neighbor[x].size(); j++) {

            scan_cnt_edge++;

            int y = neighbor[x][j], l = length[x][j].weight;

            if (dis[ch][y] > dis[ch][x] + l) {
                dis[ch][y] = dis[ch][x] + l;


                if (p != i) {
                    if (lab[ch][x] < 0)
                        lab[ch][y] = x;
                    else lab[ch][y] = lab[ch][x];
                }
                else {
                    lab[ch][y] = y;
                }
            }
            else {
                if (dis[ch][x] > dis[ch][y] + l) {

                    dis[ch][x] = dis[ch][y] + l;

                    if (lab[ch][y] < 0)
                        lab[ch][x] = y;
                    else lab[ch][x] = lab[ch][y];
                }
            }
        }

        p = tree[p].pa;
    }
}



void Tree_Decomposition::Get_Subtree_Nodes_() {
    subtree_nodes.resize(G.n + 1);
    Get_Subtree_Nodes(root);
}
int Tree_Decomposition::Get_Subtree_Nodes(int p) {
    //include itself
    if (tree[p].ch.size() == 0) {
        subtree_nodes[p] = 1;
        return 1;
    }
    int total = 1;
    for (auto i : tree[p].ch) {
        total += Get_Subtree_Nodes(i);
    }
    subtree_nodes[p] = total;
    return total;
}

void Tree_Decomposition::compute_betweenness_centrality() {
    //make tree-decomposition
    H = G;
    SG = Shortcut_Graph(G);
    RSG = Shortcut_Graph(G);
    td_time = get_time();
    reduce();
    make_tree();
    update_tree_distance();
    td_time = get_time() - td_time;

#ifdef debug
    print_tree();
#endif
    //
    G = H;
    vector<int> list;
    list.clear();
    list.push_back(root);
    printf("start TD_SPT\n");



    compute_TD_SPT();


}

void read_graph(char* filename, int& n, vector<int>& X, vector<int>& Y, vector<int>& Z) {
    ifstream ifs(filename);
    if (ifs.fail()) {
        printf("check path!\n");
    }
    int m;

    ifs >> n >> m;
    X.resize(m);
    Y.resize(m);
    Z.resize(m);
    for (int i = 0; i < m; i++) {
        ifs >> X[i] >> Y[i] >> Z[i];
        X[i]--;
        Y[i]--;
    }
}

Tree_Decomposition brandes;

int main(int argc, char* argv[]) {

    printf("start reading graph\n");
    int n;
    vector<int> X, Y, Z;
    read_graph(argv[1], n, X, Y, Z);
    printf("after reading graph\n");
    brandes.G = Int_Undirected_Graph(n, X, Y, Z);
    printf("start brandes\n");

    // brandes.f_scaned_vertices.open(argv[4]);
    // brandes.scaned_vertices.resize(brandes.G.n + 1);
    // for (int i = 0;i <= brandes.G.n;i++) {
    //     if (i % 1000 == 0) {
    //         printf("read %d \n", i);
    //     }
    //     brandes.scaned_vertices[i].resize(brandes.G.n + 1);
    //     for (int j = 0;j < brandes.G.n;j++) {
    //         brandes.scaned_vertices[i][j] = 0;
    //     }
    //     int u, n;
    //     brandes.f_scaned_vertices >> u >> n;
    //     for (int j = 0;j < n;j++) {
    //         int tmp;
    //         brandes.f_scaned_vertices >> tmp;
    //         brandes.scaned_vertices[i][tmp] = 1;
    //     }
    // }

    // for (int i = 0;i < brandes.G.n;i++) {
    //     int u, n;

    //     for (int i = 0;i < n;i++) {
    //         int tmp;
    //         brandes.f_scaned_vertices >> tmp;
    //         brandes.scaned_vertices[u].push_back(tmp);
    //     }
    // }


    double t_s = get_time();
    brandes.compute_betweenness_centrality();
    double t_t = get_time();
    brandes.t_round = get_time();
    ofstream ofs(argv[2], ios::trunc | ios::out);
    if (ofs.fail()) {
        printf("check out path\n");
    }
    for (int i = 0;i < brandes.G.n;i++) {
        ofs << brandes.max_dis[i] << endl;
    }

#ifdef debug
    for (int i = 0;i < brandes.G.n;i++) {
        printf("%2d |", i);
    }
    printf("\n");
    for (int i = 0;i < brandes.G.n;i++) {
        printf("%2d |", brandes.max_dis[i]);
    }
    printf("\n");
#endif
    brandes.f_time_log.open(argv[3]);
    for (int i = 0;i < brandes.G.n;i++) {
        brandes.f_time_log << brandes.scan_log[i] << "\n";
    }

    brandes.f_time_log.open(argv[3], ios::app);
    brandes.f_time_log << "total " << t_t - t_s << "\n";
    brandes.f_time_log << "td_dfs_time " << brandes.td_dfs_time << "\n";
    brandes.f_time_log << "up_search " << brandes.upward_time << "\n";
    brandes.f_time_log << "\n";


    printf("scan_cnt %lld\n", brandes.scan_cnt);
    printf("scan_cnt_edge %lld\n", brandes.scan_cnt_edge);
    cout << argv[1] << "  " << t_t - t_s << "\n";

    // brandes.f_scaned_vertices.open(argv[4], ios::out | ios::trunc);
    // for (int i = 0;i < brandes.G.n;i++) {
    //     brandes.f_scaned_vertices << i << " " << brandes.scaned_vertices[i].size() << " ";
    //     for (auto it : brandes.scaned_vertices[i]) {
    //         brandes.f_scaned_vertices << it << " ";
    //     }
    //     brandes.f_scaned_vertices << "\n";
    // }
}
