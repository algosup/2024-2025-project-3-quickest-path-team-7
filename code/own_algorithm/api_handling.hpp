#ifndef API_HANDLING_HPP
#define API_HANDLING_HPP

#include "header.hpp"

void handle_path_request(int client_socket, const string& request) {

    // Extract parameters
    size_t start_pos = request.find("?start=");
    size_t end_pos = request.find("&end=");
    size_t format_pos = request.find("&format=");
    size_t limit_pos = request.find(" HTTP");

    // Default to JSON if format parameter is not provided
    response_format = "json";
    if (format_pos != string::npos) {
        cout << limit_pos - format_pos - 8 << endl;
        response_format = request.substr(format_pos + 8, limit_pos - format_pos - 8);

        // Validate format (only accept "json" or "xml")
        if (response_format != "json" && response_format != "xml") {
            cout << "Invalid response format: " << response_format << endl;
            send_error(client_socket, 400, 2);
            return;
        }
    }

    cout << "Response format: " << response_format << endl;

    // Check if the start and end parameters are present
    if (start_pos == string::npos || end_pos == string::npos) {
        send_error(client_socket, 400, 1);
        return;
    }

    string start_param = request.substr(start_pos + 7, end_pos - start_pos - 7);
    string end_param;
    if (format_pos != string::npos) {
        end_param = request.substr(end_pos + 5, format_pos - end_pos - 5);
    } else { 
        end_param = request.substr(end_pos + 5, limit_pos - end_pos - 5);
    }

    // Remove URL encoding for spaces ("%20")
    size_t temp_pos;
    while ((temp_pos = start_param.find("%20")) != string::npos) {
        start_param.erase(temp_pos, 3);
    }
    while ((temp_pos = end_param.find("%20")) != string::npos) {
        end_param.erase(temp_pos, 3);
    }


    // display the parameters in the console
    cout << "Start: " << start_param << "\nEnd: " << end_param << "\n" << endl;

    // check if either the start or end parameters are not integers
    if (start_param.find(".") != string::npos || 
        start_param.find(",") != string::npos || 
        start_param.find("-") != string::npos )
    {
        send_error(client_socket, 400, 2, start_param);
        return;
    }
    if (end_param.find(".") != string::npos || 
        end_param.find(",") != string::npos || 
        end_param.find("-") != string::npos )
    {
        send_error(client_socket, 400, 2, end_param);
        return;
    }

    // convert the parameters to integers
    try { 
        g_path.start = stoi(start_param);
    } 
    catch (const exception& e) {
        send_error(client_socket, 400, 2, start_param);
    }
    try {
        g_path.end = stoi(end_param);
    }
    catch (const exception& e) {
        send_error(client_socket, 400, 2, end_param);
    }

    // Check of the node validity in graph
    if (g_path.start < 1 || g_path.start > g_graph.nodes_qty) {
        send_error(client_socket, 404, 2, to_string(g_path.start));
        return;
    }
    if (g_path.end < 1 || g_path.end > g_graph.nodes_qty) {
        send_error(client_socket, 404, 2, to_string(g_path.end));
        return;
    }
    if (g_graph.adjacency_start[g_path.start] == g_graph.adjacency_start[g_path.start + 1]) {
        send_error(client_socket, 404, 1, to_string(g_path.start));
        return;
    }
    if (g_graph.adjacency_start[g_path.end] == g_graph.adjacency_start[g_path.end + 1]) {
        send_error(client_socket, 404, 1, to_string(g_path.end));
        return;
    }
    if (g_path.start == g_path.end) {
        send_error(client_socket, 401, 3, to_string(g_path.end));
        return;
    }
    
    // Calculate the shortest path, output results and reset the data
    find_path(g_graph, g_path, g_astar, g_timer);
    send_path(g_path, client_socket);
    savePathToCSV(g_graph, g_files, g_path);
    reset_compute_data(g_graph, g_path, g_astar);

    close(client_socket);
    
}

void handle_cmd_request(int client_socket, const string& request) {
    // Extract command parameter
    size_t cmd_pos = request.find("?command=");
    size_t format_pos = request.find("&format=");
    size_t limit_pos = request.find(" HTTP");

    // Default format = JSON
    response_format = "json";
    if (format_pos != string::npos) {
        response_format = request.substr(format_pos + 8, limit_pos - format_pos - 8);
        if (response_format != "json" && response_format != "xml") {
            send_wrong_format(client_socket);
            return;
        }
    }
    cout << "Response format: " << response_format << endl;

    if (cmd_pos == string::npos) {
        send_cmd_error(client_socket, 400, 1);
        return;
    }

    // extract the command parameter
    string command;
    if (format_pos != string::npos) {
        command = request.substr(cmd_pos + 9, format_pos - cmd_pos - 9);
    } else { 
        command = request.substr(cmd_pos + 9, limit_pos - cmd_pos - 9);
    }

    cout << "Command: " << command << endl;

    // Redirect each command to its function
    stringstream ss;

    if (response_format == "xml") {
        ss  << "HTTP/1.1 200 OK\nContent-Type: application/xml\n\n"
            << "<response>\n"
            << "    <command>" << command << "</command>\n"
            << "    <response>Commands are not yet handled by the server.</response>\n"
            << "</response>\n";
    } else {
        ss  << "HTTP/1.1 200 OK\nContent-Type: application/" << response_format << "\n\n"
            << "{\n"
            << "    \"command\": \"" << command << "\",\n"
            << "    \"response\": \"Commands are not yet handled by the server.\"\n"
            << "}\n";
    }
    
    string response = ss.str();
    send(client_socket, response.c_str(), response.size(), 0);
    close(client_socket);
        
}

void handle_request(int client_socket) {
    cout << "New client connected" << endl;

    char buffer[4096] = {0};
    read(client_socket, buffer, 4096);
    string request(buffer);

    cout << "Request: \n" << request << endl;

    // Ensure request is a GET request
    if (request.find("GET") == string::npos) {
        send_error(client_socket, 405); // Method Not Allowed
        return;
    }

    // Determine if request is for /path or /cmd
    if (request.find("GET /path?") != string::npos) {
        handle_path_request(client_socket, request);
    } else if (request.find("GET /cmd?") != string::npos) {
        handle_cmd_request(client_socket, request);
    } else {
        send_endpoint_error(client_socket); // Bad request
    }
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
    address.sin_port = htons(PORT);
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

    cout << "API server running on port " << PORT << endl;
    cout << "-> http://localhost:" << PORT << "/path?start=9588784&end=2720178" << endl;

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

#endif