#ifndef API_RESPONSES_HPP
#define API_RESPONSES_HPP

#include "header.hpp"

void close_socket(int client_socket) {
    #ifdef _WIN32
        closesocket(client_socket);
        WSACleanup();
    #else
        close(client_socket);
    #endif
}

// just for fun
void send_favicon(int client_socket) {
    ifstream favicon_file("favicon.ico", ios::binary | ios::ate);
    if (!favicon_file) {
        // Return 404 if file not found
        const char *favicon_response =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Length: 0\r\n"
            "Connection: close\r\n"
            "\r\n";
        send(client_socket, favicon_response, strlen(favicon_response), 0);
        close_socket(client_socket);
        return;
    }

    // Get file size
    streamsize size = favicon_file.tellg();
    favicon_file.seekg(0, ios::beg);

    // Read file into buffer
    vector<char> buffer(size);
    if (favicon_file.read(buffer.data(), size)) {
        string header = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: image/x-icon\r\n"
            "Content-Length: " + to_string(size) + "\r\n"
            "Connection: close\r\n"
            "\r\n";
        
        send(client_socket, header.c_str(), header.size(), 0);
        send(client_socket, buffer.data(), buffer.size(), 0);
    }

    close_socket(client_socket);
    return;
}

void send_path(Path& g_path, int client_socket) {
    stringstream ss;

    if (response_format == "xml") {
       // XML Response
        ss << "HTTP/1.1 200 OK\n"
           << "Content-Type: application/xml\n"
           << "Access-Control-Allow-Origin: *\n\n";
        ss << "<response>\n";
        ss << "  <path_length>" << g_path.distance << "</path_length>\n";
        ss << "  <path>\n";
        for (const auto& node : g_path.path) {
            ss << "    <node>" << node.first << "</node>\n";
        }
        ss << "  </path>\n";
        ss << "</response>\n";
    } else {
        // JSON Response (Default)
        ss << "HTTP/1.1 200 OK\n"
           << "Content-Type: application/json\n"
           << "Access-Control-Allow-Origin: *\n\n";
        ss << "{\n";
        ss << "    \"path_length\": " << "\"" << g_path.distance << "\",\n";
        ss << "    \"path\": [";
        for (size_t i = 0; i < g_path.path.size(); i++) {
            ss << "\"" << g_path.path[i].first << "\"" << endl;
            if (i < g_path.path.size() - 1) {
                ss << ", ";
            }
        }
        ss << "]\n";
        ss << "}\n";
    }

    string response = ss.str();
    send(client_socket, response.c_str(), response.size(), 0);
    close_socket(client_socket);
}

// Send full path with all the details such as distance for each node, number of nodes, execution time in addition to the path itself
void send_full_path(Path& g_path, int client_socket) {
    stringstream ss;

    if (response_format == "xml") {
        // XML Response
        ss << "HTTP/1.1 200 OK\n"
           << "Content-Type: application/xml\n"
           << "Access-Control-Allow-Origin: *\n\n";
        ss << "<response>\n";
        ss << "  <path_length>" << g_path.distance << "</path_length>\n";
        ss << "  <nodes_quantity>" << g_path.path.size() << "</nodes_quantity>\n";
        ss << "  <calculation_time>" << g_path.calculation_time << "</calculation_time>\n";
        ss << "  <path>\n";
        for (const auto& node : g_path.path) {
            ss << "    <node>\n";
            ss << "      <id>" << node.first << "</id>\n";
            ss << "      <distance>" << node.second << "</distance>\n";
            ss << "    </node>\n";
        }
        ss << "  </path>\n";
        ss << "</response>\n";
    } else {
        // JSON Response (Default)
        ss << "HTTP/1.1 200 OK\n"
           << "Content-Type: application/json\n"
           << "Access-Control-Allow-Origin: *\n\n";
        ss << "{\n";
        ss << "    \"path_length\"     : \"" << g_path.distance         << "\",\n";
        ss << "    \"nodes_quantity\"  : \"" << g_path.path.size()      << "\",\n";
        ss << "    \"calculation_time\": \"" << g_path.calculation_time << "\",\n";
        ss << "    \"path\": [\n";
        for (size_t i = 0; i < g_path.path.size(); i++) {
            ss << "        {\n";
            ss << "            \"id\": \"" << g_path.path[i].first << "\",\n";
            ss << "            \"distance\": \"" << g_path.path[i].second << "\"\n";
            ss << "        }";
            if (i < g_path.path.size() - 1) {
                ss << ",";
            }
            ss << endl;
        }
        ss << "    ]\n";
        ss << "}\n";
    }

    string response = ss.str();
    send(client_socket, response.c_str(), response.size(), 0);
    close_socket(client_socket);
}

void send_endpoint_error(int client_socket) {
    stringstream ss;

    if (response_format == "xml") {
        ss << "HTTP/1.1 400 Bad Request\nContent-Type: application/xml\n\n";
        ss << "<status>\n";
        ss << "  <message>Bad request</message>\n";
        ss << "  <details>\n";
        ss << "    <error_type>Invalid endpoint</error_type>\n";
        ss << "    <resolution>Check the API documentation for valid endpoints.</resolution>\n";
        ss << "    <documentation>https://example.com/docs#endpoints</documentation>\n";
        ss << "    <example>GET /path?start=1&end=4</example>\n";
        ss << "    <example>GET /command?command=rebuild_graph</example>\n";
        ss << "  </details>\n";
        ss << "</status>\n";
    } else {
        ss << "HTTP/1.1 400 Bad Request\nContent-Type: application/json\n\n";
        ss << "{\n";
        ss << "    \"status\": \"Bad request\",\n";
        ss << "    \"details\": {\n";
        ss << "        \"error_type\": \"Invalid endpoint\",\n";
        ss << "        \"resolution\": \"Check the API documentation for valid endpoints.\",\n";
        ss << "        \"documentation\": \"https://example.com/docs#endpoints\",\n";
        ss << "        \"example\": \"GET /path?start=1&end=4\"\n";
        ss << "        \"example\": \"GET /command?command=rebuild_graph\"\n";
        ss << "    }\n";
        ss << "}\n";
    }

    string response = ss.str();
    send(client_socket, response.c_str(), response.size(), 0);
    close_socket(client_socket);
}

void send_wrong_format(int client_socket) {

    stringstream ss;

    if (response_format == "xml") {
        ss << "HTTP/1.1 400 Bad Request\nContent-Type: application/xml\n\n";
        ss << "<status>\n";
        ss << "  <message>Bad request</message>\n";
        ss << "  <details>\n";
        ss << "    <error_type>Invalid response format</error_type>\n";
        ss << "    <resolution>Only 'json' and 'xml' are accepted formats. If any format specified, json is selected by default</resolution>\n";
        ss << "    <documentation>https://example.com/docs#response-format</documentation>\n";
        ss << "    <example>GET /path?start=1&end=4</example>\n";
        ss << "    <example>GET /path?start=1&end=4&format=xml</example>\n";
        ss << "  </details>\n";
        ss << "</status>\n";
    } else {
        ss << "HTTP/1.1 400 Bad Request\nContent-Type: application/json\n\n";
        ss << "{\n";
        ss << "    \"status\": \"Bad request\",\n";
        ss << "    \"details\": {\n";
        ss << "        \"error_type\": \"Invalid response format\",\n";
        ss << "        \"resolution\": \"Only 'json' and 'xml' are accepted formats. If any format specified, json is selected by default\",\n";
        ss << "        \"documentation\": \"https://example.com/docs#response-format\",\n";
        ss << "        \"example\": \"GET /path?start=1&end=4\",\n";
        ss << "        \"example\": \"GET /path?start=1&end=4&format=xml\"\n";
        ss << "    }\n";
        ss << "}\n";
    }

    string response = ss.str();
    send(client_socket, response.c_str(), response.size(), 0);
    close_socket(client_socket);
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
    close_socket(client_socket);
}

void send_cmd_error (int client_socket, int error_code, int kind = 1, string node = "undefined") {
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
                    ss << "      <parameter>command</parameter>\n";
                    ss << "    </missing_parameters>\n";
                } else if (kind == 2) {
                    ss << "    <error_type>Invalid parameter format</error_type>\n";
                    ss << "    <invalid_parameter>" << node << "</invalid_parameter>\n";
                    ss << "    <resolution>This parameter requires a valid command.</resolution>\n";
                }
                ss << "    <documentation>https://example.com/docs#commands</documentation>\n";
                ss << "    <example>GET /command?command=quickest-path&start=5&end=6</example>\n";
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
                ss << "    <example>GET /command?command=rebuild_graph</example>\n";
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
                    ss << "        \"missing_parameters\": [\"command\"],\n";
                } else if (kind == 2) {
                    ss << "        \"error_type\": \"Invalid parameter format\",\n";
                    ss << "        \"invalid_parameter\": \"" << node << "\",\n";
                    ss << "        \"resolution\": \"This parameter requires a valid command.\",\n";
                }
                ss << "        \"documentation\": \"https://example.com/docs#commands\",\n";
                ss << "        \"example\": \"GET /command?command=rebuild_graph\"\n";
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
                ss << "        \"example\": \"GET /command?command=rebuild_graph\"\n";
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
    close_socket(client_socket);
}

#endif // API_RESPONSES_HPP