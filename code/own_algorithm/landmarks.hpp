#ifndef LANDMARKS_QTY_HPP
#define LANDMARKS_QTY_HPP

#include "header.hpp"


// Function to reverse the landmark_distance table indexes for faster access
// From landmark_distance[landmark_index][node] to landmark_distance[node][landmark_index]
void reverseLandmarkTableIndexes(Graph& graph) {
    int n = graph.map_size;
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

    int landmarks = 1;
    int n = graph.map_size;
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

        cout << "\rBuilding landmarks (" << landmark+1  << "/" << LANDMARKS_QTY << ") ... " << flush;

        sourceNode = graph.landmarks.back();
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

    cout << "\rBuilding landmarks (" << LANDMARKS_QTY << "/" << LANDMARKS_QTY << ") ... Done !" << endl;

    cout << "\nReversing the landmark table indexes ... " << flush;
    reverseLandmarkTableIndexes(graph);

}

// Function to save the landmarks to a binary file
void saveLandmarksToBinary(Graph& graph, Files& files){
    ofstream file(files.landmarks_backup, ios::binary);

    int n = graph.map_size;
    int m = graph.landmarks.size();
    
    file.write(reinterpret_cast<char*>(&n), sizeof(n));
    file.write(reinterpret_cast<char*>(&m), sizeof(m));

    file.write(reinterpret_cast<char*>(graph.landmarks.data()), m * sizeof(int));

    for (const vector<int>& distances : graph.landmark_distance) {
        file.write(reinterpret_cast<const char*>(distances.data()), m * sizeof(int));
    }

    file.close();
}

bool loadLandmarksFromBinary(Graph& graph, Files& files){
    cout << "Loading landmarks from "<< files.landmarks_backup << " ... " << flush;
    ifstream file(files.landmarks_backup, ios::binary);
    if (!file.is_open()) {
        cout << "Backup not found !\n" << endl;
        return false;
    }

    int n, m;
    file.read(reinterpret_cast<char*>(&n), sizeof(n));
    file.read(reinterpret_cast<char*>(&m), sizeof(m));

    graph.landmarks.resize(m);
    graph.landmark_distance.resize(n, vector<int>(m));

    file.read(reinterpret_cast<char*>(graph.landmarks.data()), m * sizeof(int));

    for (vector<int>& distances : graph.landmark_distance) {
        distances.resize(m); // Ensure the vector is properly resized
        file.read(reinterpret_cast<char*>(distances.data()), m * sizeof(int));
    }

    file.close();
    cout << "Done !" << endl;
    return true;
}

void loadLandmarks (Graph& graph, Files& files){

    // check if a landmarks backup exists
    if (!loadLandmarksFromBinary(graph, files)) {
        cout << "Building landmarks... " << flush;
        buildLandmarks(graph);
        cout << "Done !" << endl;
        cout << "Saving landmarks to binary backup... " << flush;
        saveLandmarksToBinary(graph, files); 
        cout << "Done !" << endl;
    }
}

#endif