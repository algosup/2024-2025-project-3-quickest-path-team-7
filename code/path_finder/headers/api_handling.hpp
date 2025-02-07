#ifndef API_HANDLING_HPP
#define API_HANDLING_HPP

#include "header.hpp"

void handle_path_request(int client_socket, const string& request, int option = LIGHT) {

    cout << "\n\nFind path from API:" << endl;
    // Extract parameters
    size_t start_pos = request.find("?start=");
    size_t end_pos = request.find("&end=");
    size_t format_pos = request.find("&format=");
    size_t limit_pos = request.find(" HTTP");

    // Default format = JSON
    response_format = "json";
    if (format_pos != string::npos) {
        response_format = request.substr(format_pos + 8, limit_pos - format_pos - 8);
        if (response_format != "json" && response_format != "xml") {
            cout << "Invalid response format" << endl;
            send_wrong_format(client_socket);
            return;
        }
    }
    cout << "Response format: " << response_format << endl;

    // Check if the start and end parameters are present
    if (start_pos == string::npos || end_pos == string::npos) {
        send_error(client_socket, 400, MISSING_PARAMETER);
        return;
    }

    // Structure for the parameters handling
    vector<pair<string, int>> parameters(2); // {string_content, int_value} 2 parameters inside

    // Extract the start and end parameters
    parameters[0].first = request.substr(start_pos + 7, end_pos - start_pos - 7);
    if (format_pos != string::npos) {
        parameters[1].first = request.substr(end_pos + 5, format_pos - end_pos - 5);
    } else { 
        parameters[1].first = request.substr(end_pos + 5, limit_pos - end_pos - 5);
    }
 
    // Complete check of the parameters validity
    for (auto& [raw_parameter, int_value] : parameters) {
        
        // Remove URL encoding for spaces ("%20")
        size_t temp_pos;
        while ((temp_pos = raw_parameter.find("%20")) != string::npos) {
            raw_parameter.erase(temp_pos, 3);
        }

        // check if either the start or end parameters are not integers
        if (raw_parameter.find(".") != string::npos || 
            raw_parameter.find(",") != string::npos || 
            raw_parameter.find("-") != string::npos )
        {
            send_error(client_socket, 400, INVALID_PARAMETER, raw_parameter);
            return;
        }

        try {
            int_value = stoi(raw_parameter);
        } catch (const invalid_argument& e) {
            send_error(client_socket, 400, INVALID_PARAMETER, raw_parameter);
        } catch (const out_of_range& e) {
            send_error(client_socket, 404, NODE_OUT_OF_RANGE, raw_parameter);
        }

        // Check of the node validity in graph
        if (int_value < 1 || int_value > g_graph.nodes_qty) {
            send_error(client_socket, 404, NODE_OUT_OF_RANGE, to_string(int_value));
            return;
        }
        if (g_graph.adjacency_start[int_value] == g_graph.adjacency_start[int_value + 1]) {
            send_error(client_socket, 404, NODE_NOT_FOUND, to_string(int_value));
            return;
        }
    }

    g_path.start = parameters[0].second;
    g_path.end   = parameters[1].second;

    if (g_path.start == g_path.end) {
        send_error(client_socket, 400, SAME_NODE, to_string(g_path.start));
        return;
    }

    // display the parameters in the console
    cout << "Start node         : " << g_path.start << endl;
    cout << "End node           : " << g_path.end << "\n" << endl;

    {
        lock_guard<mutex> lock(graph_path_file_access);

        // Calculate the shortest path, output results and reset the data
        find_path(g_graph, g_path, g_astar, g_timer);
        
        if      (option == LIGHT) {
            send_path(g_path, client_socket);
            displayResults(g_path);
            savePathToCSV(g_files, g_path);
        } 
        else if (option == DEBUG) {   
            send_full_path(g_path, client_socket);
            displayResults(g_path);
            savePathToCSV(g_files, g_path);
        } 
        else if (option == COMPARE) {
            Path dijkstra_path;
            Astar dijkstra_struct;
            Timer dijkstra_timer;
            reset_compute_data(g_graph, dijkstra_path, dijkstra_struct); // serves as initialization too
            dijkstra_path.start = g_path.start;
            dijkstra_path.end = g_path.end;
            find_path(g_graph, dijkstra_path, dijkstra_struct, dijkstra_timer, NULL_HEURISTIC);
            send_compared_path(g_path, dijkstra_path, client_socket);
            display_comparison_results(g_path, dijkstra_path);
            saveComparedPathToCSV(g_files, g_path, dijkstra_path);
        } 
        else {
            cout << "Invalid option" << endl;
        } 

        reset_compute_data(g_graph, g_path, g_astar);
    }   
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
        send_cmd_error(client_socket, 400, MISSING_PARAMETER);
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
    close_socket(client_socket);
        
}

void handle_request(int client_socket) {

    if (kill_api.load()) {
        close_socket(client_socket);
        return;
    }

    char buffer[4096] = {0};
    #ifdef _WIN32
        int bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0); // Windows uses recv()
    #else
        int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1); // Unix uses read()
    #endif
    string request(buffer);

    // Ensure request is a GET request
    if (request.find("GET") == string::npos) {
        display_bad_requests ? cout << "\nBAD REQUEST :\n" << request << endl : cout << "";
        send_error(client_socket, 405);
        return;
    }

    // Determine if request is for /path or /cmd
    if (request.find("GET /favicon.ico") != string::npos) {
        display_valid_requests ? cout << "\nVALID REQUEST :\n" << request << endl : cout << "";
        send_favicon(client_socket);
    } 
    else if (request.find("GET /cmd?") != string::npos) {
        display_valid_requests ? cout << "\nVALID REQUEST :\n" << request << endl : cout << "";
        handle_cmd_request(client_socket, request);
    } 
    else if (request.find("GET /path?") != string::npos) {
        display_valid_requests ? cout << "\nVALID REQUEST :\n" << request << endl : cout << "";
        endpoint_adaptation = "";
        handle_path_request(client_socket, request, LIGHT);
    } 
    else if (request.find("GET /debug_path?") != string::npos) {
        display_valid_requests ? cout << "\nVALID REQUEST :\n" << request << endl : cout << "";
        endpoint_adaptation = "debug_";
        handle_path_request(client_socket, request, DEBUG);
    } 
    else if (request.find("GET /comp_path?") != string::npos) {
        display_valid_requests ? cout << "\nVALID REQUEST :\n" << request << endl : cout << "";
        endpoint_adaptation = "comp_";
        handle_path_request(client_socket, request, COMPARE);
    } 
    else {
        send_endpoint_error(client_socket); // Bad request
    }

    cout << "\n\n\nEnter a command or the start node : " << flush;

}

void run_api_server() {

    kill_api.store(false);

    cout << "Starting the API service..." << endl;
    api_ready.store(false);
    int server_fd = -1;

    while (!api_ready.load()) {

        this_thread::sleep_for(chrono::milliseconds(100));

        #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            cerr << "WSAStartup failed" << endl;
            return;
        }
        #endif

        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            perror("socket failed");
            continue;
        }

        sockaddr_in address;
        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (::bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == -1) {
            //perror("bind failed");
            //close_socket(server_fd);
            continue;
        }

        if (listen(server_fd, 100) < 0) {
            perror("listen failed");
            close_socket(server_fd);
            continue;
        }

        {
            lock_guard<mutex> lock(graph_path_file_access);
            reset_compute_data(g_graph, g_path, g_astar);
        }

        cout << "API server running on port " << port << endl;
        cout << "Try the longest path -> http://localhost:" << port << "/path?start=9588784&end=2720178" << endl;

        api_ready.store(true);
        
    }


    while (!kill_api.load()) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }

        thread(handle_request, client_socket).detach();

    }

    close_socket(server_fd);

#ifdef _WIN32
    WSACleanup();
#endif

}

#endif