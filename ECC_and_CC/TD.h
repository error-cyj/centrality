#include "input.h"

#include "SelEle.h"
#include "int_undirected_graph.h"

#include <list>
#include <stdexcept>
const int INTMAX = 999999999;
struct mid_with_sc;
class Shortcut;



template <class T>
inline void hash_combine(size_t& seed, const T& val) {
    seed ^= hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

class rational_number {

public:
    int a; // 分子
    int b; // 分母

    static int gcd(int m, int n) {
        if (n == 0) return m;
        return gcd(n, m % n);
    }
    void reduce() {
        if (b == 0) throw std::invalid_argument("Denominator cannot be zero!");
        if (a == 0) {
            b = 1;
            return;
        }

        int g = gcd(std::abs(a), std::abs(b));
        a /= g;
        b /= g;

        if (b < 0) {
            a = -a;
            b = -b;
        }
    }


    rational_number(int numerator = 0, int denominator = 1)
        : a(numerator), b(denominator)
    {
        reduce();
    }
    rational_number operator*(const rational_number& other) const {
        return rational_number(a * other.a, b * other.b);
    }

    rational_number operator*(int num) const {
        return rational_number(a * num, b);
    }
    friend rational_number operator*(int num, const rational_number& r) {
        return r * num;
    }
    double operator*(double num) const {
        return static_cast<double>(a) / b * num;
    }
    friend double operator*(double num, const rational_number& r) {
        return r * num;
    }
    bool operator==(const rational_number& other) const {
        return (a == other.a) && (b == other.b);
    }
    bool operator!=(const rational_number& other) const {
        return !(*this == other);
    }
    rational_number operator+(const rational_number& other) const {
        int new_a = a * other.b + other.a * b;
        int new_b = b * other.b;
        return rational_number(new_a, new_b);
    }
    rational_number operator+(int num) const {
        return *this + rational_number(num);
    }
    friend rational_number operator+(int num, const rational_number& r) {
        return r + num;
    }
    rational_number& operator+=(const rational_number& other) {
        *this = *this + other;
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const rational_number& r) {
        if (r.b == 1) os << r.a;
        else os << r.a << "/" << r.b;
        return os;
    }
};

class Shortcut {
public:
    int weight, shortest_weight, count, uid, vid;  //vid = vertex id ; eno= edge number
    int sc_id;
    int removed_cnt;
    double delta;
    vector<mid_with_sc> mid_sc;

    vector<pair<int, int> > L, R;
    bool real = false;
    Shortcut();
    Shortcut(int w, int _u, int _v);

    void merge(Shortcut const& sc);//already exist
    Shortcut operator + (const Shortcut& sc) const;//dont exist

};

struct mid_with_sc {
    int mid;
    pair<int, int> u_sc, v_sc;
};

class Shortcut_Graph : public Int_Undirected_Graph {
public:

    vector< map<int, Shortcut> > E;//E[u][v]的值就是sc_pool中的第几个shortcut
    // vector< vector< pair<int, Shortcut> > > Edge;
    vector<Shortcut> sc_pool;
    Shortcut_Graph();
    Shortcut_Graph(Int_Undirected_Graph g);
    void insert_edge(int u, int v, Shortcut k);
    void delete_edge(int u, int v);
    bool is_edge_existed(int u, int v);

};

struct Node {
    vector<int> vert, pos, pos2, dis;
    vector < Shortcut>leng;
    vector<int> ch;

    vector<pair<int, int>> tree_of_vertex;
    vector<int> vert_pos_in_ancs;
    vector<vector<int>> from;

    unordered_map<int, int> vert_pos_map;//self's vert -> pos
    vector<vector<double>> delta_of_tree;//ch * vert size
    vector<vector<double>> delta_of_sc;//ch * vert size
    int height;
    int pa;
    int unique_vertex;
    Node();
};


class Tree_Decomposition {
public:
    //! lazy recover

    vector<vector<int>> changed_vertices;
    vector<vector<int>> changed_vertices_stamp;

    vector<vector<int>> searched_vertices;
    vector<vector<int>> searched_vertices_stamp;
    int err_cnt = 0;
    vector<vector<int>> scaned_vertices;
    ifstream f_scaned_vertices;
    int last_origin = -1;

    //! time
    double n2_time = 0, tmp_n2time;
    double last_time = 0, tmp_last_time;
    double td_time;
    double up_search = 0, tmp_up_search;
    double init_time = 0, tmp_init_time;
    double dfs_time = 0, tmp_dfs_time;
    double extend_time = 0, tmp_extend_time;
    double add_up_time = 0, tmp_add_up_time;
    double recover_time = 0, tmp_recover_time;
    double reverse_time = 0, tmp_reverse_time;
    double BC_time = 0, tmp_BC_time;
    double recover_data_structure = 0, tmp_recover_data_structure;

    double cut_cnt_time = 0, tmp_cut_cnt_time;
    double td_dfs_time = 0, tmp_td_dfs_time;
    double td_dfs_time_2 = 0, tmp_td_dfs_time_2;
    double spt_time = 0, tmp_spt_time;
    double upward_time = 0, tmp_upward_time;
    double lazy_time = 0, tmp_lazy_time;

    // double reduce_time = 0, tmp_reduce_time;
    // double expand_time = 0, tmp_expand_time;
    double sort_time = 0, tmp_sort_time;
    vector<long long> scan_log;
    ofstream f_time_log;

    //!universal
    int root, cnt, cv;  // cv=current vertex

    int f_cnt, r_cnt, fake_cnt;
    int min_scan;
    long long total_min_scan = 0ll;
    vector<int> degree;
    int expend_cnt = 0;
    long long total_expend = 0ll;
    long long total_check = 0ll;
    long long scan_cnt = 0ll;
    long long cut_scan_cnt = 0ll;
    int cuts;
    long long total_cuts = 0;
    long long recover_shortcut_cnt = 0ll;

    long long f_cnt_ = 0ll, r_cnt_ = 0ll, fake_cnt_ = 0ll;
    long long dij_cnt = 0ll, dij_skiped_cnt = 0ll;
    long long scan_cnt_edge = 0ll;
    vector<int> fake_flag;
    ofstream f_scan_cnt;

    vector<int> neigh;
    vector<int> leng;//sc 在shortcut pool 第几个位置
    vector<int> dfs_order;
    double t_round;
    ofstream f_subgraph;
    vector<pair<pair<int, int>, int>> subgraph_edge;
    set<int> allnodes;
    map<int, int> reorder;
    int remap(int a);
    void cutgraph(int low, int high);

    void cutdfs(int p);

    vector<Node> tree;

    Int_Undirected_Graph G, H;
    Shortcut_Graph SG, RSG;
    set<SelEle> deg;
    vector<int> ord, belong, rank;
    vector<vector<Shortcut> > length, flength;
    vector<vector<int> > sigma_bar, f_sigma_bar;

    vector<vector<int> > neighbor, fneighbor;

    int stamp = 0;
    vector<vector<int> > dis, lab;//origin_origin 的高度 -> 被访问的点
    vector<int> origin_stamp;//顶点号-> 遍历顺序

    vector<vector<int>> cover;
    set< pair<int, int > >  extend;
    vector<vector<int> > release_num;
    vector<vector<int> > comp;

    //!ECC
    vector<int> num, cnt_num, max_dis;

    vector<vector<int>> pecc;
    vector<int> search_order;





    //!CC
    void Get_Subtree_Nodes_();
    int Get_Subtree_Nodes(int p);
    vector<vector<long long>> psum;//sum of distance of vertices which is in its subtree 
    vector<long long> sum;
    vector<int> subtree_nodes;//number of tree

    //!BC
    int average_cutted_tree_num = 0, average_cutted_tree_cnt = 0;
    vector<int> in_ancs;
    vector<int> ancs;
    struct PairHash {
        size_t operator()(const pair<int, int>& p) const {
            size_t seed = 0;
            hash_combine<int>(seed, p.first);
            hash_combine<int>(seed, p.second);
            return seed;
        }
    };
    struct dij_state {
        int u, dis;
        dij_state(int u, int dis) :u(u), dis(dis) {}
        bool operator < (const dij_state& other)const {
            return dis > other.dis;
        }
    };
    int dij_ordet_count;
    vector<int> spt_order;
    int last_height;

    vector<double> BC;
    vector<vector<double>> delta;

    vector<vector<int>> sigma;

    vector<vector<int>> ok_;
    vector<int> tree_chain;

    vector<vector<pair<int, int>>> pre;

    vector<vector<pair<int, int>>> post;

    vector<vector<vector<int>>> spt;


    // vector<vector<int>> recursive_label;//ch -> x 's label (path compress)
    // vector<vector<int>> compact_label_collection;
    // vector<unordered_set<int>> cut_cnt_collection;
    vector<vector<int>>cut_cnt_collection;
    vector<vector<int>>cut_cnt_collection_collection;
    // vector<vector<rational_number>> cut_cnt;
    vector<vector<int>> cut_cnt;


    unordered_set < pair<int, int>, PairHash>subtree_recovered;
    // vector<vector<int>> cutted;
    // vector<vector<int>> cutted_collection;


    vector<vector<int>> BC_DFSed;
    // vector<vector<int>> added_cutted_graph;


    void recover_dfs(int ch, int origin, int from, int u);
    void edge_recover(int ch, int origin, int u, int v, Shortcut& sc);
    void BC_DFS(int ch, int origin, int from, int u);
    void contract(int origin);
    void BC_DFS2(int ch, int origin, int from, int u);
    // void recover_shortcut(int u, int v, int sc, int origin, double delta_, int sigma_, bool additive, int alpha);
    void add_into_edges(int u, int v, int sigma_u_origin, int sigma_v_origin, int sc, int origin, double delta_, int u_sp, int v_sp, int origin_count, bool recursive);

    void dfs_tree_of_vertex(int p);
    void recover_all_edges();
    void recover_shortcut(int u, int v, int sc, double delta_, int u_sp, int v_sp, rational_number times);
    void add_up_cuted_subgraph(int lower_hight, int higher_hight);
    void cut_dfs(int h, int x);
    void compute_sigma_bar();
    unordered_set<pair<int, int>, PairHash> recovered;

    void recover_chain(int lower_height, int higher_height);


    void init();
    void get_search_order();
    void print_tree();
    void print_rsg();
    void print_scpool();
    void print_ops();
    void print_neighbor_length();
    void print_sg();
    void compute_betweenness_centrality();
    void compute_descendant(int x);
    void reduce();
    void reduce_(int x);
    void expand(int x);
    int match(int x, vector<int>& neigh);
    void make_tree();
    void update_tree_distance();
    void update_dfs(int p);

    void upward_search(int i, int ch);
    // void upward_search_count(int i, int ch);
    void upward_search_dij(int i, int ch);
    vector<int> vis;
    void TD_DFS(int i);
    void TD_SPT_DFS(int p, int origin);

    void TD_DFS_2(int i);
    void TD_SPT_DFS_2(int p, int origin);

    void compute_TD_SPT();
    void TD_LAUNCH_DFS(int p, int origin);


    void compute_TD_SPT_2();
    void extend_node(int p, int origin, int origin_origin);

    void get_dfs_order(int p, vector<int>& a);
    static bool sort_by_height(int a, int b, const std::vector<Node>& tree, const std::vector<int>& belong);
};

