#ifndef LANDMARKS_HPP
#define LANDMARKS_HPP

#include "header.hpp"

// Function to select the landmarks using the farthest selection
// Beginiing with the root landmark, all the shortest paths to each node are calculated
// Then, it stores all these distance associated to this landmark
// The next landmark is the one that has the maximum distance to the previous landmarks
// This process is repeated until the number of landmarks is reached 

void buildLandmarks(Graph& Graph) {

    print("Building landmarks ... ");

    vector<bool> isLandmark(Graph.nodes_qty, false);
    vector<int> pathsArray(Graph.nodes_qty);
    Graph.landmark_distance.resize(landmarks_qty*Graph.nodes_qty);

    // Check if the root landmark is a valid node
    if (Graph.adjacency_start[root_landmark] == Graph.adjacency_start[root_landmark+1]) {
        root_landmark = Graph.nodes_qty;
    }
    Graph.landmarks.push_back(root_landmark);
    isLandmark[root_landmark] = true;
    isLandmark[0] = true;

    // Calculate all the distances from the first landmark to all the nodes
    // and store them in the landmark_distance table
    // Then find the farthest node as the next landmark
    for (int landmark = 0; landmark < landmarks_qty; ++landmark) {

        print("\rBuilding landmarks (" + to_string(landmark+1) + "/" + to_string(landmarks_qty) + ") ... ", type::INFO);

        int current_landmark = Graph.landmarks.back();
        // get all the shortest paths from the landmark node to all the other nodes
        pathsArray = shortestPaths(Graph, current_landmark);
        // Store them in landmark_distance[node * landmarks_qty + landmark]
        for (int node = 1; node <= Graph.nodes_qty; ++node) {
            Graph.landmark_distance[node * landmarks_qty + landmark] = pathsArray[node];
        }

        // Scan the distances from the current landmark to all the nodes
        // to find the farthest node as the next landmark

        if (landmark == landmarks_qty - 1) {
            break; // As we don't need to find the farthest node for the last landmark
        }

        // Iterate over all the nodes to find the farthest node from all previous landmarks
        int farthestNode = -1, maxMinDistance = -1;
        // Fo reach node
        for (int node = 0; node < Graph.nodes_qty; ++node) {
            // Skip the nodes already selected as landmark
            if (isLandmark[node]) continue;
            
            // Compute the minimum distance between the node to all chosen landmarks
            int minDistance = INF;
            for (int prevLandmark = 0; prevLandmark < Graph.landmarks.size(); ++prevLandmark) {
                int landmarkIndex = Graph.landmarks[prevLandmark];
                minDistance = min(minDistance, Graph.landmark_distance[node * landmarks_qty + prevLandmark]);
            }

            // Update the farthest node based on the maximum of these minimum distances
            if (minDistance > maxMinDistance) {
                maxMinDistance = minDistance;
                farthestNode = node;
            }
        }
        // Mark the farthest node as a new landmark
        Graph.landmarks.push_back(farthestNode);
        isLandmark[farthestNode] = true;

    }

    println("\rBuilding landmarks (" + to_string(landmarks_qty) + "/" + to_string(landmarks_qty) + ") ... Done !", type::VALIDATION);

}

void saveLandmarksToBinary(Graph& Graph, Files& files) {

    print("Saving landmarks to " + files.landmarks.full + " ... ");

    ofstream file(files.landmarks.full, ios::binary);
    if (!file.is_open()) {
        println("\nFailed to open the file for writing: " + files.landmarks.full, type::ERROR_BOLD);
        return;
    }

    // n = number of nodes, m = number of chosen landmarks
    int n = Graph.nodes_qty;
    int m = landmarks_qty;

    // 1) Write n and m
    file.write(reinterpret_cast<const char*>(&n), sizeof(n));
    file.write(reinterpret_cast<const char*>(&m), sizeof(m));

    // 2) Write the 'Graph.landmarks' array
    file.write(reinterpret_cast<const char*>(Graph.landmarks.data()), m * sizeof(int));

    // 3) Write the single 'Graph.landmark_distance' array of size n*m
    file.write(reinterpret_cast<const char*>(Graph.landmark_distance.data()), 
               (size_t)n * m * sizeof(int));

    file.close();

    println("\rSaving landmarks to " + files.landmarks.full + " ... Done!", type::VALIDATION);
}

bool loadLandmarksFromBinary(Graph& Graph, Files& files) {

     //Reset the landmarks data
    Graph.landmarks.clear();
    Graph.landmark_distance.clear();
    Graph.landmarks_loaded = false;
    Graph.landmarks.shrink_to_fit();
    Graph.landmark_distance.shrink_to_fit();

    print("Loading landmarks from " + files.landmarks.full + " ... ");
    ifstream file(files.landmarks.full, ios::binary);
    if (!file.is_open()) {
        println("\nBackup not found!", type::WARNING);
        return false;
    }

    int n, m;
    file.read(reinterpret_cast<char*>(&n), sizeof(n));
    file.read(reinterpret_cast<char*>(&m), sizeof(m));

    Graph.nodes_qty = n; // might update the map_size if needed
    Graph.landmarks.resize(m);
    Graph.landmark_distance.resize((size_t)n * m);

    // Read the landmarks array
    file.read(reinterpret_cast<char*>(Graph.landmarks.data()), m * sizeof(int));

    // Read the single distance array
    file.read(reinterpret_cast<char*>(Graph.landmark_distance.data()), 
              (size_t)n * m * sizeof(int));

    file.close();
    println("\rLoading landmarks from " + files.landmarks.full + " ... Done!", type::VALIDATION);
    Graph.landmarks_loaded = true;
    return true;
}

bool loadLandmarks(Graph& Graph, Files& files, bool force = false) {

    Graph.landmarks_loaded = false;

    // Ensure there are not more landmarks than nodes
    if (landmarks_qty > Graph.nodes_qty) {
        landmarks_qty = Graph.nodes_qty;
        root_landmark = Graph.nodes_qty;
        println("Number of landmarks set to " + to_string(landmarks_qty) + " (number of nodes)", type::WARNING);
        println("New root landmark: " + to_string(root_landmark), type::WARNING);
    }

    // update the landmarks backup file name based on the number of landmarks
    buildFilesPath(files);

    if (!force) {
        // check if a landmarks backup exists
        if (!loadLandmarksFromBinary(Graph, files)) {
            buildLandmarks(Graph);
            saveLandmarksToBinary(Graph, files); 
            Graph.landmarks_loaded = true;
        }
    } else {
        buildLandmarks(Graph);
        saveLandmarksToBinary(Graph, files); 
        Graph.landmarks_loaded = true;
    }

    return Graph.landmarks_loaded;

}

#endif