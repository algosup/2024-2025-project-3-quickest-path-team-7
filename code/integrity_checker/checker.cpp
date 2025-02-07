/*
The Checker is a tool for ensuring the compatibility of the data you want to use in our shortest path algorithm.
To do so, the checker will:
    - build a DAG with the given CSV file 
    - check the connectivity of the data using the DAG 
It ensures that you can travel from any existing node to another one.
*/
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