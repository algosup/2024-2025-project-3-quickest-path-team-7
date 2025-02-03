#ifndef BFS_HPP
#define BFS_HPP

#include "header.hpp"

int checker(DAG& dag, int node)
{
    return 0;
}

void connectivityChecker(DAG& dag) 
{
    vector<int> visited(NODE_MAX_VALUE, 0);
    checker(dag, 0);
}

#endif