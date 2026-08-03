#include "TD.h"
// #include<algorithm>
vector<int> extd_num;

Node::Node() {
    vert.clear();
    leng.clear();
    pos.clear();
    dis.clear();
    ch.clear();

    from.clear();
    vert_pos_in_ancs.clear();
    vert_pos_map.clear();
    tree_of_vertex.clear();
    delta_of_tree.clear();
    delta_of_sc.clear();
    pa = -1;
    unique_vertex = -1;
    height = 0;
}

Shortcut::Shortcut() {

    sc_id = -1;
    count = 1;
    removed_cnt = 0;
    delta = 0;

    real = true;
    L.clear(); R.clear();
    L.push_back(make_pair(-1, -1));
    R.push_back(make_pair(-1, -1));
}
Shortcut::Shortcut(int w, int _u, int _v) {

    weight = w;
    shortest_weight = w;
    count = 1;
    removed_cnt = 0;
    delta = 0;
    uid = _u;
    vid = _v;
    real = true;
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
    map<int, int> tmp_E;
    tmp_E.clear();
    for (int i = 0; i < n; i++) {
        E.push_back(tmp_E);
        for (map<int, int>::iterator it = g.E[i].begin(); it != g.E[i].end(); it++) {
            Shortcut sc;
            sc.shortest_weight = it->second, sc.weight = it->second, sc.uid = i, sc.vid = it->first, sc.sc_id = sc_pool.size();
            sc_pool.push_back(sc);//treat as directed graph
            E[i].insert(make_pair((*it).first, sc_pool.size() - 1));
        }
    }


}

void Shortcut_Graph::insert_edge(int u, int v, Shortcut k) {
    if (E[u].find(v) != E[u].end()) {
        sc_pool[E[u][v]].count += k.count;
        sc_pool[E[v][u]].count += k.count;

        return;
    }
    k.sc_id = sc_pool.size();
    sc_pool.push_back(k);
    E[u].insert(make_pair(v, sc_pool.size() - 1));

    k.sc_id = sc_pool.size();
    sc_pool.push_back(k);
    E[v].insert(make_pair(u, sc_pool.size() - 1));

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



Shortcut Shortcut::operator + (const Shortcut& sc) const {
    Shortcut r;
    r.weight = weight + sc.weight;
    r.shortest_weight = r.weight;
    r.count = count * sc.count;
    r.L.clear();
    r.R.clear();
    r.L.push_back(make_pair(uid, vid));
    r.R.push_back(make_pair(sc.uid, sc.vid));
    int mid_, u, v;

    if (uid == sc.uid) {
        mid_ = uid;
        r.uid = vid;
        r.vid = sc.vid;
    }
    else if (uid == sc.vid) {
        mid_ = uid;
        r.uid = vid;
        r.vid = sc.uid;
    }
    else if (vid == sc.uid) {
        mid_ = vid;
        r.uid = uid;
        r.vid = sc.vid;
    }
    else if (vid == sc.vid) {
        mid_ = vid;
        r.uid = uid;
        r.vid = sc.uid;
    }

    mid_with_sc tmp;
    tmp.mid = mid_;
    tmp.u_sc = make_pair(r.uid, sc_id);
    tmp.v_sc = make_pair(r.vid, sc.sc_id);

    r.mid_sc.push_back(tmp);

    r.real = false;
    return r;
}

Shortcut Shortcut::merge(Shortcut const& sc) {//distance equals 
    Shortcut r;
    r = *this;
    r.count += sc.count;
    r.mid_sc.insert(r.mid_sc.end(), sc.mid_sc.begin(), sc.mid_sc.end());

    r.L.insert(r.L.end(), sc.L.begin(), sc.L.end());
    r.R.insert(r.R.end(), sc.R.begin(), sc.R.end());
    r.real = r.real || sc.real;
    return r;
}


void Tree_Decomposition::expand(int x) {
    for (int i = 0;i < neighbor[x].size();i++) {
        SPPG[x].push_back(make_pair(neighbor[x][i], length[x][i]));
        SPPG[neighbor[x][i]].push_back(make_pair(x, length[x][i]));
    }


    if (length[x].size() == 1) {
        SG.E[x][neighbor[x][0]] = length[x][0];
        SG.E[neighbor[x][0]][x] = length[x][0];
    }
    else {
        for (int pu = 0;pu < neighbor[x].size();pu++) {
            for (int pv = pu + 1;pv < neighbor[x].size();pv++) {
                int u = neighbor[x][pu], v = neighbor[x][pv];
                Shortcut sc = SG.sc_pool[length[x][pu]] + SG.sc_pool[length[x][pv]];
                int sc_u = length[x][pu], sc_v = length[x][pv];

                SG.E[x][u] = length[x][pu], SG.E[u][x] = length[x][pu];
                SG.E[x][v] = length[x][pv], SG.E[v][x] = length[x][pv];
                if (sc.weight == SG.sc_pool[SG.E[u][v]].weight) {
                    if (SG.sc_pool[SG.E[u][v]].mid_sc.size() == 0) {
                        SG.delete_edge(u, v);
                        // printf("mid size zero \n");
                        // while (1);
                    }
                    else {

                        vector<mid_with_sc> tmp;
                        tmp.clear();
                        for (int i = 0;i < SG.sc_pool[SG.E[u][v]].mid_sc.size();i++) {
                            if (SG.sc_pool[SG.E[u][v]].mid_sc[i].mid != x) {
                                tmp.push_back(SG.sc_pool[SG.E[u][v]].mid_sc[i]);
                            }
                        }
                        SG.sc_pool[SG.E[u][v]].mid_sc = tmp;
                        // SG.sc_pool[SG.E[u][v]].mid_sc.erase(remove_if(SG.sc_pool[SG.E[u][v]].mid_sc.begin(), SG.sc_pool[SG.E[u][v]].mid_sc.end(), [x](const mid_with_sc& it) {
                        //     return it.mid == x;
                        //     }), SG.sc_pool[SG.E[u][v]].mid_sc.end());
                        SG.sc_pool[SG.E[u][v]].count -= sc.count;
                    }
                }
                else if (sc.weight > SG.sc_pool[SG.E[u][v]].weight) {

                }
                else {

                }
            }
        }

    }

}


void Tree_Decomposition::reduce_(int x) {

    for (int i = 0;i < neighbor[x].size();i++) {
        int y = neighbor[x][i];
        SG.delete_edge(x, y);
    }
    for (int i = 0;i < SPPG[x].size();i++) {
        SPPG[SPPG[x][i].first].erase(remove_if(SPPG[SPPG[x][i].first].begin(), SPPG[SPPG[x][i].first].end(),
            [&x](const pair<int, int>& element) {
                return element.first == x;
            }),
            SPPG[SPPG[x][i].first].end()
        );
    };
    SPPG[x].clear();

    // for (int i = 0;i < neighbor[x].size();i++) {
    //     // SPPG[]
    // }
    for (int pu = 0;pu < neighbor[x].size();pu++) {
        for (int pv = pu + 1;pv < neighbor[x].size();pv++) {
            Shortcut sc = SG.sc_pool[length[x][pu]] + SG.sc_pool[length[x][pv]];
            int u = neighbor[x][pu], v = neighbor[x][pv];
            int sc_u = length[x][pu], sc_v = length[x][pv];
            if (SG.is_edge_existed(u, v)) {

                if (sc.weight == SG.sc_pool[SG.E[u][v]].weight) {
                    // if (SG.E[u][v] == SG.E[v][u]) {
                    //     SG.sc_pool[SG.E[u][v]].count += sc.count;
                    //     SG.sc_pool[SG.E[u][v]].mid_sc.insert(SG.sc_pool[SG.E[u][v]].mid_sc.end(), sc.mid_sc.begin(), sc.mid_sc.end());
                    // }
                    // else {
                    //     printf("SG.E[u][v] != SG.E[v][u]\n");
                    //     while (1);
                    //     SG.sc_pool[SG.E[u][v]].count += sc.count;
                    //     SG.sc_pool[SG.E[u][v]].mid_sc.insert(SG.sc_pool[SG.E[u][v]].mid_sc.end(), sc.mid_sc.begin(), sc.mid_sc.end());
                    //     SG.sc_pool[SG.E[v][u]].count += sc.count;
                    //     SG.sc_pool[SG.E[v][u]].mid_sc.insert(SG.sc_pool[SG.E[v][u]].mid_sc.end(), sc.mid_sc.begin(), sc.mid_sc.end());
                    // }
                    SG.sc_pool[SG.E[u][v]].count += sc.count;
                    SG.sc_pool[SG.E[u][v]].mid_sc.insert(SG.sc_pool[SG.E[u][v]].mid_sc.end(), sc.mid_sc.begin(), sc.mid_sc.end());
                    SG.sc_pool[SG.E[u][v]].removed_cnt--;
                }
                else if (sc.weight < SG.sc_pool[SG.E[u][v]].weight) {

                }
            }
            else {
                int sc_id = 99999999;
                for (int i = 0;i < neighbor[u].size();i++) {
                    if (neighbor[u][i] == v) {
                        sc_id = length[u][i];
                    }
                }

                SG.E[u][v] = sc_id;
                SG.E[v][u] = sc_id;
            }
        }
    }
}


void Tree_Decomposition::reduce() {
    dfs_order.clear();
    deg.clear();
    neighbor.clear();
    length.clear();
    fneighbor.clear();
    flength.clear();



    vector<int> vectmp;
    vectmp.clear();
    vector<int> sc_tmp;
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
        vector<int> leng;
        vector<int> sigm;
        neigh.clear();
        leng.clear();

        int cnt_ = 0;
        for (map<int, int>::iterator it = SG.E[x].begin(); it != SG.E[x].end(); it++) {
            int y = (*it).first;
            neigh.push_back(y);
            leng.push_back(it->second);
        }
        int k = -1;

        for (int i = 0; i < neigh.size(); i++) {
            int y = neigh[i];
            deg.erase(SelEle(y));
            SG.delete_edge(x, y);
            se.DD[y]--;
        }

        for (int pu = 0; pu < neigh.size(); pu++) {

            for (int pv = pu + 1; pv < neigh.size(); pv++) {

                int u = neigh[pu], v = neigh[pv];
                Shortcut sc = SG.sc_pool[leng[pu]] + SG.sc_pool[leng[pv]];
                if (SG.is_edge_existed(u, v)) {
                    if (SG.sc_pool[SG.E[u][v]].weight > sc.weight) {
                        sc.sc_id = SG.sc_pool.size();
                        SG.sc_pool.push_back(sc);
                        SG.E[u][v] = SG.sc_pool.size() - 1;
                        sc.sc_id = SG.sc_pool.size();
                        SG.sc_pool.push_back(sc);
                        SG.E[v][u] = SG.sc_pool.size() - 1;
                    }
                    else if (SG.sc_pool[SG.E[v][u]].weight == sc.weight) {
                        Shortcut origin_sc_0 = SG.sc_pool[SG.E[u][v]], origin_sc_1 = SG.sc_pool[SG.E[v][u]];
                        Shortcut sc_ = origin_sc_0.merge(sc);
                        sc_.sc_id = SG.sc_pool.size();
                        SG.sc_pool.push_back(sc_);
                        SG.E[u][v] = SG.sc_pool.size() - 1;
                        Shortcut sc__ = origin_sc_1.merge(sc);
                        sc__.sc_id = SG.sc_pool.size();
                        SG.sc_pool.push_back(sc__);
                        SG.E[v][u] = SG.sc_pool.size() - 1;

                    }
                    else {
                    }
                }
                else {


                    //!
                    if (SG.E[u].find(v) != SG.E[u].end()) {
                        SG.sc_pool[SG.E[u][v]].count += sc.count;
                        SG.sc_pool[SG.E[v][u]].count += sc.count;

                        return;
                    }
                    sc.sc_id = SG.sc_pool.size();
                    SG.sc_pool.push_back(sc);

                    SG.E[u].insert(make_pair(v, SG.sc_pool.size() - 1));

                    sc.sc_id = SG.sc_pool.size();
                    SG.sc_pool.push_back(sc);

                    SG.E[v].insert(make_pair(u, SG.sc_pool.size() - 1));

                    SG.D[u]++;
                    SG.D[v]++;
                    //!
                    se.DD[u]++;
                    se.DD[v]++;
                    se.DD2[u]++;
                    se.DD2[v]++;
                }

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

            if (tree[belong[y]].height < tree[belong[z]].height) {
                int t = _y; _y = _z; _z = t;
            }

            int dis = INTMAX, count = 1;
            Shortcut sc;
            for (int k = 0; k < neighbor[_y].size(); k++) {
                if (neighbor[_y][k] == _z)
                {
                    dis = SG.sc_pool[length[_y][k]].shortest_weight;
                    // count = length[_y][k].count;
                    sc = SG.sc_pool[length[_y][k]];
                }
            }

            if (SG.sc_pool[length[x][i]].shortest_weight > SG.sc_pool[length[x][j]].shortest_weight + dis) {
                SG.sc_pool[length[x][i]].shortest_weight = SG.sc_pool[length[x][j]].shortest_weight + dis;
            }
        }
    }
    for (int i = 0; i < tree[p].ch.size(); i++)
        update_dfs(tree[p].ch[i]);
}

void Tree_Decomposition::update_tree_distance() {


    update_dfs(root);

}
void Tree_Decomposition::make_tree() {

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
    // int* count_width;
    // count_width = (int*)malloc(sizeof(int) * (H.n + 1));
    // for (int i = 0; i < H.n; i++)
    //     count_width[i] = 0;
    int max_width = 0;
    for (; len >= 0; len--) {
        int x = ord[len];
        Node nod;
        nod.vert = neighbor[x];
        // count_width[neighbor[x].size() + 1]++;
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
        for (int i = 0;i < nod.vert.size();i++) {
            int v = nod.vert[i];
            nod.vert_pos_map[v] = i;
            if (tree[pa].vert_pos_map.find(v) == tree[pa].vert_pos_map.end()) {
                nod.vert_pos_in_ancs.push_back(-1);
            }
            else {
                nod.vert_pos_in_ancs.push_back(tree[pa].vert_pos_map[v]);
            }
        }

        tree.push_back(nod);

    }

    root = 0;
}



void Tree_Decomposition::get_dfs_order(int p, vector<int>& a) {

    a.push_back(tree[p].unique_vertex);
    for (int i = 0; i < tree[p].ch.size(); i++)
        get_dfs_order(tree[p].ch[i], a);
}

void Tree_Decomposition::get_search_order() {
    stack<int> q;
    q.push(0);
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


    int p = i;
    int last_child = -1;
    int ch = tree[i].height - 1;
    while (p >= 0) {

        int x = tree[p].unique_vertex;

        for (int i_ = 0;i_ < tree[p].ch.size();i_++) {
            int child = tree[p].ch[i_];
            if (child != last_child) {
                TD_SPT_DFS(child, origin);
            }
        }

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

    // printf("extend x %d origin %d origin %d \n", x, origin, origin_origin);
    if (searched_vertices_stamp[ch][x] != origin) {

        searched_vertices[ch].push_back(x);
        searched_vertices_stamp[ch][x] = origin;
    }


    for (int i = 0; i < neighbor[x].size(); i++) {
        int y = neighbor[x][i], l = SG.sc_pool[length[x][i]].shortest_weight, label = lab[ch][y], c = SG.sc_pool[length[x][i]].count, l2 = SG.sc_pool[length[x][i]].weight;

        // TODO extend delta ? 
        if (tree[p].pa >= 0 && label >= 0) {

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

                extend_node(p, _origin, origin_origin);

            }

        }

        scan_cnt_edge++;
        if (dis[ch][x] > dis[ch][y] + l) {


            if (changed_vertices_stamp[ch][x] != origin) {
                changed_vertices[ch].push_back(x);
                changed_vertices_stamp[ch][x] = origin;
            }



            dis[ch][x] = dis[ch][y] + l;


            if (y != origin) {
                lab[ch][x] = label;
            }
            else {//y==origin
                lab[ch][x] = -1;
            }

        }
        else if (dis[ch][x] == dis[ch][y] + l) {

        }


    }

    for (int i = 0; i < neighbor[x].size(); i++) {

        int y = neighbor[x][i], l = SG.sc_pool[length[x][i]].shortest_weight, label = lab[ch][y], c = SG.sc_pool[length[x][i]].count, l2 = SG.sc_pool[length[x][i]].weight;
        if (dis[ch][x] == dis[ch][y] + l2) {
            tree[p].from[ch].push_back(i);
            sigma[ch][x] += sigma[ch][y] * c;

            if (origin == origin_origin) {
                pre[x].push_back(make_pair(y, length[x][i]));
            }

        }
    }
    if (origin != origin_origin) {

        int label = lab[ch][x];
        int label_height = tree[belong[label]].height - 1;
        ok_[ch][x] = 1;
        for (auto child : tree[p].ch) {
            int child_ = tree[child].unique_vertex;
            lab[ch][child_] = label;
            ok_[ch][child_] = 1;
        }

        for (int i = 0;i < tree[p].delta_of_tree[ch].size();i++) {

            tree[p].delta_of_tree[ch][i] = tree[p].delta_of_tree[label_height][i];
        }

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
    spt_order[dij_ordet_count++] = x;
    extend_node(p, origin, origin);

    int label = lab[ch][x];
    bool ok = true;

    if (label < 0 || tree[p].ch.size() == 0) {
        ok = false;
    }
    else {
        for (int i = 0; i < tree[p].vert.size(); i++) {
            if (lab[ch][tree[p].vert[i]] != label) {
                ok = false;
                break;
            }
        }
    }

    int current_dis = dis[ch][x];
    int label_height = tree[belong[label]].height - 1;
    rational_number tmp(sigma[ch][x], sigma[label_height][x]);
    if (ok) {
        for (auto it : neighbor[x]) {
            if (rational_number(sigma[ch][it], sigma[label_height][it]) != tmp) {
                ok = false;
                break;
            }
        }
    }


    if (ok == false) {
        ok_[ch][x] = 0;
    }
    else {
        ok_[ch][x] = 0;
        for (auto child : tree[p].ch) {
            int child_ = tree[child].unique_vertex;
            ok_[ch][child_] = 1;
            lab[ch][child_] = label;
        }
    }

    if (ok) {

        int label_height = tree[belong[label]].height - 1;
        int recursive_label_ = label;

        int recursive_label_height = tree[belong[recursive_label_]].height - 1;


        if (!cut_cnt_collection_collection[recursive_label_height][x]) {
            cut_cnt_collection[recursive_label_height].push_back(x);
            cut_cnt_collection_collection[recursive_label_height][x] = 1;
        }

        for (int i = 0;i < tree[p].ch.size();i++) {
            cut_cnt[recursive_label_height][tree[tree[p].ch[i]].unique_vertex] += 1;
        }


        if (changed_vertices_stamp[label_height][x] != label) {
            changed_vertices[label_height].push_back(x);
            changed_vertices_stamp[label_height][x] = label;
        }



        for (int i = 0; i < tree[p].ch.size();i++) {
            int q = tree[p].ch[i];
            cover[ch][tree[q].unique_vertex] = 1;
            if (changed_vertices_stamp[ch][tree[q].unique_vertex] != origin) {
                changed_vertices[ch].push_back(tree[q].unique_vertex);
                changed_vertices_stamp[ch][tree[q].unique_vertex] = origin;
            }
        }
    }
    else {

        for (int i = 0; i < tree[p].ch.size(); i++)
        {
            int child = tree[p].ch[i];
            if (comp[ch][tree[child].unique_vertex] != stamp) {
                TD_SPT_DFS(child, origin);
            }
        }
    }

    if (ok) {


        for (int i = 0;i < tree[p].delta_of_tree[ch].size();i++) {
            tree[p].delta_of_tree[ch][i] = tree[p].delta_of_tree[tree[belong[label]].height - 1][i];
        }
        for (int i = 0;i < tree[p].vert.size();i++) {
            int it = tree[p].vert[i];
            delta[ch][it] += tree[p].delta_of_tree[tree[belong[label]].height - 1][i + 1];
        }
        delta[ch][x] += tree[p].delta_of_tree[tree[belong[label]].height - 1][0];
    }
    else {
    }

}

void Tree_Decomposition::compute_TD_SPT() {

    init();

    printf("inited\n");
    // compute_sigma_bar();

    for (int _i = 0; _i < G.n; _i++) {
        pre.clear();
        pre.resize(G.n + 1);
        int i = belong[dfs_order[_i]];
        dij_ordet_count = 0;
        int p = i;
        int x = tree[p].unique_vertex;
        int ch = tree[i].height - 1;

        int origin = tree[i].unique_vertex;


        if (G.n > 100) {
            if (_i % (G.n / 100) == 0) {
                printf("%d / %d \n", _i, G.n);
            }
        }
#ifdef debug
        printf("~~~~~~~~~~~~~~SPTing %d ~~~~~~~~~~~~~~~~\n", origin);
#endif
        //lazy recover


        expand(x);

        if (last_origin >= 0 && tree[last_origin].ch.size() == 0) {

            last_height = ch;


            add_up_cuted_subgraph(ch, tree[last_origin].height - 1);

            for (int h = tree[last_origin].height - 1;h >= ch;h--) {
                reduce_(tree_chain[h]);
            }


            // tmp_lazy_time = get_time();
            for (int h = ch;h <= tree[last_origin].height - 1;h++) {

                spt[h].clear();
                spt[h].resize(G.n + 1);
                // pre[h].clear();
                // pre[h].resize(G.n + 1);


                for (int ii = 0;ii < cut_cnt[h].size();ii++) {
                    cut_cnt[h][ii] = 0;
                }
                // for (int it = 0;it < searched_vertices_stamp[h].size();it++) {
                //     searched_vertices_stamp[h][it] = -1;
                // }
                for (auto it : searched_vertices[h]) {
                    searched_vertices_stamp[h][it] = -1;
                }
                // for (int it = 0;it < cut_cnt_collection[h].size();it++) {
                //     cut_cnt_collection_collection[h][it] = 0;

                // }
                for (auto it : cut_cnt_collection[h]) {
                    cut_cnt_collection_collection[h][it] = 0;
                }

                for (auto it : changed_vertices[h]) {
                    dis[h][it] = INTMAX;
                    lab[h][it] = -1;
                    comp[h][it] = 0;
                    cover[h][it] = 0;
                    delta[h][it] = 0;

                    sigma[h][it] = 0;
                    BC_DFSed[h][it] = -1;

                    cut_cnt[h][it] = 0;

                    ok_[h][it] = -1;




                    tree[belong[it]].from[h].clear();
                    for (int ii = 0;ii < tree[belong[it]].delta_of_tree[h].size();ii++) {
                        tree[belong[it]].delta_of_tree[h][ii] = 0;
                    }
                    for (int ii = 0;ii < tree[belong[it]].delta_of_sc[h].size();ii++) {
                        tree[belong[it]].delta_of_sc[h][ii] = 0;
                    }

                }

                searched_vertices[h].clear();
                changed_vertices[h].clear();
                cut_cnt_collection[h].clear();
            }
            // lazy_time += get_time() - tmp_lazy_time;
        }

        tree_chain[ch] = origin;
        last_origin = i;

        // tmp_upward_time = get_time();
        upward_search_dij(i, ch);
        // upward_time += get_time() - tmp_upward_time;

        // continue;
        // tmp_td_dfs_time = get_time();
        TD_DFS(i);
        // td_dfs_time += get_time() - tmp_td_dfs_time;
        //TODO recover upward delta





        // //! recover path




        scan_log[i] = (long long)cnt;

        scan_cnt += (long long)cnt;


#ifdef debug
        printf("~~~~~~~~~~~~print pre %d ~~~~~~~~~~~~~~\n", origin);

        for (int i = 0;i < pre.size();i++) {
            for (int j = 0;j < pre[i].size();j++) {
                printf("%d -> %d sc_id %d\n", i, pre[i][j].first, pre[i][j].second);
            }
            printf("\n");
        }
        printf("~~~~~~~~~~~~end   pre ~~~~~~~~~~~~~~\n");
#endif

        // tmp_BC_time = get_time();
        contract(origin);
        // BC_DFS(ch, origin, -1, origin);//get delta


        BC_DFS2(ch, origin, -1, origin);//add into BC
        // BC_time += get_time() - tmp_BC_time;

        // tmp_td_dfs_time_2 = get_time();
        TD_DFS_2(i);
        // td_dfs_time_2 += get_time() - tmp_td_dfs_time_2;



#ifdef debug
        printf("~~~~~~~~~~print delta %d ~~~~~~~~~~~~~\n", origin);
        for (int i = 0;i < G.n;i++) {
            printf("%d %.6f\n", i, delta[ch][i]);
        }
        printf("~~~~~~~~~~end print delta ~~~~~~~~~~~~~\n");


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
        printf("\nsgm :");
        for (int ii = 0;ii < G.n;ii++) {
            printf("%2d| ", sigma[ch][ii]);
        }
        printf("\nlab :");
        for (int ii = 0;ii < G.n;ii++) {
            printf("%2d| ", lab[tree[i].height - 1][ii]);
        }
        printf("\nok  :");
        for (int ii = 0;ii < G.n;ii++) {
            printf("%2d| ", ok_[tree[i].height - 1][ii]);
        }
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~\n");

#endif

    }//end loop for enumerate origins
    for (int h = tree[last_origin].height - 1; h >= 0;h--) {
        reduce_(tree_chain[h]);
    }

    // tmp_last_time = get_time();
    add_up_cuted_subgraph(tree[root].height - 1, tree[last_origin].height - 1);

    recover_all_edges();
    // last_time += get_time() - tmp_last_time;
    printf("average sort cost %d / %d , %f \n", average_cutted_tree_num, average_cutted_tree_cnt, (double)average_cutted_tree_num / (double)average_cutted_tree_cnt);
}
void Tree_Decomposition::print_neighbor_length() {
    for (int x = 0;x < G.n;x++) {
        printf("x %d \n", x);
        for (int i = 0;i < neighbor[x].size();i++) {
            printf("nei %d len %d \n", neighbor[x][i], length[x][i]);
        }
    }
}
void Tree_Decomposition::print_ops() {

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
            printf("[[i %d pa %d u_v %d|", q2[i], tree[q2[i]].pa, tree[q2[i]].unique_vertex);
            for (int j = 0;j < tree[q2[i]].vert.size();j++) {
                printf("%d,", tree[q2[i]].vert[j]);
            }

            printf("  ]]");
        }
        printf("\n");

    }

}


void Tree_Decomposition::print_rsg() {

}
void Tree_Decomposition::print_scpool() {
    int cnt = 0;
    for (auto it : SG.sc_pool) {
        printf("%2d: u %d v %d cnt %d w %d mid ", cnt++, it.uid, it.vid, it.count, it.weight);
        for (auto mid : it.mid_sc) {
            printf("|mid %d mid u %d mid v %d count %d| ", mid.mid, mid.u_sc.first, mid.v_sc.first, SG.sc_pool[mid.u_sc.second].count * SG.sc_pool[mid.v_sc.second].count);
        }
        printf("\n");
    }
}
void Tree_Decomposition::print_sg() {
    printf("~~~~~~~~~~printf sg ~~~~~~~~~~\n");
    for (int x = 0;x < G.n;x++) {
        for (auto it : SG.E[x]) {
            printf("%d -> %d scid %d \n", x, it.first, it.second);
        }
    }
    printf("~~~~~~~~~~printf sg ~~~~~~~~~~\n");
}
void Tree_Decomposition::upward_search_dij(int i, int ch) {
    stamp++;
    extd_num[i] = 0;
    origin_stamp[tree[i].unique_vertex] = stamp;
    dis[ch][tree[i].unique_vertex] = 0;
    sigma[ch][tree[i].unique_vertex] = 1;
    cnt = 0;
    int origin = tree[i].unique_vertex;

    priority_queue<dij_state> pq;

    int x = tree[i].unique_vertex;
    int p = i;
    pq.push(dij_state(x, 0));

    while (p >= 0) {
        cnt++;
        if (changed_vertices_stamp[ch][tree[p].unique_vertex] != origin) {
            changed_vertices[ch].push_back(tree[p].unique_vertex);
            changed_vertices_stamp[ch][tree[p].unique_vertex] = origin;
        }
        if (searched_vertices_stamp[ch][tree[p].unique_vertex] != origin) {
            searched_vertices[ch].push_back(tree[p].unique_vertex);
            searched_vertices_stamp[ch][tree[p].unique_vertex] = origin;
        }

        in_ancs[tree[p].unique_vertex] = origin;
        p = tree[p].pa;
    }


    while (!pq.empty()) {
        int u = pq.top().u;

        pq.pop();

        if (vis[u] == origin) {
            continue;
        }
        spt_order[dij_ordet_count++] = u;
        vis[u] = origin;

        for (int it = 0;it < SPPG[u].size();it++) {
            scan_cnt_edge++;
            int v = SPPG[u][it].first, sc_id = SPPG[u][it].second;
            int w = SG.sc_pool[sc_id].weight;
            int c = SG.sc_pool[sc_id].count;



            if (c == 0) {


                continue;
            }
            if (dis[ch][v] > dis[ch][u] + w) {
                dis[ch][v] = dis[ch][u] + w;
                pq.push(dij_state(v, dis[ch][v]));
                pre[v].clear();
                pre[v].push_back(make_pair(u, sc_id));
                sigma[ch][v] = sigma[ch][u] * c;
                if (belong[u] != belong[origin]) {
                    if (lab[ch][u] < 0) {
                        lab[ch][v] = u;
                    }
                    else {
                        lab[ch][v] = lab[ch][u];
                    }
                }
                else {
                    lab[ch][v] = v;
                }
            }
            else if (dis[ch][v] == dis[ch][u] + w) {
                pre[v].push_back(make_pair(u, sc_id));
                sigma[ch][v] += sigma[ch][u] * c;
            }
        }

        // for (int it = 0;it < neighbor[u].size();it++) {
        //     dij_cnt++;
        //     if (in_ancs[neighbor[u][it]] != origin) {
        //         dij_skiped_cnt++;
        //         continue;
        //     }
        //     scan_cnt_edge++;
        //     int v = neighbor[u][it], sc_id = length[u][it];
        //     int w = SG.sc_pool[sc_id].weight;
        //     int c = SG.sc_pool[sc_id].count;



        //     if (c == 0) {
        //         dij_skiped_cnt++;

        //         continue;
        //     }
        //     if (dis[ch][v] > dis[ch][u] + w) {
        //         dis[ch][v] = dis[ch][u] + w;
        //         pq.push(dij_state(v, dis[ch][v]));
        //         pre[v].clear();
        //         pre[v].push_back(make_pair(u, sc_id));
        //         sigma[ch][v] = sigma[ch][u] * c;
        //         if (belong[u] != belong[origin]) {
        //             if (lab[ch][u] < 0) {
        //                 lab[ch][v] = u;
        //             }
        //             else {
        //                 lab[ch][v] = lab[ch][u];
        //             }
        //         }
        //         else {
        //             lab[ch][v] = v;
        //         }
        //     }
        //     else if (dis[ch][v] == dis[ch][u] + w) {
        //         pre[v].push_back(make_pair(u, sc_id));
        //         sigma[ch][v] += sigma[ch][u] * c;
        //     }
        // }

        // for (int it = 0;it < fneighbor[u].size();it++) {
        //     dij_cnt++;
        //     if (in_ancs[fneighbor[u][it]] != origin) {
        //         dij_skiped_cnt++;
        //         continue;
        //     }
        //     int v = fneighbor[u][it], sc_id = flength[u][it];
        //     int w = SG.sc_pool[sc_id].weight;
        //     int c = SG.sc_pool[sc_id].count;

        //     if (c == 0) {
        //         dij_skiped_cnt++;
        //         continue;
        //     }
        //     scan_cnt_edge++;
        //     if (dis[ch][v] > dis[ch][u] + w) {
        //         dis[ch][v] = dis[ch][u] + w;
        //         pq.push(dij_state(v, dis[ch][v]));
        //         pre[v].clear();
        //         pre[v].push_back(make_pair(u, sc_id));
        //         sigma[ch][v] = sigma[ch][u] * c;
        //         if (belong[u] != belong[origin]) {
        //             if (lab[ch][u] < 0) {
        //                 lab[ch][v] = u;
        //             }
        //             else {
        //                 lab[ch][v] = lab[ch][u];
        //             }
        //         }
        //         else {
        //             lab[ch][v] = v;
        //         }
        //     }
        //     else if (dis[ch][v] == dis[ch][u] + w) {
        //         pre[v].push_back(make_pair(u, sc_id));
        //         sigma[ch][v] += sigma[ch][u] * c;
        //     }
        // }


    }

}

void Tree_Decomposition::init() {
    int init_flag = 0;


    origin_stamp.resize(G.n + 1);
    BC.resize(G.n + 1);

    extend.clear();
    Get_Subtree_Nodes_();
    int max_h = 0;
    for (int i = G.n; i >= 0; i--) {
        int p = i, h = tree[i].height - 1;
        if (max_h < h)
            max_h = h;
    }

    printf("init flag %d \n", init_flag++);
    dis.resize(max_h + 1);
    lab.resize(max_h + 1);
    comp.resize(max_h + 1);
    cover.resize(max_h + 1);
    changed_vertices.resize(max_h + 1);
    changed_vertices_stamp.resize(max_h + 1);
    BC_DFSed.resize(max_h + 1);
    searched_vertices.resize(max_h + 1);
    searched_vertices_stamp.resize(max_h + 1);
    printf("init flag %d \n", init_flag++);

    delta.resize(max_h + 1);

    SPPG.resize(G.n + 1);

    sigma.resize(max_h + 1);
    spt.resize(max_h + 1);


    cut_cnt.resize(max_h + 1);
    cut_cnt_collection.resize(max_h + 1);
    cut_cnt_collection_collection.resize(max_h + 1);

    // added_cutted_graph.resize(max_h + 1);

    ok_.resize(max_h + 1);
    spt_order.resize(G.n + 1);
    tree_chain.resize(max_h + 1);
    scan_log.resize(G.n);
    vis.resize(G.n + 1);
    in_ancs.resize(G.n + 1);
    printf("init flag %d \n", init_flag++);
    for (int i = 0; i <= max_h; i++) {
        dis[i].resize(G.n + 1);
        lab[i].resize(G.n + 1);
        comp[i].resize(G.n + 1);
        cover[i].resize(G.n + 1);

        changed_vertices[i].reserve(G.n + 1);
        changed_vertices_stamp[i].resize(G.n + 1);

        searched_vertices[i].reserve(G.n + 1);
        searched_vertices_stamp[i].resize(G.n + 1);

        delta[i].resize(G.n + 1);

        sigma[i].resize(G.n + 1);
        BC_DFSed[i].resize(G.n + 1);

        cut_cnt[i].resize(G.n + 1);
        cut_cnt_collection_collection[i].resize(G.n + 1);


        ok_[i].resize(G.n + 1);
        tree_chain[i] = -1;
        for (int j = 0; j < G.n + 1; j++) {
            SPPG[j].clear();
            changed_vertices_stamp[i][j] = -1;
            searched_vertices_stamp[i][j] = -1;
            dis[i][j] = INTMAX;
            lab[i][j] = -1;
            comp[i][j] = 0;
            cover[i][j] = 0;
            delta[i][j] = 0;

            sigma[i][j] = 0;
            BC_DFSed[i][j] = -1;

            cut_cnt[i][j] = 0;
            cut_cnt_collection_collection[i][j] = 0;

            ok_[i][j] = -1;
        }
    }

    release_num.resize(G.n + 1);
    printf("init flag %d \n", init_flag++);
    for (int i = G.n; i >= 0; i--) {
        release_num[i].resize(neighbor[i].size());

        BC[i] = 0;
        in_ancs[i] = -1;
        vis[i] = -1;
        for (int j = 0; j < neighbor[i].size(); j++)
            release_num[i][j] = 0;
    }

    dfs_order.clear();


    get_dfs_order(root, dfs_order);

    extd_num.resize(G.n + 1);

    degree.resize(G.n);
    for (int i = 0;i < G.n;i++) {
        degree[i] = H.E[i].size();
    }
    dfs_tree_of_vertex(root);

    long long total_width = 0ll;
    printf("init flag %d \n", init_flag++);
    //init_delta_of_tree
    for (int i = 0;i < tree.size();i++) {
        tree[i].delta_of_tree.resize(max_h + 1);
        tree[i].from.resize(max_h + 1);

        tree[i].delta_of_sc.resize(max_h + 1);

        total_width += (tree[i].vert.size() + 1);
        for (int j = 0;j < max_h + 1;j++) {

            tree[i].from[j].clear();

            tree[i].delta_of_tree[j].resize(tree[i].vert.size() + 1);
            tree[i].delta_of_sc[j].resize(tree[i].vert.size());
            for (int k = 0;k < tree[i].delta_of_tree[j].size();k++) {
                tree[i].delta_of_tree[j][k] = 0;

            }
            for (int k = 0;k < tree[i].delta_of_sc[j].size();k++) {
                tree[i].delta_of_sc[j][k] = 0;
            }
        }
    }
    printf("average width %f tree height %d \n", (double)total_width / (double)G.n, max_h);
}



void Tree_Decomposition::TD_DFS_2(int i) {
    int origin = tree[i].unique_vertex;


    int p = i;
    int last_child = -1;
    int ch = tree[i].height - 1;
    while (p >= 0) {

        int x = tree[p].unique_vertex;

        for (int i_ = 0;i_ < tree[p].ch.size();i_++) {
            int child = tree[p].ch[i_];
            if (child != last_child) {
                TD_SPT_DFS_2(child, origin);
            }
        }

        last_child = p;
        p = tree[p].pa;
    }
}

void Tree_Decomposition::TD_SPT_DFS_2(int p, int origin) {
    int ch = tree[belong[origin]].height - 1;
    int x = tree[p].unique_vertex;
    int pa = tree[p].pa;
    int label = lab[ch][x];

    if (ok_[ch][x] == 0) {
        for (int i = 0;i < tree[p].ch.size();i++) {
            TD_SPT_DFS_2(tree[p].ch[i], origin);
        }
    }
    for (int i = 0;i < tree[p].from[ch].size();i++) {
        int from = tree[p].vert[tree[p].from[ch][i]];
        tree[p].delta_of_sc[ch][tree[p].from[ch][i]] += (double)sigma[ch][from] / (double)sigma[ch][x] * (1 + tree[p].delta_of_tree[ch][0]);
        tree[pa].delta_of_tree[ch][tree[p].vert_pos_in_ancs[tree[p].from[ch][i]] + 1] += (double)sigma[ch][from] / (double)sigma[ch][x] * (1 + tree[p].delta_of_tree[ch][0]) * SG.sc_pool[length[x][tree[p].from[ch][i]]].count;
    }

    for (int i = 1;i < tree[p].delta_of_tree[ch].size();i++) {
        tree[pa].delta_of_tree[ch][tree[p].vert_pos_in_ancs[i - 1] + 1] += tree[p].delta_of_tree[ch][i];
    }

}

void Tree_Decomposition::contract(int origin) {
    int ch = tree[belong[origin]].height - 1;
    for (int i = dij_ordet_count - 1;i >= 0;i--) {
        int v = spt_order[i];

        for (int j = 0;j < pre[v].size();j++) {
            int u = pre[v][j].first;
            add_into_edges(u, v, sigma[ch][u], sigma[ch][v], pre[v][j].second, origin, (double)sigma[ch][u] / (double)sigma[ch][v] * (1 + delta[ch][v]), 1, 1, SG.sc_pool[pre[v][j].second].count, false);
        }
    }
}
void Tree_Decomposition::BC_DFS(int ch, int origin, int from, int u) {
    if (BC_DFSed[ch][u] == ch) {
        return;
    }
    BC_DFSed[ch][u] = ch;

    for (int i = 0.;i < post[u].size();i++) {
        int v = post[u][i].first;
        BC_DFS(ch, origin, u, v);
    }


    for (int i = 0.;i < post[u].size();i++) {
        int v = post[u][i].first;

        add_into_edges(u, v, sigma[ch][u], sigma[ch][v], post[u][i].second, origin, delta[ch][v], 1, 1, SG.sc_pool[post[u][i].second].count, false);

    }



}
void Tree_Decomposition::BC_DFS2(int ch, int origin, int from, int u) {
    for (auto it : searched_vertices[ch]) {

        if (it != origin) {
            BC[it] += (delta[ch][it]);
        }
    }
}

void Tree_Decomposition::add_into_edges(int u, int v, int sigma_u_origin, int sigma_v_origin, int sc, int origin, double delta_, int u_sp, int v_sp, int origin_count, bool recursive) {
    int ch = tree[belong[origin]].height - 1;

    if (in_ancs[u] == origin && in_ancs[v] == origin) {//这部分的sc会变，直接处理掉


        Shortcut& sc_ = SG.sc_pool[sc];
        for (int i = 0;i < sc_.mid_sc.size();i++) {
            mid_with_sc mid_ = sc_.mid_sc[i];
            int mid = mid_.mid;
            int c = SG.sc_pool[mid_.u_sc.second].count * SG.sc_pool[mid_.v_sc.second].count;

            BC[mid] += (double)c * delta_;
            if (u == mid_.u_sc.first) {
                add_into_edges(u, mid, sigma_u_origin, sigma_v_origin, mid_.u_sc.second, origin, delta_ * SG.sc_pool[mid_.v_sc.second].count, u_sp, v_sp * SG.sc_pool[mid_.v_sc.second].count, origin_count, true);
                add_into_edges(mid, v, sigma_u_origin, sigma_v_origin, mid_.v_sc.second, origin, delta_ * SG.sc_pool[mid_.u_sc.second].count, u_sp * SG.sc_pool[mid_.u_sc.second].count, v_sp, origin_count, true);
                // SG.sc_pool[mid_.u_sc.second].delta += (double)sigma[ch][u] / (double)sigma[ch][v] * (1 + delta_) * v_sp * SG.sc_pool[mid_.v_sc.second].count;
                // SG.sc_pool[mid_.v_sc.second].delta += (double)sigma[ch][u] / (double)sigma[ch][v] * (1 + delta_) * u_sp * SG.sc_pool[mid_.u_sc.second].count;
            }
            else {
                add_into_edges(u, mid, sigma_u_origin, sigma_v_origin, mid_.u_sc.second, origin, delta_ * SG.sc_pool[mid_.v_sc.second].count, u_sp, v_sp * SG.sc_pool[mid_.u_sc.second].count, origin_count, true);
                add_into_edges(mid, v, sigma_u_origin, sigma_v_origin, mid_.v_sc.second, origin, delta_ * SG.sc_pool[mid_.u_sc.second].count, u_sp * SG.sc_pool[mid_.v_sc.second].count, v_sp, origin_count, true);
                // SG.sc_pool[mid_.u_sc.second].delta += (double)sigma[ch][u] / (double)sigma[ch][v] * (1 + delta_) * u_sp * SG.sc_pool[mid_.v_sc.second].count;
                // SG.sc_pool[mid_.v_sc.second].delta += (double)sigma[ch][u] / (double)sigma[ch][v] * (1 + delta_) * v_sp * SG.sc_pool[mid_.u_sc.second].count;
            }
        }


    }
    else {//归一化的delta（每条最短路）
        SG.sc_pool[sc].delta += delta_;
    }
    if (!recursive) {
        delta[ch][u] += delta_ * origin_count;
    }


}

void Tree_Decomposition::recover_all_edges() {

    stack<int> q;
    q.push(root);
    while (!q.empty()) {
        int p = q.top();
        q.pop();
        for (auto it : tree[p].ch) {
            q.push(it);
        }
        int x = tree[p].unique_vertex;
        for (int j = 0;j < neighbor[x].size();j++) {
            recover_shortcut(SG.sc_pool[length[x][j]].uid, SG.sc_pool[length[x][j]].vid, length[x][j], SG.sc_pool[length[x][j]].delta, 1, 1, rational_number(1, 1));
        }
    }
}
void Tree_Decomposition::recover_shortcut(int u, int v, int sc, double delta_, int u_sp, int v_sp, rational_number times) {

    recover_shortcut_cnt++;

    Shortcut& sc_ = SG.sc_pool[sc];
    for (int i = 0;i < sc_.mid_sc.size();i++) {
        mid_with_sc& mid_ = sc_.mid_sc[i];
        int mid = mid_.mid;
        int c = SG.sc_pool[mid_.u_sc.second].count * SG.sc_pool[mid_.v_sc.second].count * u_sp * v_sp;

        BC[mid] += delta_ * c * times;

        if (u == mid_.u_sc.first) {
            SG.sc_pool[mid_.u_sc.second].delta += delta_ * SG.sc_pool[mid_.v_sc.second].count;
            SG.sc_pool[mid_.v_sc.second].delta += delta_ * SG.sc_pool[mid_.u_sc.second].count;
        }
        else {
            SG.sc_pool[mid_.u_sc.second].delta += delta_ * SG.sc_pool[mid_.u_sc.second].count;
            SG.sc_pool[mid_.v_sc.second].delta += delta_ * SG.sc_pool[mid_.v_sc.second].count;
        }
    }

}

void Tree_Decomposition::add_up_cuted_subgraph(int lower_hight, int higher_hight) {


    for (int i = higher_hight;i >= lower_hight;i--) {//from bot to top to get recursive label

        vector<int> collection;
        for (auto it : cut_cnt_collection[i]) {
            collection.push_back(it);
        }

        average_cutted_tree_num += collection.size();
        average_cutted_tree_cnt++;
        sort(collection.begin(), collection.end(), [&](int a, int b) {
            return sort_by_height(a, b, tree, belong);
            });

        for (auto x : collection) {

            for (int j = 0;j < tree[belong[x]].ch.size();j++) {

                int y = tree[tree[belong[x]].ch[j]].unique_vertex;
                cut_cnt[i][y] += cut_cnt[i][x];

                cut_dfs(i, y);
            }



            cut_cnt[i][x] = 0;
        }

    }

}

void Tree_Decomposition::cut_dfs(int h, int x) {
    cut_scan_cnt++;

    if (ok_[h][x] == 1) {//-1 also means cutted
        int label_height = tree[belong[lab[h][x]]].height - 1;
        int x_ = tree[tree[belong[x]].pa].unique_vertex;
        if (!cut_cnt_collection_collection[label_height][x_]) {

            cut_cnt_collection[label_height].push_back(x_);
            cut_cnt_collection_collection[label_height][x_] = 1;
        }

        cut_cnt[label_height][x] += cut_cnt[h][x];


        cut_cnt[h][x] = 0;
        return;
    }
    else if (ok_[h][x] == -1) {

    }


    for (int i = 0;i < tree[belong[x]].ch.size();i++) {
        int child = tree[tree[belong[x]].ch[i]].unique_vertex;
        cut_cnt[h][child] += cut_cnt[h][x];
    }

    for (auto sc : length[x]) {
        for (int i = 0;i < length[x].size();i++) {
            int sc = length[x][i];

            SG.sc_pool[sc].delta += tree[belong[x]].delta_of_sc[h][i] * cut_cnt[h][x];
            tree[belong[x]].delta_of_sc[h][i] = 0;
        }
    }
    BC[x] += delta[h][x] * cut_cnt[h][x];
    cut_cnt[h][x] = 0;
    for (int i = 0;i < tree[belong[x]].ch.size();i++) {
        int child_unique_vertex = tree[tree[belong[x]].ch[i]].unique_vertex;

        cut_dfs(h, tree[tree[belong[x]].ch[i]].unique_vertex);

    }

}


void Tree_Decomposition::dfs_tree_of_vertex(int p) {
    int x = tree[p].unique_vertex;
    for (int i = 0;i < tree[p].vert.size();i++) {
        int v = tree[p].vert[i];
        tree[p].tree_of_vertex.push_back(make_pair(belong[v], length[x][i]));
        tree[belong[v]].tree_of_vertex.push_back(make_pair(p, length[x][i]));
    }
    for (int i = 0;i < tree[p].ch.size();i++) {
        dfs_tree_of_vertex(tree[p].ch[i]);
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

    td_time = get_time();

    reduce();
    printf("start make tree\n");
    make_tree();
    printf("update tree distance\n");
    update_tree_distance();
    td_time = get_time() - td_time;
#ifdef debug
    print_tree();
    print_rsg();
    print_scpool();
    print_ops();
    print_neighbor_length();
#endif

    G = H;
    vector<int> list;
    list.clear();
    list.push_back(root);
    printf("start TD_SPT\n");
    compute_TD_SPT();
}
bool Tree_Decomposition::sort_by_height(int a, int b, const std::vector<Node>& tree, const std::vector<int>& belong) {
    return tree[belong[a]].height < tree[belong[b]].height;
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
    double t_s = get_time();
    brandes.compute_betweenness_centrality();
    double t_t = get_time();
    brandes.t_round = get_time();
    ofstream ofs(argv[2], ios::trunc | ios::out);
    if (ofs.fail()) {
        printf("check out path\n");
    }

    for (int i = 0;i < brandes.G.n;i++) {
        ofs << fixed << setprecision(6) << brandes.BC[i] << "\n";
    }

    brandes.f_time_log.open(argv[3]);
    printf("scan log size %d\n", brandes.scan_log.size());

    for (int i = 0;i < brandes.scan_log.size();i++) {
        brandes.f_time_log << brandes.scan_log[i] << "\n";

    }


    brandes.f_time_log << "\n";

    printf("scan_cnt %lld scan_cnt_edge %lld\n", brandes.scan_cnt, brandes.scan_cnt_edge);
    printf("cut_scan_cnt %lld\n", brandes.cut_scan_cnt);
    printf("recover_shortcut_cnt %lld \n", brandes.recover_shortcut_cnt);
    printf("sg pool size %lld \n", brandes.SG.sc_pool.size());
    cout << argv[1] << "  " << t_t - t_s << "\n";
    printf("n2 time %f \n", brandes.n2_time);
    printf("last_time %f \n", brandes.last_time);
    printf("upward_time %f \n", brandes.upward_time);
    printf("td_dfs_time %f \n", brandes.td_dfs_time);
    printf("bc_time %f \n", brandes.BC_time);
    printf("dfs 2 time %f \n", brandes.td_dfs_time_2);
    printf("reverse time %f \n", brandes.reverse_time);
    printf("add_up_time %f \n", brandes.add_up_time);
    printf("lazy_time %f \n", brandes.lazy_time);
    printf("cut_cnt_time %f \n", brandes.cut_cnt_time);
    printf("dij cnt %lld ,dij skiped cnt %lld\n", brandes.dij_cnt, brandes.dij_skiped_cnt);
    // printf("expand %f reduce %f sort %f \n", brandes.expand_time, brandes.reduce_time, brandes.sort_time);
    return brandes.recursive_lab;
}
