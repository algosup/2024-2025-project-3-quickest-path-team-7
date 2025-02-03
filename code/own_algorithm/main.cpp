#include "header.hpp"
// Variables globales partagées

void handle_request(int client_socket) {

    cout << "New client connected" << endl;

    char buffer[4096] = {0};
    read(client_socket, buffer, 4096);
    string request(buffer);
    size_t param_pos = request.find("?start=");
    size_t end_pos = request.find("&end=");

    cout << "Request: \n" << request ;

    if (param_pos == string::npos || end_pos == string::npos) {
        const char* error = "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\n\nInvalid parameters format";
        send(client_socket, error, strlen(error), 0);
        close(client_socket);
        cout << "Invalid parameters format" << endl;
        return;
    }

    try {
        g_path.start = stoi(request.substr(param_pos + 7, end_pos - (param_pos + 7)));
        g_path.end = stoi(request.substr(end_pos + 5));
        // Validation des nœuds
        if (g_path.start < 1 || g_path.end < 1 || g_path.start > g_graph.nodes_qty || g_path.end > g_graph.nodes_qty) {
            const char* error = "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\n\nNode IDs out of valid range";
            send(client_socket, error, strlen(error), 0);
            close(client_socket);
            cout << "Node IDs out of valid range" << endl;
            return;
        }
        
        find_path(g_graph, g_path, g_astar, g_timer);

        send_path(g_path, client_socket);

        reset_compute_data(g_graph, g_path, g_astar);

    }
    catch (const exception& e) {
        const char* error = "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\n\nInvalid parameters format";
        send(client_socket, error, strlen(error), 0);
    }
    close(client_socket);
}
void run_api_server() {

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        return;
    }
    sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    if (::bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == -1) {
        perror("bind failed");
        return;
    }
    if (listen(server_fd, 100) < 0) { // Augmenter la queue
        perror("listen failed");
        return;
    }

    // ensure that the global variables are reset before any calculation
    reset_compute_data(g_graph, g_path, g_astar);

    cout << "API server running on 0.0.0.0:8080" << endl;
    while (true) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }
        thread(handle_request, client_socket).detach();
    }
}
int main() {
    // To allow special characters in cout such as 'µ'
    setlocale(LC_ALL, "en_US.UTF-8");

    cout << "\n\n\n\nWelcome to the path finder !\nIf any problem occurs, type 'help' for a list of commands." << endl;

    // Build the i/o filenames and eventually ask for the folder path
    takeFolderInput(g_files, SKIP);

    reset_compute_data(g_graph, g_path, g_astar);

    while (!g_graph.loaded) {
        switch(loadGraph(g_graph, g_files)) {
            case SUCCESS:
                g_graph.loaded = true;
                break;
            case NO_DATASET:
                takeFolderInput(g_files, ASK_FOLDER);
                break;
            case FAIL:
                cerr << "Error loading graph, exiting..." << endl;
                return 1;
        }
    }

    thread(run_api_server).detach();

    cout << " Example of usage: \nGET /quickest-path?start_id=1&end_id=4" << endl;
    
    // Maintenir le programme actif
    while (true) {
        string command;
        cout << "\nEnter 'exit' to quit: ";
        getline(cin, command);
        if (command == "exit") break;
    }

    return 0;
}