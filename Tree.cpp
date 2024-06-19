#include "Tree.h"

Tree::Tree()
{
    //ctor
}

Tree::Tree(int a){
    v = a;      // Number of vertices in tree
    total_cost = 0;
    adj_list.assign(v, vector<pair<int,int>>());  // assigning empty list to each vertex of tree
}

Tree::~Tree()
{
    //dtor
}
