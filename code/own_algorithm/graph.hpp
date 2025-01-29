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

    vector<TempEdge> allEdges;
    // If you know the approximate number of lines, reserve that many (not NODE_MAX_VALUE):
    // allEdges.reserve(some_estimated_line_count);

    string line;
    unsigned int counter = 0;
    unsigned int progression = 0;
    unsigned int progression_backup = 0;

    while (getline(csv_file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        vector<int> row;
        {
            string cell;
            while (getline(ss, cell, ',')) {
                row.push_back(stoi(cell));
            }
        }
        if (row.size() != 3) continue;

        int node1 = row[0];
        int node2 = row[1];
        int dist  = row[2];

        // Insert in both directions for an undirected graph
        allEdges.push_back({node1, node2, dist});
        allEdges.push_back({node2, node1, dist});

        counter++;
        progression = counter * 100 / CSV_LINES; // your known CSV_LINES
        if (progression != progression_backup) {
            cout << "\rLoading the CSV file into memory ... " << progression << " %" << flush;
            progression_backup = progression;
        }
    }
    csv_file.close();

    // Suppose you do:
    //    graph.map_size = NODE_MAX_VALUE + 1;
    // If you discovered the max node on-the-fly, you'd do:
    //    int maxNodeID = 0; // track max while reading
    //    graph.map_size = maxNodeID + 1;
    graph.map_size = NODE_MAX_VALUE + 1;

    // Sort edges by src
    sort(allEdges.begin(), allEdges.end(), 
         [](const TempEdge& a, const TempEdge& b) {
             return a.src < b.src;
         });

    // 1) Resize adjacency_start to (map_size + 1), init to 0
    graph.adjacency_start.resize(graph.map_size + 1, 0);

    // 2) Count how many edges each node has
    for (auto &e : allEdges) {
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
    // Now adjacency_start[u] is the *cumulative* sum up to node u-1
    // adjacency_start[map_size] must be set after the final prefix sum:
    graph.adjacency_start[graph.map_size] = static_cast<int>(allEdges.size());

    // 4) Allocate edges
    graph.edges.resize(allEdges.size());

    // We'll keep a temporary array of "write positions"
    vector<int> writePosition(graph.map_size, 0);
    // Initialize each writePosition[u] = adjacency_start[u], 
    // so we know where to place the next edge for node u
    for (int u = 0; u < graph.map_size; u++) {
        writePosition[u] = graph.adjacency_start[u];
    }

    // 5) Fill graph.edges in sorted order
    for (auto &e : allEdges) {
        int pos = writePosition[e.src]++;
        graph.edges[pos].id = e.dst;
        graph.edges[pos].weight = e.dist;
    }

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

void loadGraph(Graph& graph, Files& files, bool force = false) {
    buildGraph(graph, files, force);

    if (!graph.loaded) {
        // In your code, you do a loop prompting user to "retry" or specify a new CSV path
        // We'll omit that logic here, but you'd do it similarly if (graph.loaded == false).
        cerr << "Graph not loaded. Provide new CSV or fix the file paths." << endl;
        return;
    }

}

#endif