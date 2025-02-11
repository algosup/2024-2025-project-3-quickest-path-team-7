#ifndef API_RESPONSES_HPP
#define API_RESPONSES_HPP

#include "header.hpp"

void closeSocket(int client_socket) {
    #ifdef _WIN32
        closesocket(client_socket);
    #else
        close(client_socket);
    #endif
}

void sendPath(Path& GlobalPath, int client_socket) {
    stringstream ss;

    if (response_format == "xml") {
       // XML Response
        ss << "HTTP/1.1 200 OK\n"
           << "Content-Type: application/xml\n"
           << "Access-Control-Allow-Origin: *\n\n";
        ss << "<response>\n";
        ss << "  <path_length>" << GlobalPath.distance << "</path_length>\n";
        ss << "  <path>\n";
        for (const auto& node : GlobalPath.path) {
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
        ss << "    \"path_length\": " << "\"" << GlobalPath.distance << "\",\n";
        ss << "    \"path\": [";
        for (size_t i = 0; i < GlobalPath.path.size(); i++) {
            ss << "\"" << GlobalPath.path[i].first << "\"" << endl;
            if (i < GlobalPath.path.size() - 1) {
                ss << ", ";
            }
        }
        ss << "]\n";
        ss << "}\n";
    }

    string response = ss.str();
    send(client_socket, response.c_str(), response.size(), 0);
    closeSocket(client_socket);
    display_valid_responses ? println("\n\nVALID response :\n" + response, type::VALIDATION) : print("");
}

// Send full path with all the details such as distance for each node, number of nodes, execution time , and so on
void sendFullPath(Path& GlobalPath, int client_socket) {
    stringstream ss;

    if (response_format == "xml") {
        // XML Response
        ss << "HTTP/1.1 200 OK\n"
           << "Content-Type: application/xml\n"
           << "Access-Control-Allow-Origin: *\n\n";
        ss << "<response>\n";
        ss << "  <dataset>" << GlobalFiles.dataset.base << "</dataset>\n";
        ss << "  <start>" << GlobalPath.start << "</start>\n";
        ss << "  <end>" << GlobalPath.end << "</end>\n";
        ss << "  <path_length>" << GlobalPath.distance << "</path_length>\n";
        ss << "  <time_unit>" << TIME_UNIT_FULL_STR << "</time_unit>\n";
        ss << "  <calculation_time>" << GlobalPath.calculation_time << "</calculation_time>\n";
        ss << "  <heuristic_weight>" << heuristic_weight << "</heuristic_weight>\n";
        ss << "  <landmarks_quantity>" << landmarks_qty << "</landmarks_quantity>\n";
        ss << "  <landmarks>\n";
        for (const auto& lm : GlobalGraph.landmarks) {
            ss << "    <landmark>" << lm << "</landmark>\n";
        }
        ss << "  </landmarks>\n";
        ss << "  <nodes_quantity>" << GlobalPath.path.size() << "</nodes_quantity>\n";
        ss << "  <path>\n";
        for (const auto& node : GlobalPath.path) {
            ss << "    <node>\n";
            ss << "      <id>" << node.first << "</id>\n";
            ss << "      <weight>" << node.second << "</weight>\n";
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
        ss << "    \"dataset\"            : \"" << GlobalFiles.dataset.base << "\",\n";
        ss << "    \"start\"              : \"" << GlobalPath.start << "\",\n";
        ss << "    \"end\"                : \"" << GlobalPath.end << "\",\n";
        ss << "    \"path_length\"        : \"" << GlobalPath.distance         << "\",\n";
        ss << "    \"time_unit\"          : \"" << TIME_UNIT_FULL_STR     << "\",\n";
        ss << "    \"calculation_time\"   : \"" << GlobalPath.calculation_time << "\",\n";
        ss << "    \"heuristic_weight\"   : \"" << heuristic_weight        << "\",\n";
        ss << "    \"landmarks_quantity\" : \"" << landmarks_qty << "\",\n";
        ss << "    \"landmarks\": [\n";
        for (size_t i = 0; i < GlobalGraph.landmarks.size(); i++) {
            ss << "        \"" << GlobalGraph.landmarks[i] << "\"";
            if (i < GlobalGraph.landmarks.size() - 1) {
                ss << ",";
            }
            ss << endl;
        }
        ss << "    ],\n";
        ss << "    \"nodes_quantity\"    : \"" << GlobalPath.path.size()      << "\",\n";
        ss << "    \"path\": [\n";
        for (size_t i = 0; i < GlobalPath.path.size(); i++) {
            ss << "        {\n";
            ss << "            \"id\"     : \"" << GlobalPath.path[i].first << "\",\n";
            ss << "            \"weight\" : \"" << GlobalPath.path[i].second << "\"\n";
            ss << "        }";
            if (i < GlobalPath.path.size() - 1) {
                ss << ",";
            }
            ss << endl;
        }
        ss << "    ]\n";
        ss << "}\n";
    }

    string response = ss.str();
    send(client_socket, response.c_str(), response.size(), 0);
    closeSocket(client_socket);
    display_valid_responses ? println("\n\nVALID response :\n" + response, type::VALIDATION) : print("");

}

void sendComparedPath(Path& GlobalPath, Path& DijkstraPath, int client_socket) {
    stringstream ss;

    if (response_format == "xml") {
        // XML Response
        ss << "HTTP/1.1 200 OK\n"
           << "Content-Type: application/xml\n"
           << "Access-Control-Allow-Origin: *\n\n";
        ss << "<response>\n";
        ss << "  <dataset>" << GlobalFiles.dataset.base << "</dataset>\n";
        ss << "  <start>" << GlobalPath.start << "</start>\n";
        ss << "  <end>" << GlobalPath.end << "</end>\n";
        ss << "  <oversize_percentage>" << comparisonPercentage(GlobalPath.distance, DijkstraPath.distance) << "</oversize_percentage>\n";
        ss << "  <dijkstra_path_length>" << DijkstraPath.distance << "</dijkstra_path_length>\n";
        ss << "  <astar_path_length>" << GlobalPath.distance << "</astar_path_length>\n";
        ss << "  <time_unit>" << TIME_UNIT_FULL_STR << "</time_unit>\n";
        ss << "  <astar_calculation_time>" << GlobalPath.calculation_time << "</astar_calculation_time>\n";
        ss << "  <dijkstra_calculation_time>" << DijkstraPath.calculation_time << "</dijkstra_calculation_time>\n";
        ss << "  <astar_nodes_quantity>" << GlobalPath.path.size() << "</astar_nodes_quantity>\n";
        ss << "  <dijkstra_nodes_quantity>" << DijkstraPath.path.size() << "</dijkstra_nodes_quantity>\n";
        ss << "  <heuristic_weight>" << heuristic_weight << "</heuristic_weight>\n";
        ss << "  <landmarks_quantity>" << landmarks_qty << "</landmarks_quantity>\n";
        ss << "  <compared_path>\n";
        for (size_t i = 0; i < GlobalPath.path.size(); i++) {
            ss << "    <node>\n";
            ss << "      <astar>\n";
            ss << "        <id>" << DijkstraPath.path[i].first << "</id>\n";
            ss << "        <weight>" << DijkstraPath.path[i].second << "</weight>\n";
            ss << "      </astar>\n";
            ss << "      <dijkstra>\n";
            ss << "        <id>" << GlobalPath.path[i].first << "</id>\n";
            ss << "        <weight>" << GlobalPath.path[i].second << "</weight>\n";
            ss << "      </dijkstra>\n";
            ss << "    </node>\n";
        }
        ss << "  </compared_path>\n";
        ss << "</response>\n";
    } else {
        // JSON Response (Default)
        ss << "HTTP/1.1 200 OK\n"
           << "Content-Type: application/json\n"
           << "Access-Control-Allow-Origin: *\n\n";
        ss << "{\n";
        ss << "    \"dataset\"              : \"" << GlobalFiles.dataset.base << "\",\n";
        ss << "    \"start\"                : \"" << GlobalPath.start << "\",\n";
        ss << "    \"end\"                  : \"" << GlobalPath.end << "\",\n";
        ss << "    \"oversize_percentage\"  : \"" << comparisonPercentage(GlobalPath.distance, DijkstraPath.distance) << "\",\n";
        ss << "    \"dijkstra_path_length\" : \"" << DijkstraPath.distance << "\",\n";
        ss << "    \"astar_path_length\"    : \"" << GlobalPath.distance << "\",\n";
        ss << "    \"time_unit\"            : \"" << TIME_UNIT_FULL_STR << "\",\n";
        ss << "    \"astar_calculation_time\": \"" << GlobalPath.calculation_time << "\",\n";
        ss << "    \"dijkstra_calculation_time\": \"" << DijkstraPath.calculation_time << "\",\n";
        ss << "    \"astar_nodes_quantity\" : \"" << GlobalPath.path.size() << "\",\n";
        ss << "    \"dijkstra_nodes_quantity\" : \"" << DijkstraPath.path.size() << "\",\n";
        ss << "    \"heuristic_weight\"     : \"" << heuristic_weight << "\",\n";
        ss << "    \"landmarks_quantity\"   : \"" << landmarks_qty << "\",\n";
        ss << "    \"compared_path\": [\n";
        for (size_t i = 0; i < GlobalPath.path.size(); i++) {
            ss << "        {\n";
            ss << "            \"astar\": {\n";
            ss << "                \"id\": \"" << DijkstraPath.path[i].first << "\",\n";
            ss << "                \"weight\": \"" << DijkstraPath.path[i].second << "\"\n";
            ss << "            },\n";
            ss << "            \"dijkstra\": {\n";
            ss << "                \"id\": \"" << GlobalPath.path[i].first << "\",\n";
            ss << "                \"weight\": \"" << GlobalPath.path[i].second << "\"\n";
            ss << "            }\n";
            ss << "        }";
            if (i < GlobalPath.path.size() - 1) {
                ss << ",";
            }
            ss << endl;
        }
        ss << "    ]\n";
        ss << "}\n";
    }  

    string response = ss.str();
    send(client_socket, response.c_str(), response.size(), 0);
    closeSocket(client_socket);
    display_valid_responses ? println("\n\nVALID response :\n" + response, type::VALIDATION) : print("");
}

void sendEndpointError(int client_socket) {
    stringstream ss;

    if (response_format == "xml") {
        ss << "HTTP/1.1 400 Bad Request\n";
        ss << "Content-Type: application/xml\n";
        ss << "Access-Control-Allow-Origin: *\n\n";
        ss << "<status>\n";
        ss << "  <message>Bad request</message>\n";
        ss << "  <details>\n";
        ss << "    <error_type>Invalid endpoint</error_type>\n";
        ss << "    <resolution>Check the API documentation for valid endpoints.</resolution>\n";
        ss << "    <documentation>https://github.com/algosup/2024-2025-project-3-quickest-path-team-7/blob/main/Document/Functional/FunctionalSpecifications.md#command-endpoint</documentation>\n";
        ss << "    <example>GET /path?start=1&amp;end=4</example>\n";
        ss << "    <example>GET /debug_path?start=1&amp;end=4</example>\n";
        ss << "    <example>GET /comp_path?start=1&amp;end=4</example>\n";
        ss << "    <example>GET /command?command=rebuild_graph</example>\n";
        ss << "  </details>\n";
        ss << "</status>\n";
    } else {
        ss << "HTTP/1.1 400 Bad Request\n";
        ss << "Content-Type: application/json\n";
        ss << "Access-Control-Allow-Origin: *\n\n";
        ss << "{\n";
        ss << "    \"status\": \"Bad request\",\n";
        ss << "    \"details\": {\n";
        ss << "        \"error_type\": \"Invalid endpoint\",\n";
        ss << "        \"resolution\": \"Check the API documentation for valid endpoints.\",\n";
        ss << "        \"documentation\": \"https://github.com/algosup/2024-2025-project-3-quickest-path-team-7/blob/main/Document/Functional/FunctionalSpecifications.md#command-endpoint\",\n";
        ss << "        \"example\": \"GET /path?start=1&end=4\"\n";
        ss << "        \"example\": \"GET /debug_path?start=1&end=4\"\n";
        ss << "        \"example\": \"GET /comp_path?start=1&end=4\"\n";
        ss << "        \"example\": \"GET /command?command=rebuild_graph\"\n";
        ss << "    }\n";
        ss << "}\n";
    }

    string response = ss.str();
    send(client_socket, response.c_str(), response.size(), 0);
    closeSocket(client_socket);
    display_error_responses ? println("\n\nERROR response :\n" + response, type::ERROR_LIGHT) : print("");

}

void sendWrongFormat(int client_socket) {

    stringstream ss;

    if (response_format == "xml") {
        ss << "HTTP/1.1 400 Bad Request\n";
        ss << "Content-Type: application/xml\n";
        ss << "Access-Control-Allow-Origin: *\n\n";
        ss << "<status>\n";
        ss << "  <message>Bad request</message>\n";
        ss << "  <details>\n";
        ss << "    <error_type>Invalid response format</error_type>\n";
        ss << "    <resolution>Only 'json' and 'xml' are accepted formats. If any format specified, json is selected by default</resolution>\n";
        ss << "    <documentation>https://github.com/algosup/2024-2025-project-3-quickest-path-team-7/blob/main/Document/Functional/FunctionalSpecifications.md#api-details</documentation>\n";
        ss << "    <example>GET /" << endpoint_adaptation << "path?start=1&amp;end=4</example>\n";
        ss << "    <example>GET /" << endpoint_adaptation << "path?start=1&amp;end=4&amp;format=xml</example>\n";
        ss << "  </details>\n";
        ss << "</status>\n";
    } else {
        ss << "HTTP/1.1 400 Bad Request\n";
        ss << "Content-Type: application/json\n";
        ss << "Access-Control-Allow-Origin: *\n\n";
        ss << "{\n";
        ss << "    \"status\": \"Bad request\",\n";
        ss << "    \"details\": {\n";
        ss << "        \"error_type\": \"Invalid response format\",\n";
        ss << "        \"resolution\": \"Only 'json' and 'xml' are accepted formats. If any format specified, json is selected by default\",\n";
        ss << "        \"documentation\": \"https://github.com/algosup/2024-2025-project-3-quickest-path-team-7/blob/main/Document/Functional/FunctionalSpecifications.md#api-details\",\n";
        ss << "        \"example\": \"GET /" << endpoint_adaptation << "path?start=1&end=4\",\n";
        ss << "        \"example\": \"GET /" << endpoint_adaptation << "path?start=1&end=4&format=xml\"\n";
        ss << "    }\n";
        ss << "}\n";
    }

    string response = ss.str();
    send(client_socket, response.c_str(), response.size(), 0);
    closeSocket(client_socket);
    display_error_responses ? println("\n\nERROR response :\n" + response, type::ERROR_LIGHT) : print("");

}

void sendError(int client_socket, int error_code, int kind = 1, string parameter= "undefined") {
    stringstream ss;

    if (response_format == "xml") {
        ss << "HTTP/1.1 " << error_code << " ";
        switch (error_code) {
            case 400:
                if (kind == SAME_NODE) {
                    ss << "Bad Request\n";
                    ss << "Content-Type: application/xml\n";
                    ss << "Access-Control-Allow-Origin: *\n\n";
                    ss << "<status>\n";
                    ss << "  <message>Bad request</message>\n";
                    ss << "  <details>\n";
                    ss << "    <error_type>The \"start\" and \"end\" nodes are equal</error_type>\n";
                    ss << "    <parameter_value>" << parameter << "</parameter_value>\n";
                    ss << "    <resolution>Start and end nodes must be different.</resolution>\n";
                    ss << "    <documentation>https://github.com/algosup/2024-2025-project-3-quickest-path-team-7/blob/main/Document/Functional/FunctionalSpecifications.md#api-details</documentation>\n";
                    ss << "  </details>\n";
                    ss << "</status>\n";
                    break;
                }
                ss << "Bad Request\n";
                ss << "Content-Type: application/xml\n";
                ss << "Access-Control-Allow-Origin: *\n\n";
                ss << "<status>\n";
                ss << "  <message>Bad request</message>\n";
                ss << "  <details>\n";
                if (kind == MISSING_PARAMETER) {
                    ss << "    <error_type>Missing parameters</error_type>\n";
                    ss << "    <missing_parameters>\n";
                    ss << "      <parameter>start</parameter>\n";
                    ss << "      <parameter>end</parameter>\n";
                    ss << "    </missing_parameters>\n";
                } else if (kind == INVALID_PARAMETER) {
                    ss << "    <error_type>Invalid parameter format</error_type>\n";
                    ss << "    <invalid_parameter>" << parameter << "</invalid_parameter>\n";
                    ss << "    <resolution>This parameter requires a positive 32-bit INTEGER.</resolution>\n";
                }
                ss << "    <documentation>https://github.com/algosup/2024-2025-project-3-quickest-path-team-7/blob/main/Document/Functional/FunctionalSpecifications.md#api-details</documentation>\n";
                ss << "    <example>GET /" << endpoint_adaptation << "path?start=5&amp;end=6</example>\n";
                ss << "  </details>\n";
                ss << "</status>\n";
                break;

            case 404:
                ss << "Not Found\n";
                ss << "Content-Type: application/xml\n";
                ss << "Access-Control-Allow-Origin: *\n\n";
                ss << "<status>\n";
                ss << "  <message>Node not found</message>\n";
                ss << "  <details>\n";
                if (kind == NODE_NOT_FOUND) {
                    ss << "    <error_type>Node does not exist</error_type>\n";
                    ss << "    <landmark_id>" << parameter << "</landmark_id>\n";
                    ss << "    <resolution>Check the dataset for valid node IDs.</resolution>\n";
                } else if (kind == NODE_OUT_OF_RANGE) {
                    ss << "    <error_type>Node out of range</error_type>\n";
                    ss << "    <landmark_id>" << parameter << "</landmark_id>\n";
                    ss << "    <resolution>Nodes of this dataset are between 1 and " << GlobalGraph.nodes_qty << ".</resolution>\n";
                }
                ss << "  </details>\n";
                ss << "</status>\n";
                break;

            case 405:
                ss << "Method Not Allowed\n";
                ss << "Content-Type: application/xml\n";
                ss << "Access-Control-Allow-Origin: *\n\n";
                ss << "<status>\n";
                ss << "  <message>Method Not Allowed</message>\n";
                ss << "  <details>\n";
                ss << "    <method_used>" << parameter << "</method_used>\n";
                ss << "    <allowed_methods>\n";
                ss << "      <method>GET</method>\n";
                ss << "    </allowed_methods>\n";
                ss << "    <resolution>Use GET method only. You can refer to the API documentation.</resolution>\n";
                ss << "    <documentation>https://github.com/algosup/2024-2025-project-3-quickest-path-team-7/blob/main/Document/Functional/FunctionalSpecifications.md#api-details</documentation>\n";
                ss << "    <example>GET /" << endpoint_adaptation << "path?start=5&amp;end=6</example>\n";
                ss << "  </details>\n";
                ss << "</status>\n";
                break;

            default: // 500 Internal Server Error
                ss << "Internal Server Error\n";
                ss << "Content-Type: application/xml\n";
                ss << "Access-Control-Allow-Origin: *\n\n";
                ss << "  <message>An unknown error occurred</message>\n";
                ss << "  <details>\n";
                ss << "    <error>Internal Server Error</error>\n";
                ss << "    <resolution>Check server logs for detailed error information.</resolution>\n";
                ss << "    <documentation>https://github.com/algosup/2024-2025-project-3-quickest-path-team-7/blob/main/Document/Functional/FunctionalSpecifications.md</documentation>\n";
                ss << "  </details>\n";
                ss << "</status>\n";
                break;
        }
    } else { // Default JSON Response
        ss << "HTTP/1.1 " << error_code << " ";
        switch (error_code) {
            case 400:
                if (kind == SAME_NODE) {
                    ss << "Bad Request\n";
                    ss << "Content-Type: application/json\n";
                    ss << "Access-Control-Allow-Origin: *\n\n";
                    ss << "{\n";
                    ss << "    \"status\": \"Bad request\",\n";
                    ss << "    \"details\": {\n";
                    ss << "        \"error_type\": \"The 'start' and 'end' nodes are equal\",\n";
                    ss << "        \"parameter_value\": \"" << parameter << "\",\n";
                    ss << "        \"resolution\": \"Start and end nodes must be different.\",\n";
                    ss << "        \"documentation\": \"https://github.com/algosup/2024-2025-project-3-quickest-path-team-7/blob/main/Document/Functional/FunctionalSpecifications.md#api-details\"\n";
                    ss << "    }\n";
                    ss << "}\n";
                    break;
                }
                ss << "Bad Request\n";
                ss << "Content-Type: application/json\n";
                ss << "Access-Control-Allow-Origin: *\n\n";
                ss << "{\n";
                ss << "    \"status\": \"Bad request\",\n";
                ss << "    \"details\": {\n";
                if (kind == MISSING_PARAMETER) {
                    ss << "        \"error_type\": \"Missing parameters\",\n";
                    ss << "        \"missing_parameters\": [\"start\", \"end\"],\n";
                } else if (kind == INVALID_PARAMETER) {
                    ss << "        \"error_type\": \"Invalid parameter format\",\n";
                    ss << "        \"invalid_parameter\": \"" << parameter << "\",\n";
                    ss << "        \"resolution\": \"This parameter requires a positive 32-bit INTEGER.\",\n";
                }
                ss << "        \"documentation\": \"https://github.com/algosup/2024-2025-project-3-quickest-path-team-7/blob/main/Document/Functional/FunctionalSpecifications.md#api-details\",\n";
                ss << "        \"example\": \"GET /" << endpoint_adaptation << "path?start=5&end=6\"\n";
                ss << "    }\n";
                ss << "}\n";
                break;

            case 404:
                ss << "Not Found\n";
                ss << "Content-Type: application/json\n";
                ss << "Access-Control-Allow-Origin: *\n\n";
                ss << "{\n";
                ss << "    \"status\": \"Node not found\",\n";
                ss << "    \"details\": {\n";
                if (kind == NODE_NOT_FOUND) {
                    ss << "        \"error_type\": \"Node does not exist\",\n";
                    ss << "        \"landmark_id\": \"" << parameter << "\",\n";
                    ss << "        \"resolution\": \"Check the dataset for valid node IDs.\",\n";
                } else if (kind == NODE_OUT_OF_RANGE) {
                    ss << "        \"error_type\": \"Node out of range\",\n";
                    ss << "        \"landmark_id\": \"" << parameter << "\",\n";
                    ss << "        \"resolution\": \"Nodes of this dataset are between 1 and " << GlobalGraph.nodes_qty << "\",\n";
                }
                ss << "    }\n";
                ss << "}\n";
                break;

            case 405:
                ss << "Method Not Allowed\n";
                ss << "Content-Type: application/json\n";
                ss << "Access-Control-Allow-Origin: *\n\n";
                ss << "{\n";
                ss << "    \"status\": \"Method Not Allowed\",\n";
                ss << "    \"details\": {\n";
                ss << "        \"method_used\": \"" << parameter << "\",\n";
                ss << "        \"allowed_methods\": [\"GET\"],\n";
                ss << "        \"resolution\": \"Use GET method only. You can refer to the API documentation.\",\n";
                ss << "        \"documentation\": \"https://github.com/algosup/2024-2025-project-3-quickest-path-team-7/blob/main/Document/Functional/FunctionalSpecifications.md#api-details\",\n";
                ss << "        \"example\": \"GET /" << endpoint_adaptation << "path?start=5&end=6\"\n";
                ss << "    }\n";
                ss << "}\n";
                break;
            
            default: // 500 Internal Server Error
                ss << "Internal Server Error\n";
                ss << "Content-Type: application/json\n";
                ss << "Access-Control-Allow-Origin: *\n\n";
                ss << "{\n";
                ss << "    \"status\": \"An unknown error occurred\",\n";
                ss << "    \"details\": {\n";
                ss << "        \"error\": \"Internal Server Error\",\n";
                ss << "        \"resolution\": \"Check server logs for detailed error information.\",\n";
                ss << "        \"documentation\": \"https://github.com/algosup/2024-2025-project-3-quickest-path-team-7/blob/main/Document/Functional/FunctionalSpecifications.md\"\n";
                ss << "    }\n";
                ss << "}\n";
                break;
        }
    }

    string response = ss.str();
    send(client_socket, response.c_str(), response.size(), 0);
    closeSocket(client_socket);
    display_error_responses ? println("\n\nERROR response :\n" + response, type::ERROR_LIGHT) : print("");
}

void sendCmdError(int client_socket, int error_code, int kind = 1, string parameter = "undefined") {
    stringstream ss;

    if (response_format == "xml") {
        ss << "HTTP/1.1 " << error_code << " ";
        switch (error_code) {
            case 400:
                ss << "Bad Request\n";
                ss << "Content-Type: application/xml\n";
                ss << "Access-Control-Allow-Origin: *\n\n";
                ss << "<status>\n";
                ss << "  <message>Bad request</message>\n";
                ss << "  <details>\n";
                if (kind == MISSING_PARAMETER) {
                    ss << "    <error_type>Missing parameters</error_type>\n";
                    ss << "    <missing_parameters>\n";
                    ss << "      <parameter>command</parameter>\n";
                    ss << "    </missing_parameters>\n";
                } else if (kind == INVALID_PARAMETER) {
                    ss << "    <error_type>Invalid parameter format</error_type>\n";
                    ss << "    <invalid_parameter>" << parameter << "</invalid_parameter>\n";
                    ss << "    <resolution>This parameter requires a valid command.</resolution>\n";
                }
                ss << "    <documentation>https://github.com/algosup/2024-2025-project-3-quickest-path-team-7/blob/main/Document/Functional/FunctionalSpecifications.md</documentation>\n";
                ss << "    <example>GET /command?command=quickest-path&start=5&amp;end=6</example>\n";
                ss << "  </details>\n";
                ss << "</status>\n";
                break;

            case 405:
                ss << "Method Not Allowed\n";
                ss << "Content-Type: application/xml\n";
                ss << "Access-Control-Allow-Origin: *\n\n";
                ss << "<status>\n";
                ss << "  <message>Method Not Allowed</message>\n";
                ss << "  <details>\n";
                ss << "    <method_used>" << parameter << "</method_used>\n";
                ss << "    <allowed_methods>\n";
                ss << "      <method>GET</method>\n";
                ss << "    </allowed_methods>\n";
                ss << "    <resolution>Use GET method only. You can refer to the API documentation.</resolution>\n";
                ss << "    <documentation>https://github.com/algosup/2024-2025-project-3-quickest-path-team-7/blob/main/Document/Functional/FunctionalSpecifications.md#api-details</documentation>\n";
                ss << "    <example>GET /command?command=rebuild_graph</example>\n";
                ss << "  </details>\n";
                ss << "</status>\n";
                break;

            default: // 500 Internal Server Error
                ss << "Internal Server Error";
                ss << "Content-Type: application/xml\n";
                ss << "Access-Control-Allow-Origin: *\n\n";
                ss << "<status>\n";
                ss << "  <message>An unknown error occurred</message>\n";
                ss << "  <details>\n";
                ss << "    <error>Internal Server Error</error>\n";
                ss << "    <resolution>Check server logs for detailed error information.</resolution>\n";
                ss << "    <documentation>https://github.com/algosup/2024-2025-project-3-quickest-path-team-7/blob/main/Document/Functional/FunctionalSpecifications.md</documentation>\n";
                ss << "  </details>\n";
                ss << "</status>\n";
                break;
        }

    } else { // Default JSON Response

        ss << "HTTP/1.1 " << error_code << " ";
        switch (error_code) {
            case 400:
                ss << "Bad Request\n";
                ss << "Content-Type: application/json\n";
                ss << "Access-Control-Allow-Origin: *\n\n";
                ss << "{\n";
                ss << "    \"status\": \"Bad request\",\n";
                ss << "    \"details\": {\n";
                if (kind == MISSING_PARAMETER) {
                    ss << "        \"error_type\": \"Missing parameters\",\n";
                    ss << "        \"missing_parameters\": [\"command\"],\n";
                } else if (kind == INVALID_PARAMETER) {
                    ss << "        \"error_type\": \"Invalid parameter format\",\n";
                    ss << "        \"invalid_parameter\": \"" << parameter << "\",\n";
                    ss << "        \"resolution\": \"This parameter requires a valid command.\",\n";
                }
                ss << "        \"documentation\": \"https://github.com/algosup/2024-2025-project-3-quickest-path-team-7/blob/main/Document/Functional/FunctionalSpecifications.md\",\n";
                ss << "        \"example\": \"GET /command?command=rebuild_graph\"\n";
                ss << "    }\n";
                ss << "}\n";
                break;

            case 405:
                ss << "Method Not Allowed\n";
                ss << "Content-Type: application/json\n";
                ss << "Access-Control-Allow-Origin: *\n\n";
                ss << "{\n";
                ss << "    \"status\": \"Method Not Allowed\",\n";
                ss << "    \"details\": {\n";
                ss << "        \"method_used\": \"" << parameter << "\",\n";
                ss << "        \"allowed_methods\": [\"GET\"],\n";
                ss << "        \"resolution\": \"Use GET method only. You can refer to the API documentation.\",\n";
                ss << "        \"documentation\": \"https://github.com/algosup/2024-2025-project-3-quickest-path-team-7/blob/main/Document/Functional/FunctionalSpecifications.md#api-details\",\n";
                ss << "        \"example\": \"GET /command?command=rebuild_graph\"\n";
                ss << "    }\n";
                ss << "}\n";
                break;

            default: // 500 Internal Server Error
                ss << "Internal Server Error\nC";
                ss << "Content-Type: application/json\n";
                ss << "Access-Control-Allow-Origin: *\n\n";
                ss << "{\n";
                ss << "    \"status\": \"An unknown error occurred\",\n";
                ss << "    \"details\": {\n";
                ss << "        \"error\": \"Internal Server Error\",\n";
                ss << "        \"resolution\": \"Check server logs for detailed error information.\",\n";
                ss << "        \"documentation\": \"https://github.com/algosup/2024-2025-project-3-quickest-path-team-7/blob/main/Document/Functional/FunctionalSpecifications.md\"\n";
                ss << "    }\n";
                ss << "}\n";
                break;
        }
    }

    string response = ss.str();
    send(client_socket, response.c_str(), response.size(), 0);
    closeSocket(client_socket);
    display_error_responses ? println("\n\nERROR response :\n" + response, type::ERROR_LIGHT) : print("");
}


// just for fun
void sendFavicon(int client_socket) {
    ifstream favicon_file(GlobalFiles.api_icon.full, ios::binary | ios::ate);
    if (!favicon_file) {
        // Return 404 if file not found
        stringstream ss;
        if (response_format == "xml") {
            ss << "HTTP/1.1 404 Not Found\n"
               << "Content-Type: application/xml\n\n"
               << "Access-Control-Allow-Origin: *\n"
               << "<response>\n"
               << "    <error>File not found</error>\n"
               << "</response>\n";
        } else {
            ss << "HTTP/1.1 404 Not Found\n"
               << "Content-Type: application/json\n\n"
               << "Access-Control-Allow-Origin: *\n"
               << "{\n"
               << "    \"error\": \"File not found\"\n"
               << "}\n";
        }
        string response = ss.str();
        send(client_socket, response.c_str(), response.size(), 0);
        closeSocket(client_socket);
        display_error_responses ? println("\n\nERROR response :\n" + response, type::ERROR_LIGHT) : print("");
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
            "Access-Control-Allow-Origin: *\r\n"
            "Connection: close\r\n"
            "\r\n";
        
        send(client_socket, header.c_str(), header.size(), 0);
        send(client_socket, buffer.data(), buffer.size(), 0);

        display_valid_responses ? println("\n\nVALID response :\n" + header, type::VALIDATION) : print("");
    }

    closeSocket(client_socket);
    return;
}


#endif // API_RESPONSES_HPP
