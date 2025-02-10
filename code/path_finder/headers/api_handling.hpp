#ifndef API_HANDLING_HPP
#define API_HANDLING_HPP

#include "header.hpp"

void handlePathRequest(int client_socket, const string& request, int option = LIGHT) {

    println("\n\nFind path from API:");
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
            println("Invalid response format", type::ERROR);
            sendWrongFormat(client_socket);
            return;
        }
    }
    print("Response format: ");
    println(response_format, type::INFO);

    // Check if the start and end parameters are present
    if (start_pos == string::npos || end_pos == string::npos) {
        sendError(client_socket, 400, MISSING_PARAMETER);
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
            sendError(client_socket, 400, INVALID_PARAMETER, raw_parameter);
            return;
        }

        try {
            int_value = stoi(raw_parameter);
        } catch (const invalid_argument& e) {
            sendError(client_socket, 400, INVALID_PARAMETER, raw_parameter);
        } catch (const out_of_range& e) {
            sendError(client_socket, 404, NODE_OUT_OF_RANGE, raw_parameter);
        }

        // Check of the node validity in graph
        if (int_value < 1 || int_value > GlobalGraph.nodes_qty) {
            sendError(client_socket, 404, NODE_OUT_OF_RANGE, to_string(int_value));
            return;
        }
        if (GlobalGraph.adjacency_start[int_value] == GlobalGraph.adjacency_start[int_value + 1]) {
            sendError(client_socket, 404, NODE_NOT_FOUND, to_string(int_value));
            return;
        }
    }

    GlobalPath.start = parameters[0].second;
    GlobalPath.end   = parameters[1].second;

    if (GlobalPath.start == GlobalPath.end) {
        sendError(client_socket, 400, SAME_NODE, to_string(GlobalPath.start));
        return;
    }

    // display the parameters in the console
    print("Start node         : ");
    println(GlobalPath.start, type::INFO);
    print("End node           : ");
    println(GlobalPath.end, type::INFO);

    {
        lock_guard<mutex> lock(graph_path_file_access);

        // Calculate the shortest path, output results and reset the data
        findPath(GlobalGraph, GlobalPath, GlobalAstar, GlobalTimer);
        
        if      (option == LIGHT) {
            sendPath(GlobalPath, client_socket);
            displayResults(GlobalPath);
            savePathToCSV(GlobalFiles, GlobalPath);
        } 
        else if (option == DEBUG) {   
            sendFullPath(GlobalPath, client_socket);
            displayResults(GlobalPath);
            savePathToCSV(GlobalFiles, GlobalPath);
        } 
        else if (option == COMPARE) {
            Path DijkstraPath;
            Astar DijkstraStruct;
            Timer DijkstraTimer;
            resetComputeData(GlobalGraph, DijkstraPath, DijkstraStruct); // serves as initialization too
            DijkstraPath.start = GlobalPath.start;
            DijkstraPath.end = GlobalPath.end;
            findPath(GlobalGraph, DijkstraPath, DijkstraStruct, DijkstraTimer, NULL_HEURISTIC);
            sendComparedPath(GlobalPath, DijkstraPath, client_socket);
            displayComparisonResults(GlobalPath, DijkstraPath);
            saveComparedPathToCSV(GlobalFiles, GlobalPath, DijkstraPath);
        } 
        else {
            println("Invalid option", type::ERROR);
        } 

        resetComputeData(GlobalGraph, GlobalPath, GlobalAstar);
    }   
}

void handleCmdRequest(int client_socket, const string& request) {
    // Extract command parameter
    size_t cmd_pos = request.find("?command=");
    size_t format_pos = request.find("&format=");
    size_t limit_pos = request.find(" HTTP");

    // Default format = JSON
    response_format = "json";
    if (format_pos != string::npos) {
        response_format = request.substr(format_pos + 8, limit_pos - format_pos - 8);
        if (response_format != "json" && response_format != "xml") {
            sendWrongFormat(client_socket);
            return;
        }
    }
    print("Response format: ");
    println(response_format, type::INFO);

    if (cmd_pos == string::npos) {
        sendCmdError(client_socket, 400, MISSING_PARAMETER);
        return;
    }

    // extract the command parameter
    string command;
    if (format_pos != string::npos) {
        command = request.substr(cmd_pos + 9, format_pos - cmd_pos - 9);
    } else { 
        command = request.substr(cmd_pos + 9, limit_pos - cmd_pos - 9);
    }

    print("Command: ");
    println(command, type::INFO);

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
    closeSocket(client_socket);
        
}

void handleRequest(int client_socket) {

    if (kill_api.load()) {
        closeSocket(client_socket);
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
        display_bad_endpoints ? print("\nBAD REQUEST :\n" + request, type::ERROR_LIGHT) : print("");
        sendError(client_socket, 405);
        return;
    }

    // Determine if request is for /path or /cmd
    if (request.find("GET /favicon.ico") != string::npos) {
        display_valid_endpoints ? println("\n\nVALID ENDPOINT :\n" + request, type::GREEN) : print("");
        sendFavicon(client_socket);
    } 
    else if (request.find("GET /cmd?") != string::npos) {
        display_valid_endpoints ? println("\n\nVALID ENDPOINT :\n" + request, type::GREEN) : print("");
        handleCmdRequest(client_socket, request);
    } 
    else if (request.find("GET /path?") != string::npos) {
        display_valid_endpoints ? println("\n\nVALID ENDPOINT :\n" + request, type::GREEN) : print("");
        endpoint_adaptation = "";
        handlePathRequest(client_socket, request, LIGHT);
    } 
    else if (request.find("GET /debug_path?") != string::npos) {
        display_valid_endpoints ? println("\n\nVALID ENDPOINT :\n" + request, type::GREEN) : print("");
        endpoint_adaptation = "debug_";
        handlePathRequest(client_socket, request, DEBUG);
    } 
    else if (request.find("GET /comp_path?") != string::npos) {
        display_valid_endpoints ? println("\n\nVALID ENDPOINT :\n" + request, type::GREEN) : print("");
        endpoint_adaptation = "comp_";
        handlePathRequest(client_socket, request, COMPARE);
    } 
    else {
        display_bad_endpoints ? println("\n\nBAD REQUEST :\n" + request, type::ERROR_LIGHT) : print("");
        sendEndpointError(client_socket); // Bad request
    }

    println("\n\n\nEnter a command or the start node : ");

}

void runApiServer() {

    kill_api.store(false);

    println("Starting the API service...");
    api_ready.store(false);
    int server_fd = -1;

    while (!api_ready.load()) {

        this_thread::sleep_for(chrono::milliseconds(100));

        #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            println("WSAStartup failed", type::ERROR);
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
            closeSocket(server_fd);
            continue;
        }

        if (listen(server_fd, 100) < 0) {
            perror("listen failed");
            closeSocket(server_fd);
            continue;
        }

        {
            lock_guard<mutex> lock(graph_path_file_access);
            resetComputeData(GlobalGraph, GlobalPath, GlobalAstar);
        }

        println("API server running on port " + to_string(port), type::VALIDATION);
        println("Try the longest path -> http://localhost:" + to_string(port) + "/path?start=9588784&end=2720178", type::INFO);

        api_ready.store(true);
        
    }


    while (!kill_api.load()) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            //perror("accept failed");
            continue; // Break the loop on other errors
        }

        thread([client_socket]() {
            handleRequest(client_socket);
            closeSocket(client_socket); // Ensure the socket is closed after handling the request
        }).detach();
    }

    closeSocket(server_fd);

#ifdef _WIN32
    WSACleanup();
#endif

}

#endif