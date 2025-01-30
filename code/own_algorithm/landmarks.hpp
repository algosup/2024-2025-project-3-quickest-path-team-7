#ifndef LANDMARKS_HPP
#define LANDMARKS_HPP

#include "header.hpp"

// Function to select the landmarks using the farthest selection
// Beginiing with the root landmark, all the shortest paths to each node are calculated
// Then, it stores all these distance associated to this landmark
// The next landmark is the one that has the maximum distance to the previous landmarks
// This process is repeated until the number of landmarks is reached 

void buildLandmarks(Graph& graph) {

    cout << "Building landmarks ... " << flush;

    vector<bool> isLandmark(graph.nodes_qty, false);
    vector<int> pathsArray(graph.nodes_qty);
    graph.landmark_distance.resize(landmarks_qty*graph.nodes_qty);

    graph.landmarks.push_back(root_landmark);
    isLandmark[root_landmark] = true;
    isLandmark[0] = true;

    // Calculate all the distances from the first landmark to all the nodes
    // and store them in the landmark_distance table
    // Then find the farthest node as the next landmark
    for (int landmark = 0; landmark < landmarks_qty; ++landmark) {

        cout << "\rBuilding landmarks (" << landmark+1  << "/" << landmarks_qty << ") ... " << flush;

        int current_landmark = graph.landmarks.back();
        // get all the shortest paths from the landmark node to all the other nodes
        pathsArray = shortestPaths(graph, current_landmark);
        // Store them in landmark_distance[node * landmarks_qty + landmark]
        for (int node = 1; node <= graph.nodes_qty; ++node) {
            graph.landmark_distance[node * landmarks_qty + landmark] = pathsArray[node];
        }

        // Scan the distances from the current landmark to all the nodes
        // to find the farthest node as the next landmark

        if (landmark == landmarks_qty - 1) {
            break; // As we don't need to find the farthest node for the last landmark
        }

        // Iterate over all the nodes to find the farthest node from all previous landmarks
        int farthestNode = -1, maxMinDistance = -1;
        // Fo reach node
        for (int node = 0; node < graph.nodes_qty; ++node) {
            // Skip the nodes already selected as landmark
            if (isLandmark[node]) continue;
            
            // Compute the minimum distance between the node to all chosen landmarks
            int minDistance = INF;
            for (int prevLandmark = 0; prevLandmark < graph.landmarks.size(); ++prevLandmark) {
                int landmarkIndex = graph.landmarks[prevLandmark];
                minDistance = min(minDistance, graph.landmark_distance[node * landmarks_qty + prevLandmark]);
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

    cout << "\rBuilding landmarks (" << landmarks_qty << "/" << landmarks_qty << ") ... Done !" << endl;

}

void saveLandmarksToBinary(Graph& graph, Files& files) {

    cout << "Saving landmarks to " << files.landmarks_backup << " ... " << flush;

    ofstream file(files.landmarks_backup, ios::binary);
    if (!file.is_open()) {
        cerr << "\nFailed to open the file for writing: " << files.landmarks_backup << endl;
        return;
    }

    // n = number of nodes, m = number of chosen landmarks
    int n = graph.nodes_qty;
    int m = landmarks_qty;

    // 1) Write n and m
    file.write(reinterpret_cast<const char*>(&n), sizeof(n));
    file.write(reinterpret_cast<const char*>(&m), sizeof(m));

    // 2) Write the 'graph.landmarks' array
    file.write(reinterpret_cast<const char*>(graph.landmarks.data()), m * sizeof(int));

    // 3) Write the single 'graph.landmark_distance' array of size n*m
    file.write(reinterpret_cast<const char*>(graph.landmark_distance.data()), 
               (size_t)n * m * sizeof(int));

    file.close();

    cout << "Done!" << endl;
}

bool loadLandmarksFromBinary(Graph& graph, Files& files) {
    cout << "Loading landmarks from " << files.landmarks_backup << " ... " << flush;
    ifstream file(files.landmarks_backup, ios::binary);
    if (!file.is_open()) {
        cout << "Backup not found!" << endl;
        return false;
    }

    int n, m;
    file.read(reinterpret_cast<char*>(&n), sizeof(n));
    file.read(reinterpret_cast<char*>(&m), sizeof(m));

    graph.nodes_qty = n; // might update the map_size if needed
    graph.landmarks.resize(m);
    graph.landmark_distance.resize((size_t)n * m);

    // Read the landmarks array
    file.read(reinterpret_cast<char*>(graph.landmarks.data()), m * sizeof(int));

    // Read the single distance array
    file.read(reinterpret_cast<char*>(graph.landmark_distance.data()), 
              (size_t)n * m * sizeof(int));

    file.close();
    cout << "Done!" << endl;
    graph.landmarks_loaded = true;
    return true;
}

bool loadLandmarks (Graph& graph, Files& files, bool force = false) {

    graph.landmarks_loaded = false;

    // Ensure there are not more landmarks than nodes
    if (landmarks_qty > graph.nodes_qty) {
        landmarks_qty = graph.nodes_qty;
        root_landmark = graph.nodes_qty;
        cout << "Number of landmarks set to " << landmarks_qty << " (number of nodes)" << endl;
        cout << "New root landmark: " << root_landmark << endl;
    }

    // update the landmarks backup file name based on the number of landmarks
    files.landmarks_backup = files.root_landmarks_backup + "-" + to_string(landmarks_qty) + ".bin";

    if (!force) {
        // check if a landmarks backup exists
        if (!loadLandmarksFromBinary(graph, files)) {
            buildLandmarks(graph);
            saveLandmarksToBinary(graph, files); 
            graph.landmarks_loaded = true;
        }
    } else {
        buildLandmarks(graph);
        saveLandmarksToBinary(graph, files); 
        graph.landmarks_loaded = true;
    }

    return graph.landmarks_loaded;
}

#endif