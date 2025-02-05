#include "header.hpp"

int main() {
    DAG dag;
    cout << "phase 1" << endl;
    buildDag(dag);
    cout << "phase 2" << endl;
    connectivityChecker(dag);
    return 0;
}
