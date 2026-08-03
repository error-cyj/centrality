#include<vector>
using namespace std;


class SelEle{
public:
    int x;
    SelEle();
    SelEle(int _x);
    static vector<int> DD, DD2;
  //  static int a;
    bool operator< (const SelEle se)const;
};
