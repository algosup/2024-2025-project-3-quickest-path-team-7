#ifndef DAG_HPP
#define DAG_HPP

#include "header.hpp"

void loadDag(DAG& dag) 
{
    ifstream file(DATASET);
    string line;
    unordered_map<int, vector<int>> data;
    while (getline(file, line)) 
    {
        stringstream ss(line);
        string token;
        vector<int> tokens;
        while (getline(ss, token, ',')) 
        {
            tokens.push_back(stoi(token));
        }
        data[tokens[0]].push_back(tokens[1]);
        
    }
    dag.data.resize(NODE_MAX_VALUE);
    for (const auto& [node, neighbors] : data) 
    {
        dag.data[node] = neighbors;
    }
    file.close();
    cout << "dag data loaded" << endl;
}

void loadParents(DAG& dag) 
{
    cout << "2.2" << endl;
    dag.parents.resize(NODE_MAX_VALUE);
    cout << "2.3" << endl;
    for (int i = 0; i < NODE_MAX_VALUE; i++) 
    {
        for (const auto& neighbor : dag.data[i]) 
        {
            dag.parents[neighbor].push_back(i);
        }
    } 
    cout << "2.4" << endl;
    cout << "dag parents loaded" << endl;
}

void afficheData(DAG& dag) 
{
    for (int i = 0; i < NODE_MAX_VALUE; i++) 
    {
        cout << "Node " << i << " neighbors are " << endl;
        if(dag.data[i].empty())
        {
            cout << "No neighbors" << endl;
            continue;
        }
        for (const auto& neighbor : dag.data[i]) 
        {
            cout << neighbor << endl;
        }
    }
}

void afficheParents(DAG& dag) 
{
    for (int i = 0; i < NODE_MAX_VALUE; i++) 
    {
        cout << "Node " << i << " parents are " << endl;
        if(dag.parents[i].empty())
        {
            cout << "No parents" << endl;
            continue;
        }
        for (const auto& parent : dag.parents[i]) 
        {
            cout << parent << endl;
        }
    }
}

bool dfs(int node, DAG& dag, vector<int>& visited) 
{
    //cout << "DFS on node " << node << endl;
    visited[node] = visited[node]+1;  // Mark the node as visiting

    // Explore all neighbors
    if (dag.data[node].empty()) 
    {
        visited[node] = dag.parents[node].size()+1;  // Mark the node as fully processed
        return false;
    }
    for (int neighbor : dag.data[node]) 
    {
        if (visited[neighbor] == dag.parents[node].size() and dag.parents[node].size() != 0) 
        {  // A cycle is detected
            cout << "Cycle detected in node" << node << endl;
            return true;
        }
        if (visited[neighbor] == 0 && dfs(neighbor, dag, visited)) 
        {
            cout << "Cycle detected in node" << node << endl;
            return true;
        }
    }

    visited[node] = dag.parents[node].size()+1;  // Mark the node as fully processed
    return false;
}

void buildDag(DAG& dag) 
{
    cout << "1" << endl;
    loadDag(dag);
    cout << "2" << endl;
    loadParents(dag);
    cout << "3" << endl;
    //afficheData(dag);
    //afficheParents(dag);
    cout << "DAG loaded" << endl;
    // To track the state of the nodes during DFS (0 = unvisited, 1 = visiting, 2 = visited)
    int n = dag.data.size();
    cout << "4" << endl;
    unsigned int counter = 0;
    unsigned int progression = 0;
    unsigned int progression_backup = 0;
    vector<int> visited(n, 0);  // 0 = unvisited, 1 = visiting, 2 = visited
    cout << "5" << endl;
    // Check for cycles in the graph
    for (int i = 1; i < n; ++i) 
    {
        counter++;
        progression = (counter / CSV_LINES)*100;
        if (progression != progression_backup) 
        {
            cout << "\rDag checked at ... " << progression << " %" << flush;
            progression_backup = progression;
        }
        if (visited[i] == 0) 
        {  // If the node is not visited
            if (dfs(i, dag, visited)) 
            {
                cout << "The graph contains a cycle!" << endl;
                visited.clear();
                vector<int> visited(dag.data.size(), 0);
                return;
            }
        }
    }
    cout << "6" << endl;
    cout << "The graph is acyclic (DAG)." << endl;
    //cin.get();
    dag.data.clear();
    dag.data.shrink_to_fit();
    dag.parents.clear();
    dag.parents.shrink_to_fit();
}

#endif