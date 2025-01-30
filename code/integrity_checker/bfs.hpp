#ifndef BFS_HPP
#define BFS_HPP

#include "header.hpp"

void bfs(DAG& dag) 
{
    vector<int> visited(NODE_MAX_VALUE, 0);
    for (int i = 0; i < NODE_MAX_VALUE; i++) 
    {
        if (visited[i] == 0) 
        {
            if (dfs(i, dag, visited)) 
            {
                cout << "The graph is not a DAG" << endl;
                return;
            }
        }
    }
    cout << "The graph is a DAG" << endl;
}

#endif