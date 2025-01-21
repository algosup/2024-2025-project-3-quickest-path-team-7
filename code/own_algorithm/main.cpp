#include "header.hpp"

int main () {

    Graph graph;
    Files files;

    takeFolderInput(files);

    loadGraph(graph, files);

    loadLandmarks(graph, files);

    cout << graph.landmark_distance[56798][2] << endl;

    mapToConnexions(graph);

    cout << "End";
 
    cin.get(); // Pause the console

    return 0;

}