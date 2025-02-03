#include "header.hpp"

int main() {
    DAG dag;
    Graph graph;
    cout << "phase 1" << endl;
    cout << "\nInitialization\n" << endl;
    buildDag(dag);
    cout << "phase 2" << endl;
    connectivityChecker(dag);
    return 0;
}