#ifndef BFS_HPP
#define BFS_HPP

#include "header.hpp"

void checker(DAG& dag, int node, vector<int>& visited)
{
    if(dag.data.size() >= node or dag.parents.size() >= node);
        if(visited[node] == 1)
            return;
    visited[node] = 1;
    if(dag.parents.size() >= node)
    {
        if(dag.data.size()>0)
        {
            for(int neighbor: dag.data[node])
            {
                if(visited[neighbor]==0)
                    checker(dag, neighbor, visited);
            }
        }
    }
    if(dag.data.size() >= node)
    {
        if(dag.parents.size()>0)
        {
            for(int neighbor: dag.parents[node])
            {
                if(visited[neighbor]==0)
                    checker(dag, neighbor, visited);
            }
        }
    }
    return;
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
    visited[0] = 1;
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