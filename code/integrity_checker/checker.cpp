#include "header.hpp"

int main() {

    vector<vector<int>> dag;
    Graph graph;

    cout << "\nInitialization\n" << endl;

    // Build a DAG from CSV
    buildDag(dag);
    // Check if the DAG is acyclic

    // Delete the DAG

    // Load Csv data in memory as graph eventually from a backup
    buildGraph(graph);

    // Check if all the paths are existing

    
    return 0;

}