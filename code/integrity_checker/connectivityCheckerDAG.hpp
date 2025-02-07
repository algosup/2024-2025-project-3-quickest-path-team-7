/*
connectivityCheckerDAG.hpp parses the DAG and checks if all nodes are connected. 
It uses a priority queue to check all the nodes. 
It also checks if all nodes have been visited. 
If not, it prints the disconnected node and the graph isn't fully connected. 
Otherwise, it prints that the graph is fully connected.
*/
#ifndef BFS_HPP
#define BFS_HPP

#include "header.hpp"

void checker(DAG& dag, vector<bool>& visited, priority_queue<int>& pq)
{
    /*
    checker checks all the nodes connected to the node at the top of the priority queue, 
    put them in the priority queue if they are marked as false in visited and then marks them as true in visited.
    */
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
    /*
    parseGraph parses the graph by starting at node 1 and checking all the nodes connected to this node.
    every seen node is marked as true in visited.
    */
    priority_queue<int> pq;
    pq.push(1);
    visited[1] = true;
    checker(dag, visited, pq);
    return;
}

void parseVisited(vector<bool>& visited, bool& good)
{
    /*
    parseVisited checks if all nodes have been visited by searching for false values in visited.
    It returns the disconnected node and a false value if one is found.
    Otherwise, it returns a true value.
    */
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