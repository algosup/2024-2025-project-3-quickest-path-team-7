#include "header.hpp"

int main () {

    Graph graph;
    Files files;

    takeFolderInput(files, ASK_FOLDER);

    buildGraph(graph, files);
    GraphMemoryUsage(graph);
    buildLandmarks(graph, LANDMARKS);
    GraphMemoryUsage(graph);

    cin.get(); // Pause the console

    return 0;

}