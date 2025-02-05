#ifndef BFS_HPP
#define BFS_HPP

#include "header.hpp"

void checker(DAG& dag, int node, vector<int>& visited)
{
    visited[node] = 1;
    if(not dag.parents[node].empty())
    {
        for(int neigbhour: dag.parents[node])
        {
            if(visited[neigbhour] == 0)
            {
                checker(dag, neigbhour, visited);
            }
        }
    }
    if(not dag.data[node].empty())
    {
        for(int neigbhour: dag.data[node])
        {
            if(visited[neigbhour] == 0)
            {
                checker(dag, neigbhour, visited);
            }
        }
    }
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
    vector<int> visited(dag.data.size(), 0);
    visited[0] = 1;
    checker(dag, 20000000, visited);
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