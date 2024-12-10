#include "Graph.h"
using namespace std;
//adding back to git
Graph::Graph()
{
    // empty constructor
}

Graph::Graph(vector<vector<int>> &a){
    adj_mat = a;
    v = a.size();
    e = 0;
    adj_list.assign(v, vector<pair<int,int>>());
    // creating adjacency list from adjacency matrix
    for(int i=0; i<v; i++){
        for(int j=i+1; j<v; j++){
            if(adj_mat[i][j] != -1){
                adj_list[i].push_back({j, adj_mat[i][j]});
                adj_list[j].push_back({i, adj_mat[i][j]});
            }
        }
    }
}

Graph::~Graph()
{
    //dtor
}

// This function computes distance between all pairs of vertices
void Graph::compute_shortest_distances(){
    shortest_dist = adj_mat;
    // make all -1 to INT_MAX in shortest_dist
    for(int i=0; i<shortest_dist.size(); i++){
        for(int j=0; j<shortest_dist[i].size(); j++){
            if(shortest_dist[i][j] == -1)
                shortest_dist[i][j] = INT_MAX;
        }
    }
    int i, j, k;
    for(k=0; k<v; k++){
        for(i=0; i<v; i++){
            for(j=0; j<v; j++){
                if (shortest_dist[i][k] != INT_MAX && shortest_dist[k][j] != INT_MAX && shortest_dist[i][j] > (shortest_dist[i][k] + shortest_dist[k][j]))
                    shortest_dist[i][j] = shortest_dist[i][k] + shortest_dist[k][j];
            }
        }
    }

}


// This function fills the adjacency matrix of complete distance graph on terminals
// It uses all pair shortest distance paths created using compute_shortest_distances()
void Graph::get_adj(vector<vector<int>>& adj_cdg, vector<int>& terminals){
    // assigning the distance between terminals as shortest distance between them
    int s = terminals.size();
    for(int i=0; i<s; i++){
        for(int j=i+1; j<s; j++){
            int p = terminals[i];
            int q = terminals[j];
            int d = shortest_dist[p][q];
            adj_cdg[i][j] = d;
            adj_cdg[j][i] = d;
        }
    }
}

// HELPER functions for MST
// Finds the non-MST vertex with smallest distance
int Graph::minKey(vector<int>& dist, vector<bool> &mstSet)
{
    // Initialize min value
    int min = INT_MAX, min_index;

    for (int p = 0; p < v; p++)
        if (mstSet[p] == false && dist[p] < min)
            min = dist[p], min_index = p;

    return min_index;
}

// This function finds MST of given graph and returns it as tree
// It uses prims's algorithm to find MST
Tree* Graph::get_mst(){
    /*
    cout << "Adjacency list of CDG: " << endl;
    for(int i=0; i<v; i++){
        cout << i << ": ";
        for(int j=0; j<adj_list[i].size(); j++){
            cout << "(" << adj_list[i][j].first << "," << adj_list[i][j].second << "),";
        }
    }
    cout << endl;
    */

    // cout << "Inside get_mast()" << endl;
    // Array to store constructed MST
    vector<int> parent(v, -1);

    // Initial distance of each vertex from the MST - stores min distance from mst
    vector<int> dist(v, INT_MAX);

    // To represent set of vertices included in MST
    vector<bool> mstSet(v, false);

    // Initialize all keys as INFINITE
    for (int i = 0; i < v; i++){
        //dist[i] = INT_MAX;
        //mstSet[i] = false;
        parent[i] = i;
    }


    // Always include first 1st vertex in MST.
    // Make dist 0 so that this vertex is picked as first
    // vertex.
    dist[0] = 0;

    // First node is always root of MST - on;y root has parent -1
    parent[0] = -1;

    Tree *t = new Tree(v);       // creating tree with v vertices

    // The MST will have V vertices - Add one vertex in each iteration
    for (int count = 0; count < v; count++) {

        // Pick the minimum key vertex from the
        // set of vertices not yet included in MST
        int u = minKey(dist, mstSet);

        // Add the picked vertex to the MST Set
        mstSet[u] = true;

        // Add the tree edge that connects u to MST
        if(u != 0){     // need to do only if vertex is not root
            int cost = adj_mat[u][parent[u]];
            t->adj_list[u].push_back({parent[u], cost});
            t->adj_list[parent[u]].push_back({u, cost});
            // updating the tree cost by edge(u, parent[u])
            (t->total_cost) += cost;
            // cout << "Adding " << cost << " to the cost of MST." << t->total_cost << endl;
        }

        // update distance of each neighbor of u
        // cout << "Adding vertex " << u << ":";
        // cout << "Updating neighbors: ";
        for(int i=0; i<adj_list[u].size(); i++){
            int vertex = adj_list[u][i].first;
            int weight = adj_list[u][i].second;
            //cout << "(" << vertex << "," << weight << ")";
            if(mstSet[vertex]==false && weight < dist[vertex]){
                parent[vertex] = u;
                dist[vertex] = weight;
            }
        }
        //cout << endl;
    }
    
    // cout << "Parent array: ";
    // for(int i=0; i<v; i++){
    //     cout << parent[i] << " ";
    // }
    // cout << endl;

    // cout << "Distance array: ";
    // for(int i=0; i<v; i++){
    //     cout << dist[i] << " ";
    // }
    // cout << endl;
    

    // cout << "Adjacency list of MST: " << endl;
    // for(int i=0; i<v; i++){
    //     cout << i << ": ";
    //     for(int j=0; j<t->adj_list[i].size(); j++){
    //         cout << "(" << t->adj_list[i][j].first << "," << t->adj_list[i][j].second << "),";
    //     }
    // }
    // cout << endl;


    return t;
}
