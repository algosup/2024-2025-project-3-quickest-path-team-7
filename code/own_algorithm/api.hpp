#ifndef API_HPP
#define API_HPP

#include "header.hpp"

/* repsonse format json :
{
    "path": ["Landmark_A", "Landmark_B", "Landmark_C"],
    "travel_time": 120
}
*/

void send_path(Path& g_path, int client_socket) {
    stringstream ss;

        ss << "HTTP/1.1 200 OK\nContent-Type: text/plain\n\n";
        ss << "{\n";
        ss << "    \"path\": [";
        for (int i = 0; i < g_path.path.size(); i++) {
            ss << "\"" << g_path.path[i].first << "\"";
            if (i < g_path.path.size() - 1) {
                ss << ", ";
            }
        }
        ss << "],\n";
        ss << "    \"travel_time\": " << g_path.distance << "\n";
        ss << "}\n";


    string response = ss.str();

    send(client_socket, response.c_str(), response.size(), 0);
}

void send_error(int client_socket, int error_code, int kind = 1, string node = "undefined") {
    stringstream ss;

    switch(error_code) {
        case 400: {
            cout << "ERROR 400 : Bad request" << endl;
            ss  << "HTTP/1.1 400 Bad Request\n"
                << "Content-Type: application/json\n\n"
                << "{\n";
          if (kind == 1) {
            cout << "Missing parameters" << endl;
            ss  << "    \"status\": \"Missing parameter(s).\",\n"
                << "    \"details\": {\n"
                << "        \"missing_parameters\": \"start\" or \"end\" node,\n"
                << "        \"resolution\": \"Ensure both 'source' and 'destination' are included as query parameters.\",\n";
        } if (kind == 2) {
            cout << "Invalid parameter format" << endl;
            ss  << "    \"status\": \"Invalid parameter.\",\n"
                << "    \"details\": {\n"
                << "        \"invalid_parameter\": " << node << ",\n"
                << "        \"resolution\": \"This parameter requires a positive 32bits INTEGER.\",\n";
        }
            ss  << "        \"documentation\": \"https://example.com/docs#parameters\"\n"
                << "        \"example\": \"GET /path?start=5&end=6\"\n"
                << "    }\n"
                << "}\n";

            break;
        }
        case 404: {
            cout << "ERROR 404 : Node not found" << endl;
            ss  << "HTTP/1.1 404 Bad Request\n"
                << "Content-Type: application/json\n\n"
                << "{\n";
          if (kind == 1) {
            cout << "Node "<< node << " doesn't exists" << endl;
            ss  << "    \"status\": \"Node not found.\",\n"
                << "    \"details\": {\n"
                << "        \"landmark_id\": \"" << node << "\",\n"
                << "        \"resolution\": \"Check the dataset for valid node IDs, \n";
        } if (kind == 2) {
            cout << "Node "<< node << " is out of range" << endl;
            ss  << "    \"status\": \"Node out of range.\",\n"
                << "    \"details\": {\n"
                << "        \"landmark_id\": \"" << node << "\",\n"
                << "        \"resolution\": \"Nodes of this dataset are between 1 and " << g_graph.nodes_qty << ", \n";
        }
            ss  << "        \"documentation\": \"https://example.com/docs#landmarks\"\n"
                << "    }\n"
                << "}\n";
            break;
        }
        case 405: {
            ss  << "HTTP/1.1 405 Method Not Allowed\n"
                << "Content-Type: application/json\n\n"
                << "{\n"
                << "    \"status\": \"Method Not Allowed\",\n"
                << "    \"details\": {\n"
                << "        \"method_used\": \"POST\",\n"
                << "        \"allowed_methods\": [\"GET\"],\n"
                << "        \"resolution\": \"Use the correct HTTP method. Refer to the API documentation for supported methods.\",\n"
                << "        \"documentation\": \"https://example.com/docs#http-methods\"\n"
                << "        \"example\": \"GET /path?start=5&end=6\"\n"
                << "    }\n"
                << "}\n";
            cout << "Method not allowed" << endl;
            break;
        }
        default: {
            ss << "HTTP/1.1 500 Internal Server Error\n"
               << "Content-Type: application/json\n\n"
                << "{\n"
                << "    \"status\": \"An unknown error occured.\",\n"
                << "    \"details\": {\n"
                << "        \"error\": \"Internal Server Error\",\n"
                << "        \"resolution\": \"Check server logs for detailed error information.\",\n"
                << "        \"documentation\": \"https://example.com/docs#errors\"\n"
                << "    }\n"
                << "}\n";

            cout << "An unknown error occured" << endl;
        }
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

    // identify the start and end parameters in the request
    size_t source_start_pos = request.find("?start=");
    size_t source_end_pos = request.find("&end=");
    size_t destination_start_pos = request.find("end=");
    size_t destination_end_pos = request.find("HTTP/1.1");

    // check if the request is well GET request and not another
    if (request.find("GET") == string::npos) {
        send_error(client_socket, 405);
        return;
    }

    // extract the parameters from the request
    string start_param = request.substr(source_start_pos + 7, source_end_pos - source_start_pos - 7);
    string end_param = request.substr(destination_start_pos + 4, destination_end_pos - destination_start_pos - 5);

    // removes all instances of "%20" in the strings (URL encoding for space)
    size_t temp_pos;
    while ((temp_pos = start_param.find("%20")) != string::npos) {
        start_param.erase(temp_pos, 3);
    }
    while ((temp_pos = end_param.find("%20")) != string::npos) {
        end_param.erase(temp_pos, 3);
    }

    // display the parameters in the console
    cout << "Start: " << start_param << " End: " << end_param << endl;

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
        
    find_path(g_graph, g_path, g_astar, g_timer);

    send_path(g_path, client_socket);

    reset_compute_data(g_graph, g_path, g_astar);

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

    cout << "API server running on port 8080" << endl;
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