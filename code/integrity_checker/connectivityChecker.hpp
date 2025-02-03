#ifndef BFS_HPP
#define BFS_HPP

#include "header.hpp"

void checker(DAG& dag, int node, vector<int>& visited)
{
    cout << node << endl;
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
    cout << "7" << endl;
    bool fullyConnected;
    vector<int> visited(NODE_MAX_VALUE, 0);
    checker(dag, 1, visited);
    cout << "8" << endl;
    fullyConnected = parseVisited(visited);
    cout << "9" << endl;
    if(fullyConnected == true)
        cout << "graph is fully connected" << endl;
    else
        cout << "graph isn't fully connected" << endl;
    cout << "10" << endl;
    return;
}

#endif