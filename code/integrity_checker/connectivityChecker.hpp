#ifndef BFS_HPP
#define BFS_HPP

#include "header.hpp"

void checker(DAG& dag, int node, vector<bool>& visited)
{
    cout << node << endl;
    visited[node] = true;
    if(not dag.parents[node].empty())
    {
        for(int neigbhour: dag.parents[node])
        {
            if (1<=neigbhour<=NODE_MAX_VALUE) 
            {
                    if(not visited[neigbhour])
                {
                    checker(dag, neigbhour, visited);
                }
            }
        }
    }
    if(not dag.data[node].empty())
    {
        for(int neigbhour: dag.data[node])
        {
            if (1<=neigbhour<=NODE_MAX_VALUE)
            {
                    if(not visited[neigbhour])
                {
                    checker(dag, neigbhour, visited);
                }
            }
        }
    }
}

void parseVisited(vector<bool>& visited, bool& good)
{
    int k = 0;
    for(bool i : visited)
    {
        k++;
        if(not i) 
        {
            cout << "Disconnected : " << k << endl;
            good = false;
        }
    }
    return;
}
void connectivityChecker(DAG& dag) 
{
    cout << "7" << endl;
    vector<bool> visited(dag.data.size(), 0);
    visited[0] = true;
    checker(dag, 1, visited);
    cout << "8" << endl;
    bool good = true;
    parseVisited(visited, good);
    cout << "9" << endl;
    if(good)
        cout << "graph is fully connected" << endl;
    else
        cout << "graph isn't fully connected" << endl;
    cout << "10" << endl;
    return;
}

#endif