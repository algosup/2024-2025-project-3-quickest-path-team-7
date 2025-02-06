#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "header.hpp"

// Function to estimate the distance between two nodes using landmarks
int estimate_distance(Graph& graph, int source, int destination) {
    int estimation = 0;
    int source_index = source * LANDMARKS_QTY;
    int destination_index = destination * LANDMARKS_QTY;
    // For each landmark:
    for (int i = 0; i < graph.landmarks.size(); i++) {
        int diff = abs(graph.landmark_distance[source_index + i] - 
                       graph.landmark_distance[destination_index + i]);
        if (diff > estimation) {
            estimation = diff;
        }
    }
    return estimation;
}

// Updated function to reconstruct the path that takes the meeting node as a parameter
void reconstruct_bidirectional_path(Astar& astar1, Astar& astar2, Path& path_data, int meeting_node) {
    // Reconstruct forward path from start to meeting node
    vector<int_pair> forward_path;
    int current_node = meeting_node;
    while (current_node != -1) {
        forward_path.insert(forward_path.begin(), {current_node, astar1.node_before[current_node].second});
        current_node = astar1.node_before[current_node].first;
    }

    // Reconstruct backward path from meeting node to end
    vector<int_pair> backward_path;
    current_node = meeting_node;
    while (current_node != -1) {
        current_node = astar2.node_before[current_node].first;
        if (current_node != -1) { // avoid duplicating the meeting node
            backward_path.push_back({current_node, astar2.node_before[current_node].second});
        }
    }

    // Combine the two paths
    path_data.path = forward_path;
    path_data.path.insert(path_data.path.end(), backward_path.begin(), backward_path.end());
}

// Single-threaded bidirectional A* search that alternates forward and backward expansions.
void find_path(Graph& graph, Path& path_data, Astar& astar1, Astar& astar2) {
    // Initialize the heuristic estimate (for reporting or other purposes)
    path_data.estimated_distance = estimate_distance(graph, path_data.start, path_data.end);

    // Initialize priority queues with the start (forward) and end (backward) nodes.
    astar1.pq.push({path_data.start, 0, path_data.estimated_distance});
    astar2.pq.push({path_data.end, 0, path_data.estimated_distance});

    // Mark the starting nodes as visited and set their cost to 0.
    visited_forward[path_data.start] = true;
    visited_backward[path_data.end] = true;
    astar1.cost_from_start[path_data.start] = 0;
    astar2.cost_from_start[path_data.end] = 0;

    bool meeting_found = false;
    int meeting_node = -1;

    // Main loop: alternate one expansion from the forward search, then one from the backward search.
    while (!astar1.pq.empty() || !astar2.pq.empty()) {

        // ---------------- Forward Expansion ----------------
        if (!astar1.pq.empty()) {
            Node current = astar1.pq.top();
            astar1.pq.pop();
            astar1.iterations++;

            // Check if the current node has already been expanded by the backward search.
            if (visited_backward[current.id]) {
                meeting_found = true;
                meeting_node = current.id;
                break;
            }

            // Expand neighbors for forward search.
            for (int i = graph.adjacency_start[current.id]; i < graph.adjacency_start[current.id + 1]; i++) {
                Edge neighbor = graph.edges[i];
                int local_cost = astar1.cost_from_start[current.id] + neighbor.weight;
                if (local_cost < astar1.cost_from_start[neighbor.id]) {
                    // If this neighbor has not yet been expanded in the forward search, mark it.
                    if (!visited_forward[neighbor.id]) {
                        visited_forward[neighbor.id] = true;
                    }
                    // Update the predecessor and cost.
                    astar1.node_before[neighbor.id] = {current.id, neighbor.weight};
                    astar1.cost_from_start[neighbor.id] = local_cost;
                    int heuristic = estimate_distance(graph, neighbor.id, path_data.end);
                    astar1.pq.push({neighbor.id, neighbor.weight, local_cost + heuristic});
                }
            }
        }

        // Check for meeting node after the forward expansion.
        if (meeting_found)
            break;

        // ---------------- Backward Expansion ----------------
        if (!astar2.pq.empty()) {
            Node current = astar2.pq.top();
            astar2.pq.pop();
            astar2.iterations++;

            // Check if the current node has already been expanded by the forward search.
            if (visited_forward[current.id]) {
                meeting_found = true;
                meeting_node = current.id;
                break;
            }

            // Expand neighbors for backward search.
            for (int i = graph.adjacency_start[current.id]; i < graph.adjacency_start[current.id + 1]; i++) {
                Edge neighbor = graph.edges[i];
                int local_cost = astar2.cost_from_start[current.id] + neighbor.weight;
                if (local_cost < astar2.cost_from_start[neighbor.id]) {
                    if (!visited_backward[neighbor.id]) {
                        visited_backward[neighbor.id] = true;
                    }
                    astar2.node_before[neighbor.id] = {current.id, neighbor.weight};
                    astar2.cost_from_start[neighbor.id] = local_cost;
                    int heuristic = estimate_distance(graph, neighbor.id, path_data.start);
                    astar2.pq.push({neighbor.id, neighbor.weight, local_cost + heuristic});
                }
            }
        }
    }

    // If a meeting node was found, reconstruct the full path.
    if (meeting_found) {
        path_data.distance = astar1.cost_from_start[meeting_node] + astar2.cost_from_start[meeting_node];
        reconstruct_bidirectional_path(astar1, astar2, path_data, meeting_node);
    } else {
        // No path was found.
        path_data.path.clear();
        reset_algorithm_data(graph, path_data, astar1, astar2);
    }
}

// Function to return the list of all the shortest paths from the source node to every other node. So distances[i] will contain the shortest distance from the source node to node i
vector<int> shortestPaths(Graph& graph, int source) {
    vector<int> distances(graph.nodes_qty, INF); // Initialize distances to infinity
    distances[source] = 0; // Distance to the source node is 0

    // Min-heap priority queue: (distance, node)
    priority_queue<int_pair, vector<int_pair>, greater<>> pq;

    // Preload the source node's neighbors
    for (int i = graph.adjacency_start[source]; i < graph.adjacency_start[source+1]; i++) {
        Edge neighbor = graph.edges[i];
        distances[neighbor.id] = neighbor.weight;
        pq.push({neighbor.weight, neighbor.id});
    }

    pq.push({0, source});

    while (!pq.empty()) {
        int_pair top = pq.top();
        int currentDist = top.first;
        int currentNode = top.second;
        pq.pop();

        // If the popped distance is greater than the recorded distance, skip processing
        if (currentDist > distances[currentNode]) {
            continue;
        }

        // Relax all edges of the current node

        for (int i = graph.adjacency_start[currentNode]; i < graph.adjacency_start[currentNode+1]; i++) {
            Edge neighbor = graph.edges[i];
            int newDist = currentDist + neighbor.weight;
            if (newDist < distances[neighbor.id]) {
                distances[neighbor.id] = newDist;
                pq.push({newDist, neighbor.id});
            }
        }
    }

    return distances; 
}

#endif