#ifndef BFS_HPP
#define BFS_HPP

#include "header.hpp"

void checker(DAG& dag, vector<bool>& visited, priority_queue<int>& pq)
{
    int node;
    while(!pq.empty())
    {
        node = pq.top();
        pq.pop();
        if(!dag.data.empty())
        {    
            for(int neigbhor: dag.data[node])
            {
                if(!visited[neigbhor])
                {
                    pq.push(neigbhor);
                    visited[neigbhor] = true;
                }
            }
        }
        if(!dag.parents.empty())
        {    
            for(int neigbhor: dag.parents[node])
            {
                if(!visited[neigbhor])
                {
                    pq.push(neigbhor);
                    visited[neigbhor] = true;
                }
            }
        }
    }
    return;
}

void parseGraph(DAG& dag, int node, vector<bool>& visited)
{
    priority_queue<int> pq;
    pq.push(1);
    visited[1] = true;
    checker(dag, visited, pq);
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

void connectivityChecker(DAG& dag) 
{
    cout << "Parse graph and note seen nodes" << endl;
    vector<bool> visited(dag.data.size(), 0);
    visited[0] = true;
    parseGraph(dag, 1, visited);
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