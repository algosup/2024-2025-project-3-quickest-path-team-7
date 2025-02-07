/*
dag.hpp builds the DAG from the CSV file.
To build the DAG, it gathers the DAG structure with data and then modifies it to be free of cycles. 

The CSV file has this format: nodeA,nodeB,distance
To build a DAG we assume that you can only go from A to B. 
So every nodeA will have a vector field with the nodeB you can go to. 
And every nodeB will have a vector field with the nodeA you can come from.

Then we use a DFS to spot all cycles in the DAG.

when a cycle is detected,

node where the cycle is detected: 
    node 1 -> node 2 -> node 1

    node 1 has node 2 as its parent
    node 2 has node 1 as its son

    Change node 2 as son of node 1 and node 1 as parent of node 2

e.g. of output:
    node 1 -> node 2 <- node 1
    
    node 1 has node 2 as its son
    node 2 has node 1 as its parent
*/
#ifndef DAG_HPP
#define DAG_HPP

#include "header.hpp"

struct DAG 
{
    vector<vector<int>> data;
    vector<vector<int>> parents;
};

void pushOut(DAG& dag, int indexListElem, int elementOut, string conditionDataParent)
{
    /*
    pushOut makes it easier to erase a specific data from a DAG
    */
    int k = 0;
    if (conditionDataParent == "data")
    {
        for (int elementInList : dag.parents[indexListElem])
        {
            if (elementInList == elementOut)
            {
                dag.parents[indexListElem].erase(dag.parents[indexListElem].begin()+k);
            }
            k++;
        }
    }
    if (conditionDataParent == "parents")
    {
        for (int elementInList : dag.data[indexListElem])
        {
            if (elementInList == elementOut)
            {
                dag.data[indexListElem].erase(dag.data[indexListElem].begin()+k);
            }
            k++;
        }
    }
}

void loadDag(DAG& dag) 
{
    /*
    loadDag gather dag.data, a vector of DAG, with the CSV data
    */
    ifstream file(DATASET);
    string line;
    unordered_map<int, vector<int>> data;
    while (getline(file, line)) 
    {
        stringstream ss(line);
        string elemInList;
        vector<int> elemList;
        while (getline(ss, elemInList, ',')) 
        {
            elemList.push_back(stoi(elemInList));
        }
        if(elemList[0] != elemList[1])
        {
            data[elemList[0]].push_back(elemList[1]);
        }
    }
    dag.data.resize(NODE_MAX_VALUE);
    for (const auto& [node, neighbors] : data) 
    {
        dag.data[node] = neighbors;
    }
    file.close();
}

void loadParents(DAG& dag) 
{
    /*
    loadParents gather dag.parents, a vector of DAG, with the data of dag.data 
    */
    dag.parents.resize(NODE_MAX_VALUE);
    for (int i = 0; i < NODE_MAX_VALUE; i++) 
    {
        for (const auto& neighbor : dag.data[i]) 
        {
            dag.parents[neighbor].push_back(i);
        }
    } 
}

bool dfs(int node, DAG& dag, vector<int>& visited, int visitedNode) 
{
    /*
    dfs travel through dag, a DAG, and rewrite the data when a cycle is detected (example in the top comment)
    */
    visited[node] = visited[node]+1;
    if (dag.data[node].empty()) 
    {
        visited[node] = dag.parents[node].size()+1;
        return false;
    }
    for (int neighbor : dag.data[node]) 
    {
        if (visited[neighbor] == dag.parents[node].size() and dag.parents[node].size() != 0)
        { 
            dag.data[node].push_back(visitedNode);
            pushOut(dag, node, visitedNode, "data");
            pushOut(dag, visitedNode, visitedNode, "parents");
            dag.parents[visitedNode].push_back(node);
            return false;  
        }
        if (visited[neighbor] == 0 and dfs(neighbor, dag, visited, node)) 
        {   
            dag.data[node].push_back(visitedNode);
            pushOut(dag, node, visitedNode, "data");
            pushOut(dag, visitedNode, visitedNode, "parents");
            dag.parents[visitedNode].push_back(node);
            return false;
        }
    }
    visited[node] = dag.parents[node].size()+1;
    return false;
}

void buildDag(DAG& dag) 
{
    cout << "load nodes sons" << endl;
    loadDag(dag);
    cout << "load nodes parents" << endl;
    loadParents(dag);
    int n = dag.data.size();
    vector<int> visited(n, 0);
    cout << "check for cycles and fixes them if possible" << endl;
    for (int i = 1; i < n; ++i) 
    {
        if (visited[i] == 0) 
        {  
            if (dfs(i, dag, visited, 0)) 
            {
                cout << "The graph is free of cycle!" << endl;
                visited.clear();
                vector<int> visited(dag.data.size(), 0);
            }
        }
    }
    cout << "The graph is acyclic (DAG)." << endl;
    cout << "DAG end" << endl;
    return;
}

#endif