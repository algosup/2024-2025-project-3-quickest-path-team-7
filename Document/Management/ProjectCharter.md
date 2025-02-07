# Project Charter  

## **Project Title and Identification**  
**Title**: Quickest Path  
**Project**: 3  
**Date**: January 2025  
**Owner**: ALGOSUP  

---

## **Project Purpose**  
The purpose of the Quickest Path project is to develop a high-performance software solution in C++ that calculates the quickest path between two landmarks in the United States. The system will expose its functionality through a REST API supporting JSON and XML response formats. The primary focus is on providing fast and accurate results within the constraints specified by the client.

---

## **Objectives and Success Criteria**  
### Objectives:  
1. Develop a high-performance route calculation engine using heuristic algorithms.  
2. Provide API functionality to support JSON and XML response formats.  
3. Ensure the system can handle datasets with up to 24 million nodes.  
4. Deliver results with a response time of under 1 second.  

### Success Criteria:  
- Accurate path results, deviating by no more than 10% from the shortest possible route.  
- System must be operational and accessible for at least 99.9% of the time
- Scalability to handle high query volumes without performance degradation.  

---

## **Scope Overview**  
| **Feature**                              | **Description**                                                                                 | **In Scope** | **Out of Scope** |
|------------------------------------------|-------------------------------------------------------------------------------------------------|--------------|------------------|
| **[Shortest Path Calculation](#spc)**    | The system must calculate the shortest path between two landmarks using heuristic algorithms.   | ✅            |                  |
| **[REST API](#rest-api)**                | The system must expose functionality via a REST API supporting JSON and XML response formats.   | ✅            |                  |
| **[Batch Processing](#batch)**           | The system must support batch route calculations for multiple source-destination pairs.         | ✅            |                  |
| **[Data Validation](#data-validation)**  | The system must validate the integrity and connectivity of the input dataset.                  | ✅            |                  |
| **[Heuristic Optimization](#heuristic)** | The system must use heuristics to maintain performance while staying within the 10% error margin. | ✅            |                  |
| **[Multi-Format Support](#formats)**     | The API must provide outputs in both JSON and XML formats.                                      | ✅            |                  |
| **[Real-Time Responses](#real-time)**    | The system must deliver responses within 1 second for a standard laptop setup.                 | ✅            |                  |
| **[User Authentication](#authentication)** | The system must authenticate users accessing the API.                                          |              | ❌               |
| **[Offline Functionality](#offline)**    | The system must function offline without internet connectivity.                                |              | ❌               |
| **[Graphical Interface](#gui)**          | The system must provide a graphical user interface for user interaction.                       |              | ❌               |
| **[Voice Input](#voice-input)**          | The system must support voice input for navigation requests.                                   |              | ❌               |

---

## **Requirements and Constraints**  
### Requirements:  
- Use of C++ for optimal performance.  
- REST API implementation with support for multi-format outputs.  
- Dataset validation for bidirectional connections and consistency.  

### Constraints:  
- The project must be completed within the defined timeline.  
- The system must perform on a standard laptop with limited hardware resources.  

---

## **Assumptions**  
1. Users will provide valid landmark IDs for queries.  
2. Internet connectivity will be stable for REST API usage.  
3. The provided dataset is accurate and complete.  

---

## **Risks and Challenges**  
### Risks:  
1. **Dataset Size**: Large datasets may impact performance or memory usage.  
2. **Algorithm Accuracy**: Heuristic methods might occasionally deviate beyond acceptable margins.  
3. **System Load**: High query volumes could strain the system under limited hardware conditions.  

### Challenges:  
- Balancing speed and accuracy in path calculations.  
- Ensuring scalability for datasets with millions of nodes.  

---

## **Timeline**  
| **Milestone**                  | **Start Date**  | **End Date**    | **Deliverable**                                      |
|--------------------------------|-----------------|-----------------|-----------------------------------------------------|
| **Project Kickoff**            | January 6, 2025 | January 10, 2025| Charter Approval                                   |
| **Requirement Analysis**       | January 6, 2025| January 20, 2025| Functional Specification Document                  |
| **Development Start**          | January 6, 2025 | February 7, 2025 | Initial API Implementation                         |
| **System Testing**             | January 20, 2025   | February 7, 2025     | Validation Against Dataset                         |
| **Final Release**              | February 7, 2025     | February 7, 2025    | Fully Functional Quickest Path System              |

---

