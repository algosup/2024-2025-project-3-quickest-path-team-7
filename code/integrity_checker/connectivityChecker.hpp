#ifndef BFS_HPP
#define BFS_HPP

#include "header.hpp"

void checker(Graph& graph, vector<bool>& visited, priority_queue<int>& pq)
{
    int k = 1;
    int node;
    while(!pq.empty())
    {
        node = pq.top();
        pq.pop();
        k++;
        if (k % 1000000 == 0) 
        {
            cout << k << endl;
        }
        for(int neigbhor: graph.data[node])
        {
            if(!visited[neigbhor])
            {
                pq.push(neigbhor);
                visited[neigbhor] = true;
            }
        }
    }
    return;
}

void parseGraph(Graph& graph, int node, vector<bool>& visited)
{
    
    priority_queue<int> pq;
    visited[1] = true;
    pq.push(1);
    checker(graph, visited, pq);
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