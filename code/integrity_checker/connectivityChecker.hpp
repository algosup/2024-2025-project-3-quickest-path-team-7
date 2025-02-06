#ifndef BFS_HPP
#define BFS_HPP

#include "header.hpp"

void checker(Graph& graph, int node, vector<bool>& visited, vector<int>& pq)
{
    visited[node] = true;
    for(int neigbhor: graph.data[node])
    {
        if(!visited[neigbhor] and (count(pq.begin(), pq.end(), neigbhor) == 0))
        {
            pq.push_back(neigbhor);
        }
    }
    return;
}

void parseGraph(Graph& graph, int node, vector<bool>& visited)
{
    int k = 0;
    vector<int> pq;
    k++;
    checker(graph, 1, visited, pq);
    while(!pq.empty())
    {
        k++;
        if (k % 1000000 == 0) 
        {
            cout << k << endl;
        }
        checker(graph, pq[0], visited, pq);
        pq.erase(pq.begin());
    }
    return;
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

void connectivityChecker(Graph& graph) 
{
    cout << "Parse graph and note seen nodes" << endl;
    vector<bool> visited(graph.data.size(), 0);
    visited[0] = true;
    parseGraph(graph, 1, visited);
    cout << "check if all nodes have been seen" << endl;
    bool good = true;
    parseVisited(visited, good);
    if(good)
        cout << "graph is fully connected" << endl;
    else
        cout << "graph isn't fully connected" << endl;
    cout << "Connectivity check end" << endl;
    return;
}

#endif