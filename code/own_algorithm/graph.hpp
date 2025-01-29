#ifndef GRAPH_H
#define GRAPH_H

#include "header.hpp"

bool saveGraphToBinary(const Graph& graph, const Files& files) {

    // Open/Create the binary file backup
    ofstream ofs(files.map_backup, ios::binary);
    if (!ofs.is_open()) {
        return FAIL;
    }

    // Write the number of nodes
    ofs.write(reinterpret_cast<const char*>(&graph.nodes_qty), sizeof(graph.nodes_qty));

    // Write adjacency_start array size + contents
    int startArraySize = graph.adjacency_start.size();
    ofs.write(reinterpret_cast<const char*>(&startArraySize), sizeof(startArraySize));
    ofs.write(reinterpret_cast<const char*>(graph.adjacency_start.data()),
              startArraySize * sizeof(int));

    // Write edges array size + contents
    int edgesSize = graph.edges.size();
    ofs.write(reinterpret_cast<const char*>(&edgesSize), sizeof(edgesSize));
    ofs.write(reinterpret_cast<const char*>(graph.edges.data()),
              edgesSize * sizeof(Edge));

    ofs.close();

    return SUCCESS;
}

// Load compressed adjacency data from the binary file
bool loadGraphFromBinary(Graph& graph, const Files& files) {

    // Open the binary file backup
    ifstream ifs(files.map_backup, ios::binary);

    // Read map_size
    ifs.read(reinterpret_cast<char*>(&graph.nodes_qty), sizeof(graph.nodes_qty));

    // Read adjacency_start 
    int startArraySize = 0;
    ifs.read(reinterpret_cast<char*>(&startArraySize), sizeof(startArraySize));
    graph.adjacency_start.resize(static_cast<int>(startArraySize));
    ifs.read(reinterpret_cast<char*>(graph.adjacency_start.data()),
             startArraySize * sizeof(int));

    // Read edges array
    int edgesSize = 0;
    ifs.read(reinterpret_cast<char*>(&edgesSize), sizeof(edgesSize));
    graph.edges.resize(static_cast<int>(edgesSize));
    ifs.read(reinterpret_cast<char*>(graph.edges.data()),
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

bool buildGraphFromCSV(Graph& graph, const Files& files) {

    // Open the CSV file
    ifstream csv_file(files.dataset);
    
    // vector to store all connections of the CSV file
    vector<FullEdge> allEdges;

    string line;
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
            row.push_back(stoi(cell));
        }

        // Check if the line has the correct number of values
        if (row.size() != 3) {
            cout << "Error: Expected 3 values per line, but read " << row.size() << endl;
            return FAIL;
        }

        // Insert in both directions for an undirected graph
        int nodeA  = row[0];
        int nodeB  = row[1];
        int weight = row[2];
        allEdges.push_back({nodeA, nodeB, weight});
        allEdges.push_back({nodeB, nodeA, weight});

        // Keep track of the maximum node value (will be the nodes quantity)
        graph.nodes_qty = max(graph.nodes_qty, max(nodeA, nodeB));

        // Show progression and count lines
        counter++;    
        progression = counter * 100 / CSV_LINES;
        if (progression != progression_backup) {
            cout << "\rLoading the CSV file into memory ... " << progression << " %" << flush;
            progression_backup = progression;
        }
    }
    csv_file.close();

    // Check if the constant is correct
    if (counter != CSV_LINES) {
        cout << "Error: Expected " << CSV_LINES << " lines, but read " << counter << endl;
        return FAIL;
    }

    // Sort edges to then enable contigous access of the neighbor of each node
    sort(allEdges.begin(), allEdges.end(), 
         [](const FullEdge& a, const FullEdge& b) {
             return a.source < b.source;
         });

    // Resize adjacency_start to create each node index [x]
    graph.adjacency_start.resize(graph.nodes_qty + 1);

    // store the number of neighbors for each node (to then define the indexes for the edges)
    for (auto &edge : allEdges) {
        // Add 1 to the number of neighbors for the source node
        graph.adjacency_start[edge.source]++;  
    }
    // Now adjacency_start[u] = number of neighbors from node u

    // Increase index by the number of edges of the previous one, 
    // so adjacency_start[u] = index where edges for u begin in the edges array of the graph
    graph.adjacency_start[0] = 0;
    int_pair temp = {0, 0};
    for (int u = 1; u <= graph.nodes_qty; u++) {
        temp.first = graph.adjacency_start[u];
        graph.adjacency_start[u] = graph.adjacency_start[u-1] + temp.second;
        temp.second = temp.first;
    }
    // Now adjacency_start[u] = index where edges for u begin in the edges array of the graph
    
    // Resizes edges vector of the graph to enable improvised access[x]
    graph.edges.resize(allEdges.size());

    // temporary 'writePosition' will be used to know where to write the next edge of a given node
    vector<int> writePosition(graph.nodes_qty, 0);
    // Initialize each writePosition[u] = adjacency_start[u], 
    // so we know where to place the first edge for a given node u
    for (int u = 0; u < graph.nodes_qty; u++) {
        writePosition[u] = graph.adjacency_start[u];
    }

    // Fill graph.edges in sorted order at the correct position
    for (FullEdge edge : allEdges) {
        // we extract the position to write in pos and then increment for next edge
        int pos = writePosition[edge.source]++;
        graph.edges[pos].id = edge.destination;
        graph.edges[pos].weight = edge.weight;
    }

    return SUCCESS;
}

bool loadGraph(Graph& graph, Files& files, bool force = false) {

    // If not forcing a rebuild from CSV, try to load from binary backup
    if (!force) {
        cout << "\n\nLoading the graph from the backup " << files.map_backup << " ... " << flush;
        
        // Check if the backup exists
        ifstream test(files.map_backup, ios::binary);
        if (!test.is_open()) {
            cout << "\nBackup not found !" << endl;
        } else {
            test.close();

            // Load the backup of the graph
            if (loadGraphFromBinary(graph, files)) {
                cout << "Done !" << endl;
                return SUCCESS;
            } else {
                cout << "\nFailed to read the backup !" << endl;
                return FAIL;
            }
        }
    }

    // Otherwise, build the graph from CSV file directly
    cout << "Building graph from " << files.dataset << " ... " << flush;

    // Check if the dataset exists
    ifstream test(files.dataset);
    if (!test.is_open()) {
        cout << "\nDataset not found !" << endl;
        return FAIL;
    }
    test.close();

    // Build the graph from CSV
    if(buildGraphFromCSV(graph, files)) {
        cout << "Done !" << endl;
    } else {
        cout << "\nFailed to build the graph from CSV !" << endl;
        return FAIL;
    }

    // Save it to binary backup for next time
    if(saveGraphToBinary(graph, files)){
        cout << "Graph saved to " << files.map_backup << " !" << endl;
        return SUCCESS;
    } else {
        cout << "\nFailed to save the graph to binary !" << endl;
        return FAIL;
    }
}

#endif