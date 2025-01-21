#include "header.hpp"

int main () {

    Graph graph;
    Files files;

    takeFolderInput(files);

    loadGraph(graph, files);

    loadLandmarks(graph, files);

    mapToConnexions(graph);
    
    cout << "End";
 
    cin.get(); // Pause the console

    return 0;

}