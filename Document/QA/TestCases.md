<div align="center">

# Test Cases Document

---

**Title**: Quickest Path - Test Cases

**Team:**: Team 7

**Author:**: Pierre GORIN

**Version**: 1.2

---
</div>

<br><details>
<summary><h2 id="toc"> Table of Contents <i>(Click to expand)</i></h2></summary>

- [Test Cases Document](#test-cases-document)
- [1. - Introduction](#1---introduction)
  - [1.1 - Overview](#11---overview)
  - [1.2 - Scope](#12---scope)
  - [1.3 - Test Status Legend](#13---test-status-legend)
  - [1.4 - Test Case Organization](#14---test-case-organization)
- [2. - Test Cases](#2---test-cases)
  - [2.1 - Unit Tests](#21---unit-tests)
    - [2.1.1 - Dijkstra-Based Tests](#211---dijkstra-based-tests)
      - [TC-UT-DIJK-001](#tc-ut-dijk-001)
      - [TC-UT-DIJK-002](#tc-ut-dijk-002)
      - [TC-UT-DIJK-003](#tc-ut-dijk-003)
    - [2.1.2 Integrity Checker Tests](#212-integrity-checker-tests)
      - [TC-UT-INTCHK-001](#tc-ut-intchk-001)
      - [TC-UT-INTCHK-002](#tc-ut-intchk-002)
    - [2.1.3 Own Algorithm (A\*) Tests](#213-own-algorithm-a-tests)
      - [TC-UT-ASTAR-001](#tc-ut-astar-001)
      - [TC-UT-ASTAR-002](#tc-ut-astar-002)
      - [TC-UT-ASTAR-003](#tc-ut-astar-003)
    - [2.2 - Integration Tests](#22---integration-tests)
      - [TC-IT-001](#tc-it-001)
      - [TC-IT-002](#tc-it-002)
      - [TC-IT-003](#tc-it-003)
  - [2.3 - Functional Tests](#23---functional-tests)
    - [2.3.1 - CLI / Console I/O Tests](#231---cli--console-io-tests)
      - [TC-FT-001](#tc-ft-001)
      - [TC-FT-002](#tc-ft-002)
    - [2.3.2 - API Tests](#232---api-tests)
      - [TC-FT-API-001](#tc-ft-api-001)
      - [TC-FT-API-002](#tc-ft-api-002)
      - [TC-FT-API-003](#tc-ft-api-003)
  - [2.4 - Performance Tests](#24---performance-tests)
      - [TC-PT-001](#tc-pt-001)
      - [TC-PT-002](#tc-pt-002)
  - [2.5 - Smoke Tests](#25---smoke-tests)
      - [TC-SM-001](#tc-sm-001)
      - [TC-SM-002](#tc-sm-002)
  - [3. Traceability Matrix](#3-traceability-matrix)
- [4 - Appendix](#4---appendix)
  - [4.1 - Acronyms and Abbreviations](#41---acronyms-and-abbreviations)
  - [4.2 - References](#42---references)
  - [4.3 - Revision History](#43---revision-history)
  - [4.4 - Document Approval](#44---document-approval)

</details>

# 1. - Introduction

## 1.1 - Overview

This **Test Cases Document** details the various test scenarios for the **Quickest Path** project. It complements the overall **Test Strategy** by enumerating specific test steps.

The system comprises multiple modules:

- **Dijkstra** (in `code/dijkstra/`)
- **Integrity Checker** (in `code/integrity_checker/`)
- **Own Algorithm** (A\* with landmarks, in `code/own_algorithm/`)

In addition, there is an **API** component (in `code/api/`) that exposes the functionality to external clients.

## 1.2 - Scope

- **In Scope**:
  - Unit, integration, functional, performance, and smoke tests for:
    - **Dijkstra** logic
    - **Integrity Checker** logic
    - **Own Algorithm (A\*)** logic
    - **API** endpoints  
- **Out of Scope**:
  - Security/penetration testing
  - Extensive GUI / user experience testing
  - High-scale load/stress testing

## 1.3 - Test Status Legend

| Icon  | Meaning              |
| :---: | -------------------- |
|   ⏳   | **Not executed yet** |
|   ✅   | **Passed**           |
|   ❌   | **Failed**           |
|   🚧   | **In progress**      |
|   📝   | **Blocked**          |
|   🔄   | **Retest needed**    |

When executing each test, update its status with the appropriate icon above.

## 1.4 - Test Case Organization

Each test case follows this structure:

| Field               | Description                                                                          |
| ------------------- | ------------------------------------------------------------------------------------ |
| **Test Case ID**    | Unique identifier (e.g., **TC-UT-DIJK-001**).                                        |
| **Title**           | Short descriptive name.                                                              |
| **Test Type**       | Unit, Integration, Functional, Performance, or Smoke.                                |
| **Preconditions**   | Conditions/data setup before test execution.                                         |
| **Test Data**       | Input values or reference to data sets.                                              |
| **Steps**           | Step-by-step procedure to execute the test.                                          |
| **Expected Result** | Expected outcome (function return, console output, or API response).                 |
| **Status**          | One of the icons from the [Test Status Legend](#13---test-status-legend), Default: ⏳ |
| **Remarks**         | Additional notes (defect references, clarifications, etc.).                          |

# 2. - Test Cases

## 2.1 - Unit Tests

### 2.1.1 - Dijkstra-Based Tests

#### TC-UT-DIJK-001

| Field               | Description                                                                                                     |
| ------------------- | --------------------------------------------------------------------------------------------------------------- |
| **Test Case ID**    | TC-UT-DIJK-001                                                                                                  |
| **Title**           | `dijkstra_algorithm`: Basic Shortest Path Setup                                                                 |
| **Test Type**       | Unit                                                                                                            |
| **Preconditions**   | - `Graph` object with a few known edges.<br>- `PathData` object set to start at node 0, end at node 2.          |
| **Test Data**       | - Graph with edges: `0 -> 1 (weight=2)`, `1 -> 2 (weight=3)`.                                                   |
| **Steps**           | 1. Call `dijkstra_algorithm(graph, path_data, false);`<br>2. Inspect `path_data.distance` and `path_data.prev`. |
| **Expected Result** | - `path_data.distance[2] = 5`<br>- Path reconstruction yields `[0, 1, 2]`                                       |
| **Status**          | ✅                                                                                                               |
| **Remarks**         | Basic check of correctness for a small graph.                                                                   |

#### TC-UT-DIJK-002

| Field               | Description                                                                                                                          |
| ------------------- | ------------------------------------------------------------------------------------------------------------------------------------ |
| **Test Case ID**    | TC-UT-DIJK-002                                                                                                                       |
| **Title**           | `takeUserInput`: Invalid Node                                                                                                        |
| **Test Type**       | Unit                                                                                                                                 |
| **Preconditions**   | - Program compiled with `takeUserInput` in `input.hpp`.<br>- Large graph or mock environment to test invalid indexes.                |
| **Test Data**       | - Provide negative or excessively large node ID.                                                                                     |
| **Steps**           | 1. Supply `-10` or `999999999` via a mock or direct call to `takeUserInput(...)`.<br>2. Observe the return value and console output. |
| **Expected Result** | - Returns `-1` (invalid node) and prints an error message.                                                                           |
| **Status**          | ✅                                                                                                                                    |
| **Remarks**         | Ensures function detects invalid node input.                                                                                         |

#### TC-UT-DIJK-003

| Field               | Description                                                                                                                                     |
| ------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------- |
| **Test Case ID**    | TC-UT-DIJK-003                                                                                                                                  |
| **Title**           | `buildGraph`: Proper Backup Creation                                                                                                            |
| **Test Type**       | Unit                                                                                                                                            |
| **Preconditions**   | - `Files` struct set with a known CSV path.<br>- CSV file is valid and present.                                                                 |
| **Test Data**       | - `files.dataset = "USA-roads.csv"`.                                                                                                            |
| **Steps**           | 1. Call `buildGraph(graph, files, false)`. <br>2. Check if `files.backup` (e.g. `graph-backup.bin`) is created.<br>3. Validate the graph loads. |
| **Expected Result** | - Binary backup file exists.<br>- `graph.loaded == true`.                                                                                       |
| **Status**          | ✅                                                                                                                                               |
| **Remarks**         | Validates building the graph from CSV and saving to binary backup.                                                                              |

### 2.1.2 Integrity Checker Tests

#### TC-UT-INTCHK-001

| Field               | Description                                                                                      |
| ------------------- | ------------------------------------------------------------------------------------------------ |
| **Test Case ID**    | TC-UT-INTCHK-001                                                                                 |
| **Title**           | `loadGraph`: Load from Backup                                                                    |
| **Test Type**       | Unit                                                                                             |
| **Preconditions**   | - A valid backup file (e.g. `USA-roads_graph.bin`) already exists.<br>- `Graph` struct is empty. |
| **Test Data**       | - `BACKUP = "USA-roads_graph.bin"`.<br>- `DATASET = "USA-roads.csv"`.                            |
| **Steps**           | 1. Call `loadGraph(graph, false)`. <br>2. Inspect `graph.data.size()` after load.                |
| **Expected Result** | - `graph.data.size()` > 0.<br>- No error messages.                                               |
| **Status**          | 🚧                                                                                                |
| **Remarks**         | Confirms the integrity checker can load from existing backup if not forced to reload CSV.        |

#### TC-UT-INTCHK-002

| Field               | Description                                                                                       |
| ------------------- | ------------------------------------------------------------------------------------------------- |
| **Test Case ID**    | TC-UT-INTCHK-002                                                                                  |
| **Title**           | `saveGraphToBinary`: File Writing                                                                 |
| **Test Type**       | Unit                                                                                              |
| **Preconditions**   | - `graph.data` is already populated.                                                              |
| **Test Data**       | - Output filename: `"test_graph.bin"`.                                                            |
| **Steps**           | 1. Call `saveGraphToBinary("test_graph.bin", graph.data)`. <br>2. Verify `test_graph.bin` exists. |
| **Expected Result** | - `test_graph.bin` file size > 0.<br>- No I/O errors.                                             |
| **Status**          | 🚧                                                                                                 |
| **Remarks**         | Checks basic I/O operation of writing the graph to disk.                                          |

### 2.1.3 Own Algorithm (A\*) Tests

#### TC-UT-ASTAR-001

| Field               | Description                                                                                                                                                                                                    |
| ------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Test Case ID**    | TC-UT-ASTAR-001                                                                                                                                                                                                |
| **Title**           | `estimate_distance`: Landmark-Based Heuristic                                                                                                                                                                  |
| **Test Type**       | Unit                                                                                                                                                                                                           |
| **Preconditions**   | - `graph.landmarks` has ≥2 landmarks.<br>- `graph.landmark_distance[node][landmark_idx]` is valid.                                                                                                             |
| **Test Data**       | - Suppose `graph.landmarks = [10, 20]`. <br> - `graph.landmark_distance[5][0] = 15`, `graph.landmark_distance[5][1] = 40`. <br> - `graph.landmark_distance[25][0] = 55`, `graph.landmark_distance[25][1] = 5`. |
| **Steps**           | 1. Call `estimate_distance(graph, 5, 25)`. <br>2. Observe returned integer.                                                                                                                                    |
| **Expected Result** | - `abs(15-55) = 40`, `abs(40-5) = 35`, pick `max(40,35) = 40`. <br>- Return = 40.                                                                                                                              |
| **Status**          | ✅                                                                                                                                                                                                              |
| **Remarks**         | Validates the maximum-difference logic across multiple landmarks.                                                                                                                                              |

#### TC-UT-ASTAR-002

| Field               | Description                                                                                                 |
| ------------------- | ----------------------------------------------------------------------------------------------------------- |
| **Test Case ID**    | TC-UT-ASTAR-002                                                                                             |
| **Title**           | `shortestPaths`: Single-Source Distances                                                                    |
| **Test Type**       | Unit                                                                                                        |
| **Preconditions**   | - `graph.map` has a small set of nodes with known distances.                                                |
| **Test Data**       | - E.g. `graph.map[0] -> { {1, 5}, {2, 10} }`.                                                               |
| **Steps**           | 1. Call `shortestPaths(graph, 0)`. <br>2. Check returned vector for correct minimal distances to each node. |
| **Expected Result** | - `distances[1] = 5`, `distances[2] = 10`, etc.                                                             |
| **Status**          | ✅                                                                                                           |
| **Remarks**         | Basic check of single-source BFS/Dijkstra-like approach used by `shortestPaths`.                            |

#### TC-UT-ASTAR-003

| Field               | Description                                                                                                   |
| ------------------- | ------------------------------------------------------------------------------------------------------------- |
| **Test Case ID**    | TC-UT-ASTAR-003                                                                                               |
| **Title**           | `buildLandmarks`: Landmark Selection                                                                          |
| **Test Type**       | Unit                                                                                                          |
| **Preconditions**   | - A small graph (~10 nodes) in `graph.map` for quick computations.                                            |
| **Test Data**       | - Manually constructed adjacency or a small test CSV.                                                         |
| **Steps**           | 1. Call `buildLandmarks(graph)`. <br>2. Confirm `graph.landmarks.size() == LANDMARKS_QTY`.                    |
| **Expected Result** | - The function picks distinct landmarks. <br>- `graph.landmark_distance[node][lm_idx]` is properly populated. |
| **Status**          | ✅                                                                                                             |
| **Remarks**         | Validates the farthest-node approach in selecting multiple landmarks.                                         |

### 2.2 - Integration Tests

#### TC-IT-001

| Field               | Description                                                                                  |
| ------------------- | -------------------------------------------------------------------------------------------- |
| **Test Case ID**    | TC-IT-001                                                                                    |
| **Title**           | Dijkstra + CSV + Backup Flow                                                                 |
| **Test Type**       | Integration                                                                                  |
| **Preconditions**   | - `USA-roads.csv` is present.<br>- No binary backup initially or use `force = true`.         |
| **Test Data**       | - `buildGraph(graph, files, true)`.                                                          |
| **Steps**           | 1. Force building the graph from CSV.<br>2. Run `dijkstra_algorithm(...)`.                   |
| **Expected Result** | - Graph loads from CSV, then creates backup.<br>- Running Dijkstra yields correct distances. |
| **Status**          | ⏳                                                                                            |
| **Remarks**         | Validates end-to-end usage of CSV parsing → in-memory graph → Dijkstra → backup creation.    |

#### TC-IT-002

| Field               | Description                                                                                                              |
| ------------------- | ------------------------------------------------------------------------------------------------------------------------ |
| **Test Case ID**    | TC-IT-002                                                                                                                |
| **Title**           | A\* + Landmark Building + Backup                                                                                         |
| **Test Type**       | Integration                                                                                                              |
| **Preconditions**   | - Graph is built from CSV or binary if it exists.<br>- Landmarks are built or loaded from backup.                        |
| **Test Data**       | - `LANDMARKS_QTY = 10`.                                                                                                  |
| **Steps**           | 1. `loadGraph(graph, files);`<br>2. `loadLandmarks(graph, files);`<br>3. `find_path(graph, path_data, astar1, astar2);`. |
| **Expected Result** | - 10 landmarks are loaded or created.<br>- `find_path` returns correct route and distance.                               |
| **Status**          | ⏳                                                                                                                        |
| **Remarks**         | Tests synergy among building/loading the map, building/loading landmarks, and performing a final A\* search.             |

#### TC-IT-003

| Field               | Description                                                                                                                        |
| ------------------- | ---------------------------------------------------------------------------------------------------------------------------------- |
| **Test Case ID**    | TC-IT-003                                                                                                                          |
| **Title**           | Integrity Checker + Dijkstra                                                                                                       |
| **Test Type**       | Integration                                                                                                                        |
| **Preconditions**   | - `checker.cpp` can generate or verify partial structures (some code is placeholder).                                              |
| **Test Data**       | - Potential DAG or partial data.                                                                                                   |
| **Steps**           | 1. In `checker.cpp`, build a partial DAG (placeholder). <br>2. In `dijkstra/path_finder.cpp`, run normal flow.                     |
| **Expected Result** | - No data conflict if DAG is valid in the checker.<br>- Dijkstra runs normally on the same data.                                   |
| **Status**          | ⏳                                                                                                                                  |
| **Remarks**         | Example test verifying no data corruption between the two modules. Currently partial because of placeholder code in `checker.cpp`. |


## 2.3 - Functional Tests

### 2.3.1 - CLI / Console I/O Tests


#### TC-FT-001

| Field               | Description                                                                                                            |
| ------------------- | ---------------------------------------------------------------------------------------------------------------------- |
| **Test Case ID**    | TC-FT-001                                                                                                              |
| **Title**           | Dijkstra `main()`: Valid Start/End CLI                                                                                 |
| **Test Type**       | Functional                                                                                                             |
| **Preconditions**   | - `dijkstra/path_finder.cpp` compiled into an executable.                                                              |
| **Test Data**       | - Start node: 100, End node: 200.                                                  |
| **Steps**           | 1. Run the executable.<br>2. Enter `100` for start node.<br>3. Enter `200` for end node.<br>4. Observe console output. |
| **Expected Result** | - Outputs total distance, edges count, and calculation time.<br>- Saves `shortest_path.csv`.                           |
| **Status**          | ✅                                                                                                                      |
| **Remarks**         | Ensures the main loop, `takeUserInput`, `dijkstra`, and file-writing steps all run in a typical scenario.              |

#### TC-FT-002

| Field               | Description                                                                                                          |
| ------------------- | -------------------------------------------------------------------------------------------------------------------- |
| **Test Case ID**    | TC-FT-002                                                                                                            |
| **Title**           | Own Algorithm `main()`: Landmarks + A\* Workflow                                                                     |
| **Test Type**       | Functional                                                                                                           |
| **Preconditions**   | - `own_algorithm/main.cpp` compiled.<br>- Graph and landmarks are built or will build on first run.                  |
| **Test Data**       | - Start node: 500, End node: 10000.                                                           |
| **Steps**           | 1. Launch the A\* executable.<br>2. Provide start node `500`, end node `10000`.<br>3. Observe calculation output.    |
| **Expected Result** | - Displays total distance, estimated distance, error %, edges count, timing.<br>- Saves path to `shortest-path.csv`. |
| **Status**          | ⏳                                                                                                                    |
| **Remarks**         | Confirms main functional flow for A\* approach with landmarks and concurrency.                                       |

### 2.3.2 - API Tests

#### TC-FT-API-001

| Field               | Description                                                                     |
| ------------------- | ------------------------------------------------------------------------------- |
| **Test Case ID**    | TC-FT-API-001                                                                   |
| **Title**           | `GET /status` Endpoint                                             |
| **Test Type**       | Functional                                                                      |
| **Preconditions**   | - Local server exposing a `/status` endpoint.<br>- Server started. |
| **Test Data**       | - N/A                                                                           |
| **Steps**           | 1. Send HTTP `GET /status`.<br>2. Observe response code and payload.            |
| **Expected Result** | - `200 OK` with JSON (e.g. `{"status":"ok"}`).                                  |
| **Status**          | ✅                                                                               |
| **Remarks**         | Ensures the service is reachable and returns a standard heartbeat response.     |

#### TC-FT-API-002

| Field               | Description                                                                                                  |
| ------------------- | ------------------------------------------------------------------------------------------------------------ |
| **Test Case ID**    | TC-FT-API-002                                                                                                |
| **Title**           | `POST /calculate` Endpoint (Dijkstra/A\*)                                                                    |
| **Test Type**       | Functional                                                                                                   |
| **Preconditions**   | - API server running.<br>- Graph in memory or loaded from backup.                                            |
| **Test Data**       | ```json { "start": 500, "end": 10000 }```                                                                    |
| **Steps**           | 1. POST to `/calculate` with the JSON body.<br>2. Verify returned JSON with `distance`, `path`, etc.         |
| **Expected Result** | - `200 OK` with JSON response, e.g. `{ "distance": 12345, "path": [500,...,10000], "calculationTime": 45 }`. |
| **Status**          | ⏳                                                                                                            |
| **Remarks**         | Checks if the API calls the internal Dijkstra/A\* logic and returns valid results in JSON.                   |

#### TC-FT-API-003

| Field               | Description                                                                                 |
| ------------------- | ------------------------------------------------------------------------------------------- |
| **Test Case ID**    | TC-FT-API-003                                                                               |
| **Title**           | `POST /calculate` Invalid Input                                                             |
| **Test Type**       | Functional                                                                                  |
| **Preconditions**   | - API server running.<br>- Node validation in place.                                        |
| **Test Data**       | ```json { "start": -1, "end": 50000 }```                                                    |
| **Steps**           | 1. POST to `/calculate` with invalid node IDs.<br>2. Observe HTTP status and response body. |
| **Expected Result** | - `400 Bad Request` or `422 Unprocessable Entity` with an error message (`"Invalid node"`). |
| **Status**          | ✅                                                                                           |
| **Remarks**         | Verifies API handles invalid nodes gracefully.                                              |

## 2.4 - Performance Tests

#### TC-PT-001

| Field               | Description                                                                                                                  |
| ------------------- | ---------------------------------------------------------------------------------------------------------------------------- |
| **Test Case ID**    | TC-PT-001                                                                                                                    |
| **Title**           | Single-Query Response Time (Dijkstra/A\*)                                                                                    |
| **Test Type**       | Performance                                                                                                                  |
| **Preconditions**   | - System compiled.<br>- A typical start/end node pair covering a medium distance.                                            |
| **Test Data**       | - Start = 1000, End = 20000.                                                                                                 |
| **Steps**           | 1. Run the program (or API) for path `1000 -> 20000`. <br>2. Measure time using an external tool. |
| **Expected Result** | - Execution `< 1000 ms`.<br>- No major memory spikes.                                                                        |
| **Status**          | ✅                                                                                                                            |
| **Remarks**         | Basic performance test for a single typical query.                                                                           |

#### TC-PT-002

| Field               | Description                                                                                                                  |
| ------------------- | ---------------------------------------------------------------------------------------------------------------------------- |
| **Test Case ID**    | TC-PT-002                                                                                                                    |
| **Title**           | Burst Requests / Concurrency                                                                                                 |
| **Test Type**       | Performance                                                                                                                  |
| **Preconditions**   | - Script (Python/Postman runner) to send multiple queries quickly (if API). <br>- Or repeated CLI calls in quick succession. |
| **Test Data**       | - 10 to 50 consecutive requests with random start/end pairs.                                                                 |
| **Steps**           | 1. Trigger queries in a short window (≤5 seconds). <br>2. Observe average response time and success rate.                    |
| **Expected Result** | - No crash.<br>- 95% of requests respond under 1000 ms.                                                                      |
| **Status**          | 🚧                                                                                                                            |
| **Remarks**         | Validates concurrency aspects of A\* bidirectional search.                                                                   |

## 2.5 - Smoke Tests

#### TC-SM-001

| Field               | Description                                                                   |
| ------------------- | ----------------------------------------------------------------------------- |
| **Test Case ID**    | TC-SM-001                                                                     |
| **Title**           | CLI Launch & Basic Calculation                                                |
| **Test Type**       | Smoke                                                                         |
| **Preconditions**   | - Executable compiled.                                                        |
| **Test Data**       | - Start = 1, End = 2, trivially connected.                                    |
| **Steps**           | 1. Launch console program (Dijkstra or A\*).<br>2. Provide minimal start/end. |
| **Expected Result** | - Program doesn’t crash.<br>- Returns a sensible distance.                    |
| **Status**          | 🚧                                                                             |
| **Remarks**         | Quick check that data loads and a minimal path is found.                      |

#### TC-SM-002

| Field               | Description                                                       |
| ------------------- | ----------------------------------------------------------------- |
| **Test Case ID**    | TC-SM-002                                                         |
| **Title**           | API Endpoint Responsive                                           |
| **Test Type**       | Smoke                                                             |
| **Preconditions**   | - API server up.                                  |
| **Test Data**       | - `GET /status`.                                 |
| **Steps**           | 1. Hit the health check endpoint.<br>2. Observe HTTP status code. |
| **Expected Result** | - `200 OK` promptly.                                              |
| **Status**          | 🚧                                                                 |
| **Remarks**         | Simple check that the server runs, ignoring deeper correctness.   |

## 3. Traceability Matrix

| **Test Case ID** | **Requirement**                                         | **Status** |
| ---------------- | ------------------------------------------------------- | ---------- |
| TC-UT-DIJK-001   | Must calculate shortest path accurately (Dijkstra).     | ✅          |
| TC-UT-ASTAR-001  | Must calculate shortest path accurately (A\*).          | ✅          |
| TC-UT-ASTAR-002  | Must calculate shortest path accurately (A\*).          | ✅          |
| TC-IT-001        | Must calculate shortest path accurately (Dijkstra).     | ⏳          |
| TC-IT-002        | Must calculate shortest path accurately (A\*).          | ⏳          |
| TC-IT-003        | Must calculate shortest path accurately (A\*).          | ⏳          |
| TC-FT-001        | Must calculate shortest path accurately (Dijkstra).     | ✅          |
| TC-FT-002        | Must calculate shortest path accurately (A\*).          | ⏳          |
| TC-PT-001        | Must respond within ~1000ms for typical queries.        | ✅          |
| TC-PT-002        | Must respond within ~1000ms for typical queries.        | 🚧          |
| TC-UT-DIJK-002   | Must handle invalid inputs.                             | ✅          |
| TC-FT-API-003    | Must handle invalid inputs.                             | ✅          |
| TC-UT-DIJK-003   | Must allow building/loading from CSV and backups.       | ✅          |
| TC-UT-INTCHK-001 | Must allow building/loading from CSV and backups.       | 🚧          |
| TC-UT-INTCHK-002 | Must allow building/loading from CSV and backups.       | 🚧          |
| TC-IT-001        | Must allow building/loading from CSV and backups.       | ⏳          |
| TC-IT-002        | Must allow building/loading from CSV and backups.       | ⏳          |
| TC-IT-003        | Must allow building/loading from CSV and backups.       | ⏳          |
| TC-UT-ASTAR-001  | Must perform concurrency in A\* (bidirectional search). | ✅          |
| TC-UT-ASTAR-003  | Must perform concurrency in A\* (bidirectional search). | ✅          |
| TC-IT-002        | Must perform concurrency in A\* (bidirectional search). | ⏳          |
| TC-PT-002        | Must perform concurrency in A\* (bidirectional search). | 🚧          |
| TC-SM-001        | Must have minimal smoke checks post-build.              | 🚧          |
| TC-SM-002        | Must have minimal smoke checks post-build.              | 🚧          |
| TC-FT-API-001    | Potential API endpoints.                                | ✅          |
| TC-FT-API-002    | Potential API endpoints.                                | ✅          |
| TC-FT-API-003    | Potential API endpoints.                                | ✅          |

# 4 - Appendix

## 4.1 - Acronyms and Abbreviations

- **API**: Application Programming Interface
- **CSV**: Comma-Separated Values
- **DAG**: Directed Acyclic Graph
- **HTTP**: Hypertext Transfer Protocol
- **Endpoint**: A specific URL within an API

## 4.2 - References

- [Quality Assurance](https://en.wikipedia.org/wiki/Software_quality_assurance)
- [Dijkstra's Algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm)
- [A* Search Algorithm](https://en.wikipedia.org/wiki/A*_search_algorithm)
- [API](https://en.wikipedia.org/wiki/API)
- [API Testing](https://www.softwaretestinghelp.com/api-testing/)

## 4.3 - Revision History

| **Version** | **Date**   | **Author**   | **Changes**                                |
| ----------- | ---------- | ------------ | ------------------------------------------ |
| 1.0         | 2025-01-30 | Pierre GORIN | First Draft of Test Cases Document         |
| 1.1         | 2025-01-31 | Pierre GORIN | Added more test cases and few corrections. |
| 1.2         | 2025-02-03 | Pierre GORIN | Rework of the document structure.          |
| 1.3         | 2025-02-07 | Pierre GORIN | Updated test cases.                        |

## 4.4 - Document Approval

| Reviewer           | Role              | Approved | Date |
| ------------------ | ----------------- | -------- | ---- |
| Abderrazaq MAKRAN  | Program Manager   | ✅        | 2025-01-30 |
| Elone DELILLE      | Project Manager   | ✅        | 2025-01-30     |
| Guillaume DERAMCHI | Technical Lead    | ✅        | 2025-01-30     |
| Pierre GORIN       | Quality Assurance | ✅        | 2025-01-30     |
| Benoît DE KEYN     | Software Engineer | ✅        | 2025-01-30     |
| Axel DAVID         | Software Engineer | ✅        | 2025-01-30     |