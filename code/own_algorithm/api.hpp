#ifndef API_HPP
#define API_HPP

#include "header.hpp"

void send_path(Path& g_path, int client_socket) {
    stringstream ss;

    if (response_format == "xml") {
        // XML Response
        ss << "HTTP/1.1 200 OK\nContent-Type: application/xml\n\n";
        ss << "<response>\n";
        ss << "  <time>" << g_path.distance << "</time>\n";
        ss << "  <path>\n";
        for (const auto& landmark : g_path.path) {
            ss << "    <landmark>" << landmark.first << "</landmark>\n";
        }
        ss << "  </path>\n";
        ss << "</response>\n";
    } else {
        // JSON Response (Default)
        ss << "HTTP/1.1 200 OK\nContent-Type: application/json\n\n";
        ss << "{\n";
        ss << "    \"path\": [";
        for (size_t i = 0; i < g_path.path.size(); i++) {
            ss << "\"" << g_path.path[i].first << "\"";
            if (i < g_path.path.size() - 1) {
                ss << ", ";
            }
        }
        ss << "],\n";
        ss << "    \"travel_time\": " << g_path.distance << "\n";
        ss << "}\n";
    }

    string response = ss.str();
    send(client_socket, response.c_str(), response.size(), 0);
}

void send_error(int client_socket, int error_code, int kind = 1, string node = "undefined") {
    stringstream ss;

    if (response_format == "xml") {
        ss << "HTTP/1.1 " << error_code << " ";
        switch (error_code) {
            case 400:
                ss << "Bad Request\nContent-Type: application/xml\n\n";
                ss << "<status>\n";
                ss << "  <message>Bad request</message>\n";
                ss << "  <details>\n";
                if (kind == 1) {
                    ss << "    <error_type>Missing parameters</error_type>\n";
                    ss << "    <missing_parameters>\n";
                    ss << "      <parameter>start</parameter>\n";
                    ss << "      <parameter>end</parameter>\n";
                    ss << "    </missing_parameters>\n";
                } else if (kind == 2) {
                    ss << "    <error_type>Invalid parameter format</error_type>\n";
                    ss << "    <invalid_parameter>" << node << "</invalid_parameter>\n";
                    ss << "    <resolution>This parameter requires a positive 32-bit INTEGER.</resolution>\n";
                }
                ss << "    <documentation>https://example.com/docs#parameters</documentation>\n";
                ss << "    <example>GET /path?start=5&end=6</example>\n";
                ss << "  </details>\n";
                ss << "</status>\n";
                break;

            case 404:
                ss << "Not Found\nContent-Type: application/xml\n\n";
                ss << "<status>\n";
                ss << "  <message>Node not found</message>\n";
                ss << "  <details>\n";
                if (kind == 1) {
                    ss << "    <error_type>Node does not exist</error_type>\n";
                    ss << "    <landmark_id>" << node << "</landmark_id>\n";
                    ss << "    <resolution>Check the dataset for valid node IDs.</resolution>\n";
                } else if (kind == 2) {
                    ss << "    <error_type>Node out of range</error_type>\n";
                    ss << "    <landmark_id>" << node << "</landmark_id>\n";
                    ss << "    <resolution>Nodes of this dataset are between 1 and " << g_graph.nodes_qty << ".</resolution>\n";
                }
                ss << "    <documentation>https://example.com/docs#landmarks</documentation>\n";
                ss << "  </details>\n";
                ss << "</status>\n";
                break;

            case 405:
                ss << "Method Not Allowed\nContent-Type: application/xml\n\n";
                ss << "<status>\n";
                ss << "  <message>Method Not Allowed</message>\n";
                ss << "  <details>\n";
                ss << "    <method_used>POST</method_used>\n";
                ss << "    <allowed_methods>\n";
                ss << "      <method>GET</method>\n";
                ss << "    </allowed_methods>\n";
                ss << "    <resolution>Use the correct HTTP method. Refer to the API documentation.</resolution>\n";
                ss << "    <documentation>https://example.com/docs#http-methods</documentation>\n";
                ss << "    <example>GET /path?start=5&end=6</example>\n";
                ss << "  </details>\n";
                ss << "</status>\n";
                break;

            default: // 500 Internal Server Error
                ss << "Internal Server Error\nContent-Type: application/xml\n\n";
                ss << "<status>\n";
                ss << "  <message>An unknown error occurred</message>\n";
                ss << "  <details>\n";
                ss << "    <error>Internal Server Error</error>\n";
                ss << "    <resolution>Check server logs for detailed error information.</resolution>\n";
                ss << "    <documentation>https://example.com/docs#errors</documentation>\n";
                ss << "  </details>\n";
                ss << "</status>\n";
                break;
        }
    } else { // Default JSON Response
        ss << "HTTP/1.1 " << error_code << " ";
        switch (error_code) {
            case 400:
                ss << "Bad Request\nContent-Type: application/json\n\n";
                ss << "{\n";
                ss << "    \"status\": \"Bad request\",\n";
                ss << "    \"details\": {\n";
                if (kind == 1) {
                    ss << "        \"error_type\": \"Missing parameters\",\n";
                    ss << "        \"missing_parameters\": [\"start\", \"end\"],\n";
                } else if (kind == 2) {
                    ss << "        \"error_type\": \"Invalid parameter format\",\n";
                    ss << "        \"invalid_parameter\": \"" << node << "\",\n";
                    ss << "        \"resolution\": \"This parameter requires a positive 32-bit INTEGER.\",\n";
                }
                ss << "        \"documentation\": \"https://example.com/docs#parameters\",\n";
                ss << "        \"example\": \"GET /path?start=5&end=6\"\n";
                ss << "    }\n";
                ss << "}\n";
                break;

            case 404:
                ss << "Not Found\nContent-Type: application/json\n\n";
                ss << "{\n";
                ss << "    \"status\": \"Node not found\",\n";
                ss << "    \"details\": {\n";
                if (kind == 1) {
                    ss << "        \"error_type\": \"Node does not exist\",\n";
                    ss << "        \"landmark_id\": \"" << node << "\",\n";
                    ss << "        \"resolution\": \"Check the dataset for valid node IDs.\",\n";
                } else if (kind == 2) {
                    ss << "        \"error_type\": \"Node out of range\",\n";
                    ss << "        \"landmark_id\": \"" << node << "\",\n";
                    ss << "        \"resolution\": \"Nodes of this dataset are between 1 and " << g_graph.nodes_qty << "\",\n";
                }
                ss << "        \"documentation\": \"https://example.com/docs#landmarks\"\n";
                ss << "    }\n";
                ss << "}\n";
                break;

            case 405:
                ss << "Method Not Allowed\nContent-Type: application/json\n\n";
                ss << "{\n";
                ss << "    \"status\": \"Method Not Allowed\",\n";
                ss << "    \"details\": {\n";
                ss << "        \"method_used\": \"POST\",\n";
                ss << "        \"allowed_methods\": [\"GET\"],\n";
                ss << "        \"resolution\": \"Use the correct HTTP method. Refer to the API documentation.\",\n";
                ss << "        \"documentation\": \"https://example.com/docs#http-methods\",\n";
                ss << "        \"example\": \"GET /path?start=5&end=6\"\n";
                ss << "    }\n";
                ss << "}\n";
                break;

            default: // 500 Internal Server Error
                ss << "Internal Server Error\nContent-Type: application/json\n\n";
                ss << "{\n";
                ss << "    \"status\": \"An unknown error occurred\",\n";
                ss << "    \"details\": {\n";
                ss << "        \"error\": \"Internal Server Error\",\n";
                ss << "        \"resolution\": \"Check server logs for detailed error information.\",\n";
                ss << "        \"documentation\": \"https://example.com/docs#errors\"\n";
                ss << "    }\n";
                ss << "}\n";
                break;
        }
    }

    string response = ss.str();
    send(client_socket, response.c_str(), response.size(), 0);
    close(client_socket);
}

void handle_request(int client_socket) {
    cout << "\n\nNew client connected" << endl;

    char buffer[4096] = {0};
    read(client_socket, buffer, 4096);
    string request(buffer);

    cout << "\nRequest received: \n" << request << endl;

    // Ensure the request is a GET request
    if (request.find("GET") == string::npos) {
        send_error(client_socket, 405);
        return;
    }

    // Extract query parameters
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