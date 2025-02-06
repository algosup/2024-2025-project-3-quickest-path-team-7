#ifndef GRAPH_H
#define GRAPH_H

#include "header.hpp"

void buildGraph(Graph& graph) 
{
    cout << "Load graph" << endl;
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
        data[elemList[1]].push_back(elemList[0]);
        
    }
    graph.data.resize(NODE_MAX_VALUE);
    for (const auto& [node, neighbors] : data) 
    {
        graph.data[node] = neighbors;
    }
    file.close();
    return;
}

#endif