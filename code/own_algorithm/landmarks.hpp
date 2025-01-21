#ifndef LANDMARKS_QTY_HPP
#define LANDMARKS_QTY_HPP

#include "header.hpp"


// Function to remove the weight from the graph to free memory
// The graph is now a list of connexions
void mapToConnexions(Graph& graph){
    cout << "Transferring map to single connexions ..." << endl;
    int graph_size = graph.map.size();
    int temp_node;
    graph.connexions.resize(graph_size);
    for (int node = 0; node < graph_size; ++node){
        for (int sub_node = 0; sub_node < graph.map[node].size(); ++sub_node ){
            temp_node = graph.map[node][sub_node].first;
            graph.connexions[node].push_back(temp_node);
        }
    graph.map[node].clear();
    }
    graph.map.clear();
    graph.map.shrink_to_fit();
    cout << "Map cleared" << endl;
}

// Function to reverse the landmark_distance table indexes for faster access
// From landmark_distance[landmark_index][node] to landmark_distance[node][landmark_index]
void reverseLandmarkTableIndexes(Graph& graph) {
    int n = graph.map.size();
    int m = graph.landmarks.size();
    vector<vector<int>> reversedLandmarkTable(n, vector<int>(m));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            reversedLandmarkTable[i][j] = graph.landmark_distance[j][i];
        }
    }
    
    graph.landmark_distance.clear();
    graph.landmark_distance.shrink_to_fit();
    graph.landmark_distance = reversedLandmarkTable;
}


// Function to select the landmarks using the farthest selection
// Beginiing with the root landmark, all the shortest paths to each node are calculated
// Then, it stores all these distance associated to this landmark
// The next landmark is the one that has the maximum distance to the previous landmarks
// This process is repeated until the number of landmarks is reached 

void buildLandmarks(Graph& graph) {
    int n = graph.map.size();
    vector<bool> isLandmark(n, false);
    graph.landmark_distance.resize(LANDMARKS_QTY, vector<int>(n));
    int firstLandmark = ROOT;
    int sourceNode;

    graph.landmarks.push_back(firstLandmark);
    isLandmark[firstLandmark] = true;
    isLandmark[0] = true; // As 0 is not a valid node

    // Calculate all the distances from the first landmark to all the nodes
    // and store them in the landmark_distance table
    // Then find the farthest node as the next landmark
    for (int landmark = 0; landmark < LANDMARKS_QTY; ++landmark) {
        sourceNode = graph.landmarks.back();
        cout << "Landmark " << landmark + 1 << ": " << sourceNode << endl;
        cout << "Calculating shortest path to all nodes..." << endl;
        graph.landmark_distance[landmark] = shortestPaths(graph, sourceNode);

        // Scan the distances from the current landmark to all the nodes
        // to find the farthest node as the next landmark

        if (landmark == LANDMARKS_QTY - 1) {
            break; // As we don't need to find the farthest node for the last landmark
        }

        // Iterate over all the nodes to find the farthest node from all previous landmarks
        int farthestNode = -1, maxDistance = -1, maxMinDistance = -1;
        // Fo reach node
        for (int node = 0; node < n; ++node) {
            // Skip the nodes already selected as landmark
            if (isLandmark[node]) continue;
            
            // Compute the minimum distance between the node to all chosen landmarks
            int minDistance = INF;
            for (int prevLandmark = 0; prevLandmark < graph.landmarks.size(); ++prevLandmark) {
                int landmarkIndex = graph.landmarks[prevLandmark];
                minDistance = min(minDistance, graph.landmark_distance[prevLandmark][node]);
            }

            // Update the farthest node based on the maximum of these minimum distances
            if (minDistance > maxMinDistance) {
                maxMinDistance = minDistance;
                farthestNode = node;
            }
        }
        // Mark the farthest node as a new landmark
        graph.landmarks.push_back(farthestNode);
        isLandmark[farthestNode] = true;
    }

    cout << "Reversing the landmark table indexes..." << endl;
    reverseLandmarkTableIndexes(graph);
    cout << "Landmarks reversing completed." << endl;
}

void loadLandmarks (Graph& graph, Files& files){
    cout << "Building landmarks..." << endl;
    /* // check if a landmarks backup exists
    if (loadLandmarksFromBinary(files.landmarks_backup, graph.landmarks)) {
        cout << "Landmarks loaded from binary backup." << endl;
        return;
    } */
    buildLandmarks(graph);
    cout << "Landmarks built." << endl;
    /* saveLandmarksToBinary(files.landmarks_backup, graph.landmarks); 
    cout << "Landmarks saved to binary backup." << endl;*/
    cout << "End of landmarks building." << endl;

}

#endif