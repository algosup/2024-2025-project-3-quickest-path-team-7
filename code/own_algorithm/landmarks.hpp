#ifndef LANDMARKS_HPP
#define LANDMARKS_HPP

#include "header.hpp"

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
    graph.landmark_distance = reversedLandmarkTable;
}

// Function to select the landmarks using the farthest selection
// Beginiing with the root landmark, all the shortest paths to each node are calculated
// Then, it stores all these distance associated to this landmark
// The next landmark is the one that has the maximum distance to the previous landmarks
// This process is repeated until the number of landmarks is reached 

void buildLandmarks(Graph& graph, int numLandmarks) {
    int n = graph.map.size();
    vector<bool> isLandmark(n, false);
    graph.landmark_distance.resize(numLandmarks, vector<int>(n));
    int firstLandmark = ROOT;
    int sourceNode;

    graph.landmarks.push_back(firstLandmark);
    isLandmark[firstLandmark] = true;
    isLandmark[0] = true; // As 0 is not a valid node

    // Calculate all the distances from the first landmark to all the nodes
    // and store them in the landmark_distance table
    // Then find the farthest node as the next landmark
    for (int landmark = 0; landmark < numLandmarks; ++landmark) {
        sourceNode = graph.landmarks.back();
        cout << "Landmark " << landmark + 1 << ": " << sourceNode << endl;
        cout << "Calculating shortest path to all nodes..." << endl;
        graph.landmark_distance[landmark] = shortestPaths(graph, sourceNode);

        // Scan the distances from the current landmark to all the nodes
        // to find the farthest node as the next landmark

        if (landmark == numLandmarks - 1) {
            break; // As we don't need to find the farthest node for the last landmark
        }

        // Iterate over all the nodes to find the longest distance
        int farthestNode = -1, maxDistance = -1;
        for (int node = 0; node < n; ++node) {
            if (!isLandmark[node] && graph.landmark_distance[landmark][node] > maxDistance) {
                farthestNode = node;
                maxDistance = graph.landmark_distance[landmark][node];
            }
        }
        // Mark the farthest node as a new landmark
        graph.landmarks.push_back(farthestNode);
        isLandmark[farthestNode] = true;
    }

    // Reverse the landmark_distance table for faster access
    cout << "Reversing the landmark table indexes..." << endl;
    reverseLandmarkTableIndexes(graph);
}



/* 
// Function to select landmarks using greedy farthest selection
void selectLandmarks(Graph& graph, int numLandmarks) {
    int n = graph.map.size();
    vector<bool> isLandmark(n, false);
    int firstLandmark = ROOT;

    graph.landmarks.push_back(firstLandmark);
    isLandmark[firstLandmark] = true;

    // Step 2: Iteratively choose the farthest node from the existing landmarks
    for (int i = 1; i < numLandmarks; ++i) {
        int sourceNode = graph.landmarks.back();
        vector<int> distances = dijkstra(graph, sourceNode);
        // Find the longest distance and take it as the farthest node
        int farthestNode = -1, maxDistance = -1;
        for ( int distance : distances) {
            if (!isLandmark[distance.first] && distance.second > maxDistance) {
                farthestNode = distance.first;
                maxDistance = distance.second;
            }
        }

        // Mark the farthest node as a landmark
        graph.landmarks.push_back(farthestNode);
        cout << "Landmark " << i << ": " << farthestNode << endl;
        isLandmark[farthestNode] = true;
    }
}

// build the landmark distance table  
void buildLandmarkTable(Graph& graph) {
    int n = graph.map.size();
    int m = graph.landmarks.size();
    graph.landmark_distance.resize(n, vector<int>(m, INF));

    unsigned int counter = 0;
    unsigned int progression = 0;
    unsigned int progression_backup = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            graph.landmark_distance[i][j] = dijkstra(graph, i)[graph.landmarks[j]];
        }
        counter++;
        progression = counter * 100 / CSV_LINES;
        if (progression != progression_backup) {
            cout << "\rLoading the CSV file into memory ... " << progression << " %" << flush;
            progression_backup = progression;
        }
    }
}
 */
#endif