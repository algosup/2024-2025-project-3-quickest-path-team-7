#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "header.hpp"

void reset_compute_data(Graph& graph, Path& path_data, Astar& astar) {

    // initialize the vectors
    astar.checked.clear();
    astar.node_before.clear();
    astar.cost_from_start.clear();

    astar.checked.resize(graph.nodes_qty, false);
    astar.node_before.resize(graph.nodes_qty, {-1, 0});
    astar.cost_from_start.resize(graph.nodes_qty, INF);

    // Empty the priority queue
    while (!astar.pq.empty()) {
        astar.pq.pop();
    }

    // reset the path_data
    path_data.path.clear();
    path_data.distance = 0;
    path_data.calculation_time = 0;
    path_data.estimated_distance = 0;
}

// Function to estimate the distance between two nodes using landmarks
int estimate_distance(Graph& graph, int source, int destination) {

    int estimation = 0;
    int source_index = source * landmarks_qty;
    int destination_index = destination * landmarks_qty;

    // For each landmark : "
    for (int i = 0; i < graph.landmarks.size(); i++) {
        // Take the absolute value of the difference between these two distances
        int diff = abs(graph.landmark_distance[source_index+i] - graph.landmark_distance[destination_index+i]);
        // Keep only the maximum of these estimations
        if (diff > estimation) {
            estimation = diff;
        }
    }

    return estimation * heuristic_weight;
}

void reconstruct_path(vector<int_pair>& node_before, Path& path_data) {

    int current_node = path_data.end;

    while (current_node != path_data.start) {
        path_data.path.insert(path_data.path.begin(), {current_node, node_before[current_node].second});
        path_data.distance += node_before[current_node].second;
        current_node = node_before[current_node].first;
    }
    path_data.path.insert(path_data.path.begin(), {current_node, 0});

    return;
}

// A* derived algorithm to find the shortest path between two nodes
void astar_algorithm(Graph& graph, Path& path_data, Astar& astar) {

    if (! graph.loaded) {
        cout << "Graph not loaded, please load it first." << endl;
        return;
    }
    if (!graph.landmarks_loaded) {
        cout << "Landmarks not loaded, please load them first." << endl;
        return;
    }

    int start_to_end_estimation = estimate_distance(graph, path_data.start, path_data.end);

    Node current_node = {path_data.start, 0, start_to_end_estimation};
    astar.pq.push(current_node);
    astar.cost_from_start[current_node.id] = 0;

    while (!astar.pq.empty()) {
        current_node = astar.pq.top();
        astar.pq.pop();

        if (current_node.id == path_data.end) {
            reconstruct_path(astar.node_before, path_data);
            return;
        }

        if (astar.checked[current_node.id]) continue;
        astar.checked[current_node.id] = true;

        for (int i = graph.adjacency_start[current_node.id]; i < graph.adjacency_start[current_node.id+1]; i++) {

            if (astar.checked[graph.edges[i].id]) continue;

            int local_cost = astar.cost_from_start[current_node.id] + graph.edges[i].weight;
            if (local_cost < astar.cost_from_start[graph.edges[i].id]) {

                astar.node_before[graph.edges[i].id] = {current_node.id, graph.edges[i].weight};
                astar.cost_from_start[graph.edges[i].id] = local_cost;

                int estimated_cost = local_cost + estimate_distance(graph, graph.edges[i].id, path_data.end);
                Node neighbor = {graph.edges[i].id, graph.edges[i].weight, estimated_cost};
                astar.pq.push(neighbor);
            }
        }
    }
} 

void find_path(Graph& graph, Path& path_data, Astar& astar, Timer& timer) {

    start_timer(timer);
    astar_algorithm(graph, path_data, astar);
    stop_timer(timer);

    path_data.calculation_time = timer.time;

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