#ifndef Graph_H
#define Graph_H

#include "header.hpp"

bool saveGraphToBinary(const Graph& Graph, const Files& Files) {

    print("Saving the graph to " + Files.graph.full + " ... ");
    // Open/Create the binary file backup
    ofstream ofs(Files.graph.full, ios::binary);
    if (!ofs.is_open()) {
        return FAIL;
    }

    // Write the number of nodes
    ofs.write(reinterpret_cast<const char*>(&Graph.nodes_qty), sizeof(Graph.nodes_qty));

    // Write adjacency_start array size + contents
    int startArraySize = Graph.adjacency_start.size();
    ofs.write(reinterpret_cast<const char*>(&startArraySize), sizeof(startArraySize));
    ofs.write(reinterpret_cast<const char*>(Graph.adjacency_start.data()),
              startArraySize * sizeof(int));

    // Write edges array size + contents
    int edgesSize = Graph.edges.size();
    ofs.write(reinterpret_cast<const char*>(&edgesSize), sizeof(edgesSize));
    ofs.write(reinterpret_cast<const char*>(Graph.edges.data()),
              edgesSize * sizeof(Edge));

    ofs.close();

    println("\rSaving the graph to " + Files.graph.full + " ... Done !", type::VALIDATION);
    return SUCCESS;
}

// Load compressed adjacency data from the binary file
bool loadGraphFromBinary(Graph& Graph, const Files& Files) {

    // Open the binary file backup
    ifstream ifs(Files.graph.full, ios::binary);

    // Read map_size
    ifs.read(reinterpret_cast<char*>(&Graph.nodes_qty), sizeof(Graph.nodes_qty));

    // Read adjacency_start 
    int startArraySize = 0;
    ifs.read(reinterpret_cast<char*>(&startArraySize), sizeof(startArraySize));
    Graph.adjacency_start.resize(static_cast<int>(startArraySize));
    ifs.read(reinterpret_cast<char*>(Graph.adjacency_start.data()),
             startArraySize * sizeof(int));

    // Read edges array
    int edgesSize = 0;
    ifs.read(reinterpret_cast<char*>(&edgesSize), sizeof(edgesSize));
    Graph.edges.resize(static_cast<int>(edgesSize));
    ifs.read(reinterpret_cast<char*>(Graph.edges.data()),
             edgesSize * sizeof(Edge));

    ifs.close();
    
    return SUCCESS;

}

// Temporary structure to store edges before compressing
struct FullEdge {
    int source;
    int destination;
    int weight;
};

bool buildGraphFromCSV(Graph& Graph, const Files& Files) {

    // Open the CSV file
    ifstream csv_file(Files.dataset.full);
    
    // vector to store all connections of the CSV file
    vector<FullEdge> allEdges;

    string line;
    Graph.nodes_qty = 0;
    unsigned int counter = 0;
    unsigned int progression = 0;
    unsigned int progression_backup = 0;

    // For each line in the CSV file
    while (getline(csv_file, line)) {

        // Skip empty lines
        if (line.empty()) continue;

        // For each 'cell' (number) in the line, extract it in row[]
        stringstream ss(line);
        vector<int> row;
        string cell;
        while (getline(ss, cell, ',')) {
            try {
                row.push_back(stoi(cell));
            } catch (const invalid_argument& e) {
                println("ERROR : Parsing CSV at line " + to_string(counter+1), type::ERROR);
                println("-> Invalid node value", type::ERROR);
                return FAIL;
            } catch (const out_of_range& e) {
                println("ERROR : Parsing CSV at line " + to_string(counter+1), type::ERROR);
                println("-> Value out of range !", type::ERROR);
                return FAIL;
            }
            if (row.back() <= 0) {
                println("ERROR : Parsing CSV at line " + to_string(counter+1), type::ERROR);
                println("-> Nodes and weights must be integers and > 0 ", type::ERROR);
                return FAIL;
            }
        }

        // Check if the line has the correct number of values
        if (row.size() != 3) {
            println("ERROR : Parsing CSV at line " + to_string(counter+1), type::ERROR);
            println("-> Expected 3 values but read " + to_string(row.size()), type::ERROR);
            return FAIL;
        }
        //Check if nodes and weight are strictly positive integers and store them in int variables
        // Use try-catch to avoid invalid inputs
        int nodeA  = row[0];
        int nodeB  = row[1];
        int weight = row[2];
        allEdges.push_back({nodeA, nodeB, weight});
        allEdges.push_back({nodeB, nodeA, weight});

        // Keep track of the maximum node value (will be the nodes quantity)
        Graph.nodes_qty = max(Graph.nodes_qty, max(nodeA, nodeB));

        // Show progression and count lines
        counter++;
        progression = counter * 100 / CSV_LINES;
        if (progression != progression_backup) {
            print("\rLoading the CSV file into memory ... " + to_string(progression) + " %", type::INFO);
            progression_backup = progression;
        }
    }
    csv_file.close();

    println("\rLoading the CSV file into memory ... 100 %", type::VALIDATION);

    // Sort edges to then enable contigous access of the neighbor of each node
    print("Sorting edges ... ");
    sort(allEdges.begin(), allEdges.end(), 
         [](const FullEdge& a, const FullEdge& b) {
             return a.source < b.source;
         });
    println("\rSorting edges ... Done !", type::VALIDATION);

    print("Organizing data ... ");
    // Resize adjacency_start to create each node index [x] and [x+1]
    Graph.adjacency_start.resize(Graph.nodes_qty + 2);

    // store the number of neighbors for each node (to then define the indexes for the edges)
    for (auto &edge : allEdges) {
        // Add 1 to the number of neighbors for the source node
        Graph.adjacency_start[edge.source]++;  
    }
    // Now adjacency_start[u] = number of neighbors from node u

    // Increase index by the number of edges of the previous one, 
    // so adjacency_start[u] = index where edges for u begin in the edges array of the Graph
    Graph.adjacency_start[0] = 0;
    int_pair temp = {0, 0};
    for (int u = 1; u <= Graph.nodes_qty+1; u++) {
        temp.first = Graph.adjacency_start[u];
        Graph.adjacency_start[u] = Graph.adjacency_start[u-1] + temp.second;
        temp.second = temp.first;
    }
    // Now adjacency_start[u] = index where edges for u begin in the edges array of the Graph
    
    // Resizes edges vector of the Graph to enable improvised access[x]
    Graph.edges.resize(allEdges.size());

    // temporary 'writePosition' will be used to know where to write the next edge of a given node
    vector<int> writePosition(Graph.nodes_qty+1, 0);
    // Initialize each writePosition[u] = adjacency_start[u], 
    // so we know where to place the first edge for a given node u
    for (int u = 0; u < Graph.nodes_qty+1; u++) {
        writePosition[u] = Graph.adjacency_start[u];
    }

    // Fill Graph.edges in sorted order at the correct position
    for (FullEdge edge : allEdges) {
        // we extract the position to write in pos and then increment for next edge
        int pos = writePosition[edge.source]++;
        Graph.edges[pos].id = edge.destination;
        Graph.edges[pos].weight = edge.weight;
    }

    println("\rOrganizing data ... Done !", type::VALIDATION);

    return SUCCESS;
}

void loadGraph(Graph& Graph, Files& Files, bool force = false) {

    // Reset the Graph data
    Graph.adjacency_start.clear();
    Graph.edges.clear();
    Graph.nodes_qty = 0;
    Graph.loaded = false;
    Graph.landmarks_loaded = false;
    // Clear all the vectors
    Graph.edges.clear();
    Graph.landmarks.clear();
    Graph.adjacency_start.clear();
    Graph.landmark_distance.clear();
    // Free memory from the vectors
    Graph.edges.shrink_to_fit();
    Graph.landmarks.shrink_to_fit();
    Graph.adjacency_start.shrink_to_fit();
    Graph.landmark_distance.shrink_to_fit();
    
    // If not forcing a rebuild from CSV, try to load from binary backup
    if (!force) {
        print("\n\nLoading the Graph from the backup " + Files.graph.full + " ... ");
        // Check if the backup exists
        ifstream test(Files.graph.full, ios::binary);
        if (!test.is_open()) {
            println("\nBackup not found !", type::WARNING);
        } else {
            test.close();

            // Load the backup of the Graph
            if (!loadGraphFromBinary(Graph, Files)) {
                println("\nFailed to load the backup !", type::ERROR);
            } else {
                println("\rLoading the Graph from the backup " + Files.graph.full + " ... Done !", type::VALIDATION);
                if (!loadLandmarks(Graph, Files, force)) {
                    println("Landmarks loading failed. Exiting... ", type::ERROR);
                } 
                Graph.loaded = true;
                return;
            }
        }
    }

    // Otherwise, build the Graph from CSV file directly
    requireDataset(Files);

    // Build the Graph from CSV
    if(!buildGraphFromCSV(Graph, Files)) {
        println("Failed to build the graph from CSV !", type::ERROR);
        println("Check the content of your CSV file and try again ... ", type::ERROR);
        return;
    }

    // Save it to binary backup for next time
    if(!saveGraphToBinary(Graph, Files)){
        println("Failed to save the graph to binary !", type::ERROR);
    } else {
        if (!loadLandmarks(Graph, Files, force)) {
            println("Landmarks loading failed. Exiting... ", type::ERROR);
        } 
        Graph.loaded = true;
        return;
    }

    
}

#endif