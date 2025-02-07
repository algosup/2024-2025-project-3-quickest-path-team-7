#ifndef OUTPUTS_H
#define OUTPUTS_H

#include "header.hpp"

struct Graph 
{
    vector<vector<int>> data;
    vector<int> start_indices;
    vector<int> neighbors;
    vector<int> weights;
};

struct DAG 
{
    vector<vector<int>> data;
    vector<vector<int>> parents;
};

// Function to calculate then display the memory usage of the graph
void GraphMemoryUsage(Graph& graph) 
{

    size_t totalSize = 0;
    for (const auto& neighbors : graph.data) 
    {
        totalSize += sizeof(neighbors) + (neighbors.size() * sizeof(pii));
    }
    // Then graph.start_indices
    totalSize += sizeof(graph.start_indices) + (graph.start_indices.size() * sizeof(int));
    // Then graph.neighbors
    totalSize += sizeof(graph.neighbors) + (graph.neighbors.size() * sizeof(int));
    // Finally graph.weights
    totalSize += sizeof(graph.weights) + (graph.weights.size() * sizeof(int));


    // Adapt the division by 1024 acording to the size of the graph (from Byte to GB) in 2 digit precision
    if (totalSize > 1024 * 1024 * 1024) 
    {
        cout << "Memory used by the Graph: " << fixed << setprecision(2) << (double)totalSize / (1024 * 1024 * 1024) << " GB" << endl;
    } else if (totalSize > 1024 * 1024) 
    {
        cout << "Memory used by the Graph: " << fixed << setprecision(2) << (double)totalSize / (1024 * 1024) << " MB" << endl;
    } else if (totalSize > 1024) 
    {
        cout << "Memory used by the Graph: " << fixed << setprecision(2) << (double)totalSize / 1024 << " KB" << endl;
    } else 
    {
        cout << "Memory used by the Graph: " << fixed << setprecision(2) << (double)totalSize << " Bytes" << endl;
    }
}

// Function to format a number with spaces between each group of three digits
string formatWithSpaces(long number) 
{
    string numStr = to_string(number);
    int insertPosition = numStr.length() - 3;
    while (insertPosition > 0) 
    {
        numStr.insert(insertPosition, " ");
        insertPosition -= 3;
    }
    return numStr;
}


#endif