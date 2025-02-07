#include "header.hpp"

int main() 
{
    DAG dag;
    cout << "Build dag" << endl;
    buildDag(dag);
    cout << "Check connectivity" << endl;
    connectivityChecker(dag);
    return 0;
}