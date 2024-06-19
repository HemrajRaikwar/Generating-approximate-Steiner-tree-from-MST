#include <iostream>
#include <cstdlib>
#include <time.h>
#include <bits/stdc++.h>
#include "Graph.h"

using namespace std;

// This function computes the value of b^e
long long int pow(unsigned int b, unsigned int e) {
    if (e == 0) return (long long int)1;
    if (e == 1) return (long long int)b;
    if (e % 2 == 0) {
        return (long long int)pow(b, e/2) * (long long int)pow(b, e/2);
    }
    return (long long int)b * (long long int)pow(b, e - 1);
}

void print_terminals(vector<int>& term){
    cout << "Terminal set: ";
    for(int i=0; i<term.size(); i++){
        cout << term[i] << " ";
    }
    cout << endl;
}

void print_adj(vector<vector<int>> &adj){
    int n = adj.size();
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout << adj[i][j] << " ";
        }
        cout << endl;
    }
}

// this function prints important details about MST passed as parameter
void print_mst_details(Tree* cdg_mst){
    cout << "MST details: " << endl;
    cout << "Number of vertices: " << cdg_mst->v << endl;
    cout << "Cost of MST: " << cdg_mst->total_cost << endl;
    cout << "Adjacency list: " << endl;
    
    int n = cdg_mst->v;
    for(int i=0; i<n; i++){
        cout << i << ":";
        for(int j=0; j<cdg_mst->adj_list[i].size(); j++){
            int u = cdg_mst->adj_list[i][j].first;
            int w = cdg_mst->adj_list[i][j].second;
            cout << "(" << u << "," << w << "), ";
        }
        cout << endl;
    }
    cout << endl;
    
}


long long int max_l(long long int a, long long int b){
    if(a>=b) return a;
    return b;
}


// helper function that performs DFS on the MST and generates a spanning tree of the complete distance graph
void dfs(int src, vector<bool>& visited, vector<bool>& is_terminal, unordered_map<int,int>& terminals_position, Tree *t, Tree* st, int last_terminal, vector<vector<int>>& adj_cdg){
    if(visited[src]) return;
    if(is_terminal[src] && last_terminal!=-1){
        // add an edge from current node to last vis terminal
        int p = terminals_position[src]; // index of src terminal in terminal set
        int q = terminals_position[last_terminal]; // index of last_terminal in terminal set
        int cost = adj_cdg[p][q];   // shortest distance between p and q
        // adding an edge from p to q and q to p in st
        st->adj_list[p].push_back({q, cost});
        st->adj_list[q].push_back({p, cost});
        st->total_cost += cost; 
    }
    
    visited[src] = true;

    if(is_terminal[src])
        last_terminal = src;

    // traverse through adjacency list of src to make recursive calls
    for(int i=0; i<t->adj_list[src].size(); i++){
        int q = t->adj_list[src][i].first;
        dfs(q, visited, is_terminal, terminals_position, t, st, last_terminal, adj_cdg);
    }
}


int main()
{

    srand(time(0));     // Using current time as seed of random generator

    // get the value of N
    int n;
    cout << "Enter number of vertices: ";
    cin >> n;

    int k;
    cout << "Enter value of k (number of distinct edge weights): ";
    cin >> k;

    int s;
    cout << "Enter number of terminals: ";
    cin >> s;

    int num_graphs = 100;
    double total = 0;
    int ccc = 0;
    float largest_factor = INT_MIN;
    float smallest_factor = INT_MAX;

    long long int number_of_possible_graphs = pow(2, n*n-n);
    long long int start_at = pow(2, n);
    //cout << "Number of possible graphs: " << number_of_possible_graphs << endl;
    for(long long int counter=max_l(start_at, LONG_MAX), cnt=0; cnt<num_graphs; counter--){
        ///////////////////////////////////////////////////////
        // PART 1: GENERATING GRAPH
        //////////////////////////////////////////////////////
        auto c = counter;
        vector<vector<int>> adj_mat(n, vector<int>(n, 0));
        for(int i=0; i<n && c!=0; i++){
                adj_mat[i][i] = 0;      // distance with self
            for(int j=i+1; j<n && c!=0; j++){
                if(rand()%9 != 0){
                    // if random number generated is even then add edge between them
                    // randomly generate edge weight between 1 to k
                    int r = 1+rand()%k;
                    adj_mat[i][j] = r;
                    adj_mat[j][i] = r;
                }
                else{
                    // There is no edge between i and j ==> assign -1 as weight
                    // TO DO: DO I NEED TO MAKE THEM something else?????????
                    adj_mat[i][j] = -1;
                    adj_mat[j][i] = -1;
                }
                c /= 2;
            }
        }
        // cout << "Adjacency list of original graph: "<< endl;
        // print_adj(adj_mat);
        // generating graph
        Graph g;
        g = Graph(adj_mat);
        //cout << "GRAPH IS GENERATED!" << endl;

        //////////////////////////////////////////////////////
        // PART 2: MST OF COMPLETE DISTANCE GRAPH ON TERMINALS
        //////////////////////////////////////////////////////
        // randomly choose set of terminals
        set<int> t;
        while(t.size() != s){
            int r = rand()%n;
            t.insert(r);
        }
        vector<int> terminals(t.begin(), t.end()); // converting set to vector 
        //print_terminals(terminals);

        // generating adjacency matrix for terminals only
        vector<vector<int>> adj_cdg(s, vector<int>(s, 0));

        // computing shortest distance between each node
        g.compute_shortest_distances();
        // creating adjacency matrix for complete distance graph on terminal set
        g.get_adj(adj_cdg, terminals);

        // generating complete distance graph using given adjacency matrix
        Graph cdg;
        cdg = Graph(adj_cdg);
        //cout << "Adjacency matrix of CDG: "<< endl;
        // print_adj(adj_cdg);

        // generating MST of cdg
        Tree* cdg_mst = cdg.get_mst();
        // cout << endl;
        // cout << "MST of CDG: " << endl;
        //print_mst_details(cdg_mst);

        /////////////////////////////////////////////////////////
        // PART 3: CREATE MST OF ORIGINAL GRAPH
        /////////////////////////////////////////////////////////
        Tree *mst = g.get_mst();
        // cout << endl;
        // cout << "MST of original graph: " << endl;
        //print_mst_details(mst);


        ///////////////////////////////////////////////////////////
        // PART 4: CREATE SPANNING TREE OF CDG FROM MST OF GRAPH
        ///////////////////////////////////////////////////////////
        vector<bool> visited(n, false);
        // resulting tree
        Tree *st = new Tree(s);
        // store if each vertex is terminal or not
        vector<bool> is_terminal(n, false);
        unordered_map<int,int> terminals_positions;
        for(int i=0; i<terminals.size(); i++){
            is_terminal[terminals[i]] = true;
            terminals_positions[terminals[i]] = i;
        }
        int last_terminal = -1;
        int total_sp_cost = 0;
        dfs(terminals[0], visited, is_terminal, terminals_positions, mst, st, last_terminal, adj_cdg);
        // printing the spanning tree cost
        // cout << endl;
        // cout << "Spanning tree of CDG: " << endl;
        // print_mst_details(st);

        ///////////////////////////////////////////////////////////
        // PART 5: COMPARING MST OF CDF AND SPANNING TREE OF CDG (created from MST of graph)
        ///////////////////////////////////////////////////////////
        float n1 = cdg_mst->total_cost;
        float n2 = st->total_cost;
        if(n2){
            cout << "Graph " << cnt;
            cnt++;
            float factor = (float)n2/(float)n1;
            cout << cdg_mst->total_cost << "\t\t" << st->total_cost << "\t\t" << factor << endl;
            ccc++;
            if(largest_factor < factor) largest_factor = factor;
            if(smallest_factor > factor) smallest_factor = factor;
            total += factor;
        }
        //cout << "************************************************************" << endl;
    }
    cout << "************************************************************" << endl;
    cout << "FINAL RESULTS:" << endl;
    cout << "LARGEST FACTOR  -> " << largest_factor << endl;
    cout << "SMALLEST FACTOR -> " << smallest_factor << endl;
    cout << "AVERAGE FACTOR  -> " << total/(float)ccc << endl;
    cout << "************************************************************" << endl;
    //cout << total/num_graphs;
    return 0;
}
