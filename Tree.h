#include<bits/stdc++.h>

#ifndef TREE_H
#define TREE_H

using namespace std;

class Tree
{
    public:
        Tree();
        Tree(int v);
        int total_cost;     // represents total cost of tree
        virtual ~Tree();
        int v;      // number of vertices - 0 to v-1
        vector<vector<pair<int,int>>> adj_list;     // adjacency list for weighted tree
        // adj_list has V vectors and each vector stores the edges in the form <other_vertex, weight>

    protected:

    private:
};

#endif // TREE_H
