#ifndef LANDMARKS_QTY_HPP
#define LANDMARKS_QTY_HPP

#include "header.hpp"

// Function to select the landmarks using the farthest selection
// Beginiing with the root landmark, all the shortest paths to each node are calculated
// Then, it stores all these distance associated to this landmark
// The next landmark is the one that has the maximum distance to the previous landmarks
// This process is repeated until the number of landmarks is reached 

void buildLandmarks(Graph& graph) {

    vector<bool> isLandmark(graph.map_size, false);
    vector<int> pathsArray(graph.map_size);
    graph.landmark_distance.resize(LANDMARKS_QTY*graph.map_size);
    int firstLandmark = ROOT;

    graph.landmarks.push_back(firstLandmark);
    isLandmark[firstLandmark] = true;
    isLandmark[0] = true; // As 0 is not a valid node

    // Calculate all the distances from the first landmark to all the nodes
    // and store them in the landmark_distance table
    // Then find the farthest node as the next landmark
    for (int landmark = 0; landmark < LANDMARKS_QTY; ++landmark) {

        cout << "\rBuilding landmarks (" << landmark+1  << "/" << LANDMARKS_QTY << ") ... " << flush;

        int current_landmark = graph.landmarks.back();
        // get all the shortest paths from the landmark node to all the other nodes
        pathsArray = shortestPaths(graph, current_landmark);
        // Store them in landmark_distance[node * LANDMARKS_QTY + landmark]
        for (int node = 0; node < graph.map_size; ++node) {
            graph.landmark_distance[node * LANDMARKS_QTY + landmark] = pathsArray[node];
        }

        // Scan the distances from the current landmark to all the nodes
        // to find the farthest node as the next landmark

        if (landmark == LANDMARKS_QTY - 1) {
            break; // As we don't need to find the farthest node for the last landmark
        }

        // Iterate over all the nodes to find the farthest node from all previous landmarks
        int farthestNode = -1, maxMinDistance = -1;
        // Fo reach node
        for (int node = 0; node < graph.map_size; ++node) {
            // Skip the nodes already selected as landmark
            if (isLandmark[node]) continue;
            
            // Compute the minimum distance between the node to all chosen landmarks
            int minDistance = INF;
            for (int prevLandmark = 0; prevLandmark < graph.landmarks.size(); ++prevLandmark) {
                int landmarkIndex = graph.landmarks[prevLandmark];
                minDistance = min(minDistance, graph.landmark_distance[node * LANDMARKS_QTY + prevLandmark]);
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

}

void saveLandmarksToBinary(Graph& graph, Files& files) {
    ofstream file(files.landmarks_backup, ios::binary);
    if (!file.is_open()) {
        cerr << "Failed to open the file for writing: " << files.landmarks_backup << endl;
        return;
    }

    // n = number of nodes, m = number of chosen landmarks
    int n = graph.map_size;
    int m = (int)graph.landmarks.size();

    // 1) Write n and m
    file.write(reinterpret_cast<const char*>(&n), sizeof(n));
    file.write(reinterpret_cast<const char*>(&m), sizeof(m));

    // 2) Write the 'graph.landmarks' array
    file.write(reinterpret_cast<const char*>(graph.landmarks.data()), m * sizeof(int));

    // 3) Write the single 'graph.landmark_distance' array of size n*m
    file.write(reinterpret_cast<const char*>(graph.landmark_distance.data()), 
               (size_t)n * m * sizeof(int));

    file.close();
}

bool loadLandmarksFromBinary(Graph& graph, Files& files) {
    cout << "Loading landmarks from " << files.landmarks_backup << " ... " << flush;
    ifstream file(files.landmarks_backup, ios::binary);
    if (!file.is_open()) {
        cout << "Backup not found!\n" << endl;
        return false;
    }

    int n, m;
    file.read(reinterpret_cast<char*>(&n), sizeof(n));
    file.read(reinterpret_cast<char*>(&m), sizeof(m));

    graph.map_size = n; // might update the map_size if needed
    graph.landmarks.resize(m);
    graph.landmark_distance.resize((size_t)n * m);

    // Read the landmarks array
    file.read(reinterpret_cast<char*>(graph.landmarks.data()), m * sizeof(int));

    // Read the single distance array
    file.read(reinterpret_cast<char*>(graph.landmark_distance.data()), 
              (size_t)n * m * sizeof(int));

    file.close();
    cout << "Done!" << endl;
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