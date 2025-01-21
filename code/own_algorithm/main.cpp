#include "header.hpp"

int main () {

    Graph graph;
    Files files;

    takeFolderInput(files, ASK_FOLDER);
    buildGraph(graph, files);
    buildLandmarks(graph, LANDMARKS);
    mapToConnexions(graph);
    cout << "End";
 
    cin.get(); // Pause the console

    return 0;

}