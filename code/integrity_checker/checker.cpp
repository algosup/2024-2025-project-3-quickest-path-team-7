#include "header.hpp"

int main() {

    DAG dag;
    Graph graph;

    cout << "\nInitialization\n" << endl;

    // Build a DAG from CSV
    dag = buildDag(dag);
    // Check if the DAG is acyclic
    bfs(dag);
    // Delete the DAG

    // Load Csv data in memory as graph eventually from a backup
    //buildGraph(graph);

    // Check if all the paths are existing

    
    return 0;

}