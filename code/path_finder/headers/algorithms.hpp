#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "header.hpp"

void resetComputeData(Graph& Graph, Path& Path, Astar& Astar) {

    // initialize the vectors
    Astar.checked.clear();
    Astar.node_before.clear();
    Astar.cost_from_start.clear();

    Astar.checked.resize(Graph.nodes_qty, false);
    Astar.node_before.resize(Graph.nodes_qty, {-1, 0});
    Astar.cost_from_start.resize(Graph.nodes_qty, INF);

    // Empty the priority queue
    while (!Astar.pq.empty()) {
        Astar.pq.pop();
    }

    // reset the Path
    Path.path.clear();
    Path.distance = 0;
    Path.calculation_time = 0;
    Path.estimated_distance = 0;
}

// Function to estimate the distance between two nodes using landmarks
int estimateDistance(Graph& Graph, int source, int destination, bool null_heuristic = false) {

    if (null_heuristic) return 0; // Serves to disable the heuristic so Astar is reduced to a dijkstra algorithm

    int estimation = 0;
    int source_index = source * landmarks_qty;
    int destination_index = destination * landmarks_qty;

    // For each landmark : "
    for (int i = 0; i < Graph.landmarks.size(); i++) {
        // Take the absolute value of the difference between these two distances
        int diff = abs(Graph.landmark_distance[source_index+i] - Graph.landmark_distance[destination_index+i]);
        // Keep only the maximum of these estimations
        if (diff > estimation) {
            estimation = diff;
        }
    }

    return estimation * heuristic_weight;
}

void reconstructPath(vector<int_pair>& node_before, Path& Path) {

    int current_node = Path.end;

    while (current_node != Path.start) {
        Path.path.insert(Path.path.begin(), {current_node, node_before[current_node].second});
        Path.distance += node_before[current_node].second;
        current_node = node_before[current_node].first;
    }
    Path.path.insert(Path.path.begin(), {current_node, 0});

    return;
}

// A* derived algorithm to find the shortest path between two nodes
void astarAlgorithm(Graph& Graph, Path& Path, Astar& Astar, bool null_heuristic = false) {

    if (! Graph.loaded) {
        println("ERROR: Graph not loaded, please load it first.", type::ERROR_BOLD);
        return;
    }
    if (!Graph.landmarks_loaded) {
        println("ERROR: Landmarks not loaded, please load them first.", type::ERROR_BOLD);
        return;
    }

    int start_to_end_estimation = estimateDistance(Graph, Path.start, Path.end, null_heuristic);

    Node current_node = {Path.start, 0, start_to_end_estimation};
    Astar.pq.push(current_node);
    Astar.cost_from_start[current_node.id] = 0;

    while (!Astar.pq.empty()) {
        current_node = Astar.pq.top();
        Astar.pq.pop();

        if (current_node.id == Path.end) {
            reconstructPath(Astar.node_before, Path);
            return;
        }

        if (Astar.checked[current_node.id]) continue;
        Astar.checked[current_node.id] = true;

        for (int i = Graph.adjacency_start[current_node.id]; i < Graph.adjacency_start[current_node.id+1]; i++) {

            if (Astar.checked[Graph.edges[i].id]) continue;

            int local_cost = Astar.cost_from_start[current_node.id] + Graph.edges[i].weight;
            if (local_cost < Astar.cost_from_start[Graph.edges[i].id]) {

                Astar.node_before[Graph.edges[i].id] = {current_node.id, Graph.edges[i].weight};
                Astar.cost_from_start[Graph.edges[i].id] = local_cost;

                int estimated_cost = local_cost + estimateDistance(Graph, Graph.edges[i].id, Path.end, null_heuristic);
                Node neighbor = {Graph.edges[i].id, Graph.edges[i].weight, estimated_cost};
                Astar.pq.push(neighbor);
            }
        }
    }
} 

void findPath(Graph& Graph, Path& Path, Astar& Astar, Timer& timer, bool null_heuristic = false) {

    startTimer(timer);
    astarAlgorithm(Graph, Path, Astar, null_heuristic);
    stopTimer(timer);

    Path.calculation_time = timer.time;

}

// Function to return the list of all the shortest paths from the source node to every other node. So distances[i] will contain the shortest distance from the source node to node i
vector<int> shortestPaths(Graph& Graph, int source) {
    vector<int> distances(Graph.nodes_qty, INF); // Initialize distances to infinity
    distances[source] = 0; // Distance to the source node is 0

    // Min-heap priority queue: (distance, node)
    priority_queue<int_pair, vector<int_pair>, greater<>> pq;

    // Preload the source node's neighbors
    for (int i = Graph.adjacency_start[source]; i < Graph.adjacency_start[source+1]; i++) {
        Edge neighbor = Graph.edges[i];
        distances[neighbor.id] = neighbor.weight;
        pq.push({neighbor.weight, neighbor.id});
    }

    pq.push({0, source});

    while (!pq.empty()) {
        int_pair top = pq.top();
        int current_dist = top.first;
        int current_node = top.second;
        pq.pop();

        // If the popped distance is greater than the recorded distance, skip processing
        if (current_dist > distances[current_node]) {
            continue;
        }

        // Relax all edges of the current node
        for (int i = Graph.adjacency_start[current_node]; i < Graph.adjacency_start[current_node+1]; i++) {
            Edge neighbor = Graph.edges[i];
            int newDist = current_dist + neighbor.weight;
            if (newDist < distances[neighbor.id]) {
                distances[neighbor.id] = newDist;
                pq.push({newDist, neighbor.id});
            }
        }
    }

    return distances; 
}

#endif