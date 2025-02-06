#include "header.hpp"

int main() {
    DAG dag;
    Graph graph;
    cout << "Build dag" << endl;
    buildDag(dag);
    cout << "Check connectivity" << endl;
    buildGraph(graph);
    connectivityChecker(graph);
    return 0;
}
