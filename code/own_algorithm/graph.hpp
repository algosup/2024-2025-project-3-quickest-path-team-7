#ifndef GRAPH_H
#define GRAPH_H

#include "header.hpp"

void saveGraphToBinary(const Graph& graph, const Files& files) {
    ofstream ofs(files.map_backup, ios::binary);
    if (!ofs.is_open()) {
        cerr << "Failed to open file for writing: " << files.map_backup << endl;
        return;
    }

    // 1) Write map_size
    ofs.write(reinterpret_cast<const char*>(&graph.map_size), sizeof(graph.map_size));

    // 2) Write adjacency_start array size + contents
    //    size = (graph.map_size + 1)
    int startArraySize = graph.adjacency_start.size();
    ofs.write(reinterpret_cast<const char*>(&startArraySize), sizeof(startArraySize));
    ofs.write(reinterpret_cast<const char*>(graph.adjacency_start.data()),
              startArraySize * sizeof(int));

    // 3) Write edges array size + contents
    int edgesSize = graph.edges.size();
    ofs.write(reinterpret_cast<const char*>(&edgesSize), sizeof(edgesSize));
    ofs.write(reinterpret_cast<const char*>(graph.edges.data()),
              edgesSize * sizeof(Edge));

    ofs.close();
}

// Load compressed adjacency data from the binary file
void loadGraphFromBinary(Graph& graph, const Files& files) {
    ifstream ifs(files.map_backup, ios::binary);
    if (!ifs.is_open()) {
        cerr << "Failed to open file for reading: " << files.map_backup << endl;
        graph.loaded = false;
        return;
    }

    // 1) Read map_size
    ifs.read(reinterpret_cast<char*>(&graph.map_size), sizeof(graph.map_size));

    // 2) Read adjacency_start
    int startArraySize = 0;
    ifs.read(reinterpret_cast<char*>(&startArraySize), sizeof(startArraySize));
    graph.adjacency_start.resize(static_cast<int>(startArraySize));
    ifs.read(reinterpret_cast<char*>(graph.adjacency_start.data()),
             startArraySize * sizeof(int));

    // 3) Read edges
    int edgesSize = 0;
    ifs.read(reinterpret_cast<char*>(&edgesSize), sizeof(edgesSize));
    graph.edges.resize(static_cast<int>(edgesSize));
    ifs.read(reinterpret_cast<char*>(graph.edges.data()),
             edgesSize * sizeof(Edge));

    ifs.close();
    graph.loaded = true;

}

// Temporary structure to store edges before compressing
struct TempEdge {
    int src; // source
    int dst; // destination
    int dist; // distance
};

// CSV_LINES is presumably the total lines in your CSV
// or you can read until EOF with a simpler loop.
void buildGraphFromCSV(Graph& graph, const Files& files) {
    ifstream csv_file(files.dataset);
    if (!csv_file.is_open()) {
        cerr << "Cannot open dataset: " << files.dataset << endl;
        graph.loaded = false;
        return;
    }

    // We'll parse the CSV and store edges
    vector<TempEdge> allEdges;
    allEdges.reserve(NODE_MAX_VALUE); // Reserve some guess if you know approx # of lines

    string line;
    unsigned int counter = 0;
    unsigned int progression = 0;
    unsigned int progression_backup = 0;

    while (getline(csv_file, line)) {
        if (line.empty()) continue;
        // CSV format: node1, node2, distance
        // Example: 123,456,999
        // We'll parse them
        stringstream ss(line);
        string cell;
        vector<int> row;
        while (getline(ss, cell, ',')) {
            row.push_back(stoi(cell));
        }
        if (row.size() != 3) continue;

        int node1 = row[0];
        int node2 = row[1];
        int dist  = row[2];

        // Insert in both directions for an undirected graph
        allEdges.push_back({node1, node2, dist});
        allEdges.push_back({node2, node1, dist});

        counter++;
        progression = counter * 100 / CSV_LINES;
        if (progression != progression_backup) {
            cout << "\rLoading the CSV file into memory ... " << progression << " %" << flush;
            progression_backup = progression;
        }
    }
    csv_file.close();

    // Now we know how many nodes we have:
    // map_size = largest ID + 1
    graph.map_size = NODE_MAX_VALUE + 1;

    // Sort edges by src
    sort(allEdges.begin(), allEdges.end(), 
         [](const TempEdge& a, const TempEdge& b) {
             return a.src < b.src;
         });

    // Resize adjacency_start to (map_size + 1)
    graph.adjacency_start.resize(graph.map_size, 0);

    // We’ll build 'graph.edges' from 'allEdges'
    graph.edges.resize(allEdges.size());

    // Pass 1: Count how many edges each node has
    for (auto &e : allEdges) {
        // e.src is the node
        graph.adjacency_start[e.src]++;  
    }
    // Now adjacency_start[u] = number of edges from node u
    // We need prefix sums so adjacency_start[u] = index where edges for u begin

    // prefix sum
    graph.adjacency_start[0] = 0;
    int_pair temp = {0, 0};
    for (int u = 1; u <= NODE_MAX_VALUE; u++) {
        temp.first = graph.adjacency_start[u];
        graph.adjacency_start[u] = graph.adjacency_start[u-1] + temp.second;
        temp.second = temp.first;
    }
    // So adjacency_start[u] now is the cumulative count of edges up to node u-1
    // adjacency_start[u+1] - adjacency_start[u] = # of edges from node u

    // Pass 2: Place edges in the correct slot
    // We'll keep a temporary array of "write positions" so we know where to put each edge
    vector<int> writePosition(NODE_MAX_VALUE);
    for (int u = 0; u < graph.map_size; u++) {
        writePosition[u] = graph.adjacency_start[u];
    }

    // Fill graph.edges in sorted order
    for (auto &e : allEdges) {
        int pos = writePosition[e.src]++;
        graph.edges[pos].id = e.dst;
        graph.edges[pos].weight = e.dist;
    }

    // adjacency_start[map_size] should be = allEdges.size()
    graph.adjacency_start[graph.map_size] = allEdges.size();

    // Mark loaded
    graph.loaded = true;

}

// Build the graph (from backup if exists, else from CSV)
void buildGraph(Graph& graph, Files& files, bool force = false) {
    // If not forcing a rebuild, try to load from binary
    if (!force) {
        ifstream test(files.map_backup, ios::binary);
        if (test.is_open()) {
            test.close();
            loadGraphFromBinary(graph, files);
            if (graph.loaded) {
                cout << "Loaded graph from " << files.map_backup << "!" << endl;
                return;
            }
        }
    }

    // Otherwise, read the CSV and build from scratch
    cout << "Building graph from " << files.dataset << "..." << endl;
    buildGraphFromCSV(graph, files);
    if (!graph.loaded) {
        cerr << "Error: graph not built from CSV." << endl;
        return;
    }

    // Save to binary for next time
    saveGraphToBinary(graph, files);
    cout << "\nGraph saved to binary " << files.map_backup << endl;
}

void loadGraph(Graph& graph, Files& files, bool force = true) {
    buildGraph(graph, files, force);

    if (!graph.loaded) {
        // In your code, you do a loop prompting user to "retry" or specify a new CSV path
        // We'll omit that logic here, but you'd do it similarly if (graph.loaded == false).
        cerr << "Graph not loaded. Provide new CSV or fix the file paths." << endl;
        return;
    }

}

#endif