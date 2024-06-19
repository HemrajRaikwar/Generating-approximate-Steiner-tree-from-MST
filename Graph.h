#ifndef GRAPH_H
#define GRAPH_H
#include <bits/stdc++.h>
#include "Tree.h"
using namespace std;

class Graph
{
    public:
        // destructor
        virtual ~Graph(void);

        // properties
        int v;  // number of vertices
        int e;  // number of edges
        vector<vector<pair<int,int>>> adj_list;        // adjacency list of graph - each node contains the edges in the form of pair (node, weight)
        vector<vector<int>> adj_mat;                    // adjacency matrix of graph
        vector<vector<int>> shortest_dist;              // this variable stores the shortest distance of each node with every other node

        // constructors
        Graph(vector<vector<int>> &a);  // constructor to create graph with adjacency matrix
        Graph();

        // helper functions
        void compute_shortest_distances();
        void get_adj(vector<vector<int>>& adj_cdg, vector<int>& terminals);
        int minKey(vector<int>& dist, vector<bool> &mstSet);  // helper function for MST
        Tree* get_mst();     // generates MST and returns it as tree


    protected:

    private:
};

#endif // GRAPH_H
