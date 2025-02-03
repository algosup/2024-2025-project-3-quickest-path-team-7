#ifndef BFS_HPP
#define BFS_HPP

#include "header.hpp"

void checker(DAG& dag, int node, vector<int>& visited)
{
    if(visited[node] == 1)
        return;
    visited[node] = 1;
    for(int neighbor: dag.data[node])
        checker(dag, neighbor, visited);
    for(int neighbor: dag.parents[node])
        checker(dag, neighbor, visited);
}

bool parseVisited(vector<int>& visited)
{
    for(int i : visited)
    {
        if(i == 0)
            return false;
    }
    return true;
}

void connectivityChecker(DAG& dag) 
{
    bool fullyConnected;
    vector<int> visited(NODE_MAX_VALUE, 0);
    checker(dag, 0, visited);
    fullyConnected = parseVisited(visited);
    if(fullyConnected == true)
        cout << "graph is fully connected" << endl;
    else
        cout << "graph isn't fully connected" << endl;
}

#endif