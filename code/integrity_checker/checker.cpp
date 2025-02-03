#include "header.hpp"

int main() {
    DAG dag;
    Graph graph;
    cout << "\nInitialization\n" << endl;
    buildDag(dag);
    connectivityChecker(dag);
    return 0;
}