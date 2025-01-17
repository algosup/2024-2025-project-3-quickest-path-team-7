<div align="center">

# Functional Specifications

**Title:** Quickest Path

**Author:** Abderrazaq MAKRAN

**Team:**  7

**Reviewer:** Pierre GORIN

**Created on:** January 6<sup>th</sup>, 2025

**Last updated:** January 16<sup>th</sup>, 2025

---
---
## Document History
| Date       | Version | Document Revision Description | Document Author   |
|------------|---------|-------------------------------|-------------------|
| 01/06/2025 | 0       | Create template               | Abderrazaq MAKRAN |
| 01/16/2025 | 0.5       | Finished Intro (have to check data validation and add back to top) + Doing Functional API Details               | Abderrazaq MAKRAN |
---

## Stakeholders
| Name           | Occupation                  | Links                          |
|----------------|-----------------------------|--------------------------------|
| Franck JEANNIN | Client (ALGOSUP's director) | [Website](https://algosup.com) |

---
</div>

<br><details>
<summary><h2 id="toc"> Table of Contents <i>(Click to expand)</i></h2></summary>

1. [Introduction](#1-introduction)
   - [1.1 Project Scope](#11-project-scope)  
   - [1.2 Project Team](#12-project-team)  
   - [1.3 Project Reviewers](#13-project-reviewers)  
   - [1.4 Deliverables](#14-deliverables)  
   - [1.5 Milestones](#15-milestones)  
   - [1.6 Terms, Acronyms, and Definitions](#16-terms-acronyms-and-definitions)  
2. [System](#2-system)

</details>

---

# 1. Introduction
This document defines the functional specifications of the Quickest Path project, which aims to develop a software solution in C++ that calculates the fastest path between two landmarks in the USA. The system will expose its functionality via a REST API supporting JSON and XML response formats. The main goal is to deliver fast, accurate results within the constraints specified by the client.

## 1.1 Project Scope
| **Feature**                              | **Description**                                                                                 | **In Scope** | **Out of Scope** |
|------------------------------------------|-------------------------------------------------------------------------------------------------|--------------|------------------|
| **Shortest Path Calculation**            | The system must calculate the shortest path between two landmarks using heuristic algorithms.   | ✅            |                  |
| **REST API**                             | The system must expose functionality via a REST API with a single GET endpoint, supporting JSON and XML response formats.   | ✅            |                  |
| **Data Validation**  ```TO CHECK```                    | The system must validate the integrity and connectivity of the input dataset.                  | ✅            |                  |
| **Heuristic Optimization**               | The system must use heuristics to maintain performance while staying within the 10% error margin. | ✅            |                  |
| **Multi-Format Support**                 | The API must provide outputs in both JSON and XML formats.                                      | ✅            |                  |
| **Real-Time Responses**                  | The system must deliver responses within 1 second for a standard laptop setup.                 | ✅            |                  |
| **Batch Processing**                     | The system must support batch route calculations for multiple source-destination pairs.         |              | ❌               |
| **User Authentication**                  | The system must authenticate users accessing the API.                                          |              | ❌               |
| **Offline Functionality**                | The system must function offline without internet connectivity.                                |              | ❌               |
| **Graphical Interface**                  | The system must provide a graphical user interface for user interaction.                       |              | ❌               |
| **Voice Input**                          | The system must support voice input for navigation requests.                                   |              | ❌               |

---

## 1.2 Project Team
| Role              | Description                                                                                                                                                                | Name                                                                 |
|-------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------|
| Project Manager   | - In charge of organization, planning, and budgeting.<br>- Ensures communication within the team and keeps them motivated.                                                 | [Elone DELILLE](https://www.linkedin.com/in/elonedelille/)           |
| Program Manager   | - Ensures the project meets the client's expectations.<br>- Acts as the intermediary between the client and the development team.<br>- Responsible for writing the Functional Specifications. | [Abderrazaq MAKRAN](https://www.linkedin.com/in/abderrazaq-makran/)  |
| Tech Lead         | - Makes technical decisions for the project.<br>- Translates the Functional Specifications into Technical Specifications.                                  | [Guillaume DERAMCHI](https://www.linkedin.com/in/guillaume-deramchi/)| 
| Technical Writer  | - Writes the documentation for the project.<br>- Based on the specifications, creates a manual for end-users.                                                              | [Tino GABET](https://www.linkedin.com/in/tino-gabet-5794bb32a/)      |
| Software Engineer | - Writes the code and unit tests, comments, and documents the code.<br>- Participates in technical decisions.                                                              | [Benoit DE KEYN](https://www.linkedin.com/in/benoît-de-keyn-71611b293/), [Axel DAVID](https://www.linkedin.com/in/axel-david-6384bb32a/) |
| Quality Assurance | - Tests all product functionalities to find bugs and issues.<br>- Defines the test strategy and writes the Test Plan.<br>- Documents all encountered bugs and ensures their resolution. | [Pierre GORIN](https://www.linkedin.com/in/pierre-gorin-61a784221/)  |

---

## 1.3 Project Reviewers
External project reviewers have been appointed by the project owner to review our specifications and provide feedback.

---

## 1.4 Deliverables
| Deliverable                               | Link to the Document                                          |
|-------------------------------------------|---------------------------------------------------------------|
| Functional Specifications                 | [Functional Specifications](./Functional-Specifications.md)   |
| Technical Specifications                  | [Technical Specifications](./Technical-Specifications.md)     |
| Test Plan                                 | [Test Plan](./Test-Plan.md)                                   |
| User Manual                               | [User Manual](./User-Manual.md)                               |
| Code                                      | [Code Repository](../../Src/software_team3/software_team3.ino)|

---

## 1.5 Milestones
| Milestone                      | Date       |
|--------------------------------|------------|
| First Weekly Report            | 01/10/2025 |
| Functional Specifications (v1) | 01/20/2025 |
| Second Weekly Report           | 01/17/2025 |
| Technical Specifications (v1)  | 01/31/2025 |
| Proof of Concept (POC)         | 01/25/2025 |
| MVP (Minimum Viable Product)   | 01/30/2025 |
| Test Plan                      | 02/03/2025 |
| User Manual                    | 02/03/2025 |
| Final Code                     | 02/03/2025 |
| Final Presentation             | 02/07/2025 |
| Project Post-Mortem            | 02/13/2025 |

---

## 1.6 Terms, Acronyms, and Definitions
| Term/Acronym                | Definition                                                                                  |
|-----------------------------|----------------------------------------------------------------------------------------------|
| Functional Specifications   | A document explaining what the system needs to do and how it should work.                   |
| Technical Specifications    | A document detailing how the system will be built, including algorithms and data structures.|
| Test Plan                   | A document describing the testing strategy for the system.                                   |
| REST API                    | A web service interface that allows communication using standard HTTP methods.              |
| JSON                        | A lightweight data format that is easy to read and write for both humans and machines.      |
| XML                         | A markup language designed to store and transport data in a structured format.              |
| Heuristic Algorithm         | A problem-solving approach prioritizing speed over precision.                               |
| Dataset                     | A structured collection of data used for analysis and processing.                           |
| Bidirectional Connections   | Connections that allow navigation in both directions between nodes.                         |




## 1.7 Requirements, Constraints, and Assumptions

### 1.7.1 Requirements
**Language:**  
The system must be developed in C++ to optimize performance for graph computations and align with project requirements.

**REST API:**  
The system will expose its functionality through a REST API:
- **Input:** Accepts the source and destination landmark IDs as query parameters.
- **Output:** Returns the total travel time and the ordered list of landmarks along the path.
- **Response Formats:** Supports JSON (default) and XML formats.
- **Error Handling:** The API must handle and respond with appropriate HTTP status codes (e.g., 200, 400, 404) and descriptive error messages.

**Graph Validation (One-Time Check):**  
The dataset will be processed to generate a graph, which will undergo a one-time validation to:
- Ensure it is free of loops (cycles).
- Verify it is fully connected, ensuring a path exists between any two landmarks.

This graph will be preloaded into memory and reused for all subsequent queries during the localhost session.

**Performance:**  
- Respond to all queries within 1 second on a standard development machine (laptop).
- Allow a heuristic margin of up to 10% from the shortest path for performance optimization.

**Environment:**  
- The system will run exclusively on a localhost environment for development and testing purposes.

---

### 1.7.2 Constraints
**Timeline:**  
The project must adhere to the defined timeline and milestones.

**Hardware:**  
The system must operate efficiently on a standard development laptop with limited computational resources (e.g., 8 GB RAM, quad-core processor).

**Graph Validation Frequency:**  
The graph validation check must only be performed once during the initialization phase and will not be repeated for each query.

**Environment:**  
The system does not require internet connectivity for operation, as all computations are performed locally. Interaction is limited to local API calls (e.g., `http://localhost:8080`).

---

### 1.7.3 Assumptions
- The provided dataset is accurate and does not require transformation or corrections beyond loop validation and connectivity checks.
- Users will provide valid and existing landmark IDs for source and destination queries.
- The system is expected to handle only a limited number of concurrent queries during localhost testing.
- The API will not require authentication or encryption for localhost development.


---

## 1.8 Risks and Challenges

### 1.8.1 Risks  
1. **Dataset Size**: Large datasets (24 million nodes) may cause performance or memory challenges.  
2. **Algorithm Accuracy**: The use of heuristics for approximations could lead to deviations exceeding the acceptable 10% margin in rare cases.  
3. **System Load**: High query volumes could overload the system, particularly on constrained hardware.  

### 1.8.2 Challenges  
- **Speed vs. Accuracy**: Ensuring that heuristic optimizations maintain a balance between fast response times and acceptable error margins.  
- **Graph Size Management**: Efficiently managing large datasets in memory for multiple queries without excessive resource consumption.  
- **Robust Error Handling**: Consistently identifying and communicating errors in input or system operations without impacting the user experience.  

---

## 1.9 Personas and Use Cases

### 1.9.1 Personas

#### **Persona 1: Sarah, the Backend Developer**
- **Goals**: Integrate the API into a logistics platform for real-time route calculations.  
- **Needs**: Comprehensive API documentation and predictable response times.  
- **Pain Points**: Struggles with datasets that lack clarity or APIs with inconsistent performance.  

#### **Persona 2: Hamid, the Data Scientist**
- **Goals**: Validate the algorithm’s accuracy and analyze performance across large datasets.  
- **Needs**: Clear documentation on heuristic error margins and bulk result validation capabilities.  
- **Pain Points**: Requires well-structured data outputs for downstream processing.  

#### **Persona 3: Leo, the DevOps Engineer**
- **Goals**: Deploy the system on a scalable cloud platform and monitor its performance under heavy load.  
- **Needs**: Precise resource utilization details and robust error handling to ensure uptime.  
- **Pain Points**: Difficulty in deploying systems with unclear technical specifications or insufficient error reporting.  

---

### 1.9.2 Use Case: Calculating the Quickest Path

**Scenario**: A backend developer uses the API to calculate delivery routes for a logistics application.  

#### Input
- Source and destination landmark IDs as query parameters (e.g., `?source=123&destination=456`).  
- Optionally specify response format as `format=json` (default) or `format=xml`.

#### Process
1. The developer submits a GET request with the required parameters.  
2. The system validates the input and checks for errors.  
3. The system retrieves the shortest path from the preloaded graph.  
4. The system formats the result in the requested format (JSON or XML).  
5. The system responds with the calculated travel time and the ordered list of landmarks.

#### Output
##### **200 OK**
- **When**: The query is successful, and the path is found.  
- **Example JSON Response**:  
  ```json
  {
      "time": 120,
      "path": ["123", "234", "345", "456"]
  }
- **Example XML Response**:
  ```XML
        <response>
            <time>120</time>
            <path>
                <landmark>123</landmark>
                <landmark>234</landmark>
                <landmark>345</landmark>
                <landmark>456</landmark>
            </path>
        </response>
  ```
  
# 2. System

## 2.1 Functional API Details

### **2.1.1 Endpoints**

This project will use a single `GET` request method to handle all queries, ensuring simplicity and consistency in the API design.

#### **Endpoint**: `GET /quickest-path`
- **Description**: Calculates the quickest path between two landmarks in the graph.
- **Accepted Headers**:
  - `Accept`: Specifies the desired response format. Supported values are:
    - `application/json` (default)
    - `application/xml`
- **Query Parameters**:
  - `source` (required): The ID of the starting landmark (A).
  - `destination` (required): The ID of the destination landmark (B).
  - `format` (optional): Overrides the `Accept` header to specify the response format. Accepted values are `json` and `xml`.

#### **Request Examples**:

**1. Request with `Accept` Header**:
```plaintext
GET /quickest-path?source=123&destination=456 HTTP/1.1
Host: localhost:8080
Accept: application/json
```
**2. Request with `Query` Parameter**:
```
GET http://localhost:8080/quickest-path?source=123&destination=456&format=xml
```
## 2.2 Response Details
### 2.2.1 Success Response
- **200 OK**: The path is successfully calculated.
- **Response**:
  - The response contains the calculated travel time (in arbitrary units, such as minutes, hours, or km/h)  and the ordered list of landmarks.
  - The format depends on the format query parameter in the request.
    - **Example Responses**:
      - **Default (JSON)**:
        ```json
        {
            "time": 145,
            "path": ["123", "234", "345", "456"]
        }
      - **Example XML Response**:
        ```XML
              <response>
                  <time>145</time>
                  <path>
                      <landmark>123</landmark>
                      <landmark>234</landmark>
                      <landmark>345</landmark>
                      <landmark>456</landmark>
                  </path>
              </response>
        ```

### 2.2.2 Error Responses
The API returns appropriate error codes with descriptive messages in the requested format (json by default):
  - **400 Bad Request**: Malformed request or missing parameters.

    - **Default JSON:** 
      ```json
      {
        "status": "Invalid or missing parameters.",
        "code": "ERR4001",
        "details": {
          "missing_parameters": ["source", "destination"],
          "resolution": "Ensure both 'source' and 'destination' are included as query parameters.",
          "documentation": "https://example.com/docs#parameters"
        }
      }
      ```
    - **Requested XML**:
      ```XML
        <status>
            <message>Invalid or missing parameters.</message>
            <code>ERR4001</code>
            <details>
                <missing_parameters>
                    <parameter>source</parameter>
                    <parameter>destination</parameter>
                </missing_parameters>
                <resolution>Ensure both 'source' and 'destination' are included as query parameters.</resolution>
                <documentation>https://example.com/docs#parameters</documentation>
            </details>
        </error>

      ```
  - **404 Not Found:** Invalid source or destination ID.

    - **Default JSON:** 
      ```json
      {
        "status": "Landmark not found.",
        "code": "ERR4041",
        "details": {
          "landmark_id": "123",
          "resolution": "Check the dataset for valid landmark IDs.",
          "documentation": "https://example.com/docs#landmarks"
        }
      }
      ```
    - **Requested XML**:
      ```XML
        <status>
            <message>Landmark not found.</message>
            <code>ERR4041</code>
            <details>
                <landmark_id>123</landmark_id>
                <resolution>Check the dataset for valid landmark IDs.</resolution>
                <documentation>https://example.com/docs#landmarks</documentation>
            </details>
        </error>
      ```
  - **500 Internal Server Error**: Unexpected server-side error.

    - **Default JSON:** 
      ```json
      {
        "status": "Unexpected server-side error.",
        "code": "ERR5001",
        "details": {
          "timestamp": "2025-01-16T14:30:00Z",
          "support": "Contact support@example.com with the timestamp and request details."
        }
      }
      ```
    - **Requested XML**:
      ```XML
        <status>
            <message>Unexpected server-side error.</message>
            <code>ERR5001</code>
            <details>
                <timestamp>2025-01-16T14:30:00Z</timestamp>
                <support>Contact support@example.com with the timestamp and request details.</support>
            </details>
        </error>
      ```
  - **405 Method Not Allowed**: unsupported HTTP method (e.g., POST instead of GET) is used on your API

    - **Default JSON:** 
      ```json
      {
          "status": "Method Not Allowed",
          "code": "ERR4051",
          "details": {
              "method_used": "POST",
              "allowed_methods": ["GET"],
              "resolution": "Use the correct HTTP method. Refer to the API documentation for supported methods.",
              "documentation": "https://example.com/docs#http-methods"
          },
          "timestamp": "2025-01-16T14:45:00Z"
      }
      ```
    - **Requested XML**:
      ```XML
        <status>
            <message>Method Not Allowed</message>
            <code>ERR4051</code>
            <details>
                <method_used>POST</method_used>
                <allowed_methods>
                    <method>GET</method>
                </allowed_methods>
                <resolution>Use the correct HTTP method. Refer to the API documentation for supported methods.</resolution>
                <documentation>https://example.com/docs#http-methods</documentation>
            </details>
            <timestamp>2025-01-16T14:45:00Z</timestamp>
        </error>
      ```

## 2.2 Data validation


















### Approvals

| Full name           | Occupation               |Aprovals| Date| 
| ---------------     | ------------------------ |--------| --------| 
| Elone DELILLE       | Project manager          | ✅ ❌  |  02//2025  | 
|  Abderrazaq MAKRAN  | Program manager          | ✅ ❌  |  02//2025  | 
| Guillaume DERAMCHI  | Tech lead                | ✅ ❌  |  02//2025  | 
| Benoit DE KEYN      | Software engineer 1      | ✅ ❌  |  02//2025  | 
| Axel DAVID          | Software engineer 2      | ✅ ❌  |  02//2025  | 
| Pierre GORIN        | Quality assurance        | ✅ ❌  |  02//2025  | 
| Tino GABET          | Technical writer         | ✅ ❌  |  02//2025  | 
