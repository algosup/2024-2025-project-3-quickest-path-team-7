# 📋 Project Overview

This project aims to develop a high-performance software solution that calculates the quickest path between two landmarks in the United States. The solution will be exposed as a REST API, capable of handling requests efficiently and returning responses in JSON or XML formats.

The project dataset consists of a large file (`USA-roads.csv`) with approximately 24 million connections between landmarks, representing the travel time between each pair.

# 📂 Project Structure

- `src/`: Contains the C++ source code files
- `README.md`: Project documentation

# ⚙️ Setup and Installation

Clone the repository:
```sh
git clone https://github.com/algosup/2024-2025-project-3-quickest-path-team-7.git
cd 2024-2025-project-3-quickest-path-team-7
```

Build the project using CMake:
```sh
mkdir build
cd build
cmake ..
make
```

Run the server:
```sh
./quickest_path_server
```

# 🛠 Technologies Used

- **C++**: Core language for implementing the algorithms and server
- **Crow**: Lightweight HTTP server for handling REST API requests
- **nlohmann/json**: JSON parsing and serialization
- **PugiXML**: XML parsing and serialization

# 🚀 Features

- **Quick Path Calculation**: Calculates the shortest or approximate quickest path between two landmarks
- **REST API**: Provides a web service for sending requests and receiving responses
- **Multi-Format Support**: Returns responses in JSON or XML
- **Performance Optimization**: Designed to handle large datasets efficiently

# 📚 API Documentation

**Endpoint**: `GET /quickest-path`

**Request Parameters**:
- `start_id` (String): ID of the start landmark
- `end_id` (String): ID of the destination

**Example Request**:
```
GET /quickest-path?start_id=100&end_id=200
```

**Response Formats**:

**JSON**:
```json
{
    "path": ["Landmark_A", "Landmark_B", "Landmark_C"],
    "travel_time": 120
}
```

**XML**:
```xml
<response>
    <path>
        <landmark>Landmark_A</landmark>
        <landmark>Landmark_B</landmark>
        <landmark>Landmark_C</landmark>
    </path>
    <travel_time>120</travel_time>
</response>
```

# 📈 Algorithm Explanation

The project uses the A* (A-Star) algorithm to calculate the quickest path between two landmarks. The algorithm combines:

- `g(n)`: The cost from the start landmark to a given landmark
- `h(n)`: A heuristic estimate of the cost from the current landmark to the destination

This allows the algorithm to prioritize paths that are more likely to lead to the destination faster.

If geographical coordinates (latitude and longitude) are not available, Dijkstra's Algorithm will be used as a fallback.

# 🧪 Testing

Unit tests are provided in the `tests/` folder to ensure the correctness of the pathfinding algorithms and API functionality. Run the tests using:
```sh
make test
```

# 📝 Future Improvements

- Add support for real-time traffic data
- Implement caching to speed up repeated requests
- Explore using Bidirectional Search to further optimize pathfinding

# 👨‍💻 Contributors

- Abderrazaq Makran (Program Manager)
- Elone Dellile (Project Manager)
- Guillaume Deramchi (Technical Lead)
- Pierre Gorin (Quality Assurance)
- Benoît De Keyn (Software Engineer)
- Axel David (Software Engineer)
- Tino Gabet (Technical Writer)

# 📄 License

This project is licensed under the MIT License. See the `LICENSE` file for details.
