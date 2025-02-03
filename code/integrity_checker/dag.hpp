#ifndef DAG_HPP
#define DAG_HPP

#include "header.hpp"

void push_out(DAG& dag, int indexListElem, int elementOut, string conditionDataParent)
{
    int k = 0;
    if (conditionDataParent == "data")
    {
        for (int elementInList : dag.parents[indexListElem])
        {
            k++;
            if (elementInList == elementOut)
            {
                dag.parents[indexListElem].erase(dag.parents[indexListElem].begin()+k);
            }
        }
    }
    if (conditionDataParent == "parents")
    {
        for (int elementInList : dag.data[indexListElem])
        {
            k++;
            if (elementInList == elementOut)
            {
                dag.data[indexListElem].erase(dag.data[indexListElem].begin()+k);
            }
        }
    }
}

void loadDag(DAG& dag) 
{
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
        data[elemList[0]].push_back(elemList[1]);
        
    }
    dag.data.resize(NODE_MAX_VALUE);
    for (const auto& [node, neighbors] : data) 
    {
        dag.data[node] = neighbors;
    }
    file.close();
    cout << "dag data loaded" << endl;
}

void loadParents(DAG& dag) 
{
    cout << "2.2" << endl;
    dag.parents.resize(NODE_MAX_VALUE);
    cout << "2.3" << endl;
    for (int i = 0; i < NODE_MAX_VALUE; i++) 
    {
        for (const auto& neighbor : dag.data[i]) 
        {
            dag.parents[neighbor].push_back(i);
        }
    } 
    cout << "2.4" << endl;
    cout << "dag parents loaded" << endl;
}

void afficheData(DAG& dag) 
{
    for (int i = 0; i < NODE_MAX_VALUE; i++) 
    {
        cout << "Node " << i << " neighbors are " << endl;
        if(dag.data[i].empty())
        {
            cout << "No neighbors" << endl;
            continue;
        }
        for (const auto& neighbor : dag.data[i]) 
        {
            cout << neighbor << endl;
        }
    }
}

void afficheParents(DAG& dag) 
{
    for (int i = 0; i < NODE_MAX_VALUE; i++) 
    {
        cout << "Node " << i << " parents are " << endl;
        if(dag.parents[i].empty())
        {
            cout << "No parents" << endl;
            continue;
        }
        for (const auto& parent : dag.parents[i]) 
        {
            cout << parent << endl;
        }
    }
}

bool dfs(int node, DAG& dag, vector<int>& visited, int visitedNode) 
{
    //cout << "DFS on node " << node << endl;
    visited[node] = visited[node]+1;  // Mark the node as visiting

    // Explore all neighbors
    if (dag.data[node].empty()) 
    {
        visited[node] = dag.parents[node].size()+1;  // Mark the node
        return false;
    }
    for (int neighbor : dag.data[node]) 
    {
        if (visited[neighbor] == dag.parents[node].size() and dag.parents[node].size() != 0) // if the node is visited 
        {  // A cycle is detected

            cout << "Cycle detected in node" << node << endl;
            //return true;

            dag.data[node].push_back(visitedNode);
            push_out(dag, node, visitedNode, "data");
            push_out(dag, visitedNode, visitedNode, "parents");
            dag.parents[visitedNode].push_back(node);
            
        }
        if (visited[neighbor] == 0 and dfs(neighbor, dag, visited, node)) 
        {
            cout << "Cycle detected in node" << node << endl;
            
            dag.data[node].push_back(visitedNode);
            push_out(dag, node, visitedNode, "data");
            push_out(dag, visitedNode, visitedNode, "parents");
            dag.parents[visitedNode].push_back(node);
        }
    }
/*
when cycle is detected,

node where cycle is detected: 
    node 1 -> node x -> node 2 -> node 1
    node 1 has node 2 as parent
    node 2 has node 1 as son

    change node 2 as son of node 1 and node 1 as parent of node 2

e.g. of output:
    node 2 <- node 1 -> node x -> node 2
    node 1 has node 2 as son
    node 2 has node 1 as parent
*/
    visited[node] = dag.parents[node].size()+1;  // Mark the node
    return false;
}

DAG buildDag(DAG& dag) 
{
    cout << "1" << endl;
    loadDag(dag);
    cout << "2" << endl;
    loadParents(dag);
    cout << "3" << endl;
    //afficheData(dag);
    //afficheParents(dag);
    cout << "DAG loaded" << endl;
    // To track the state of the nodes during DFS (0 = unvisited, 1 = visiting, 2 = visited)
    int n = dag.data.size();
    cout << "4" << endl;
    unsigned int counter = 0;
    unsigned int progression = 0;
    unsigned int progression_backup = 0;
    vector<int> visited(n, 0);  // 0 = unvisited, 1 = visiting, 2 = visited
    cout << "5" << endl;
    // Check for cycles in the graph
    for (int i = 1; i < n; ++i) 
    {
        counter++;
        progression = (counter / CSV_LINES)*100;
        if (progression != progression_backup) 
        {
            cout << "\rDag checked at ... " << progression << " %" << flush;
            progression_backup = progression;
        }
        if (visited[i] == 0) 
        {  // If the node is not visited
            if (dfs(i, dag, visited, 0)) 
            {
                cout << "The graph is free of cycle!" << endl;
                visited.clear();
                vector<int> visited(dag.data.size(), 0);
            }
        }
    }
    cout << "6" << endl;
    cout << "The graph is acyclic (DAG)." << endl;
    //cin.get();
    return dag;
}

#endif