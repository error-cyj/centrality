#include "SelEle.h"

SelEle::SelEle(int _x)
{
    x = _x;
}

SelEle::SelEle()
{
}


bool SelEle::operator< (const SelEle se)const{
  //  if (DD2[x] != DD2[se.x])
    //    return DD2[x] < DD2[se.x];
    if (DD[x] != DD[se.x])
        return DD[x] < DD[se.x];
    return x < se.x;
}

vector<int> SelEle::DD, SelEle::DD2;
//int SelEle:: a;
