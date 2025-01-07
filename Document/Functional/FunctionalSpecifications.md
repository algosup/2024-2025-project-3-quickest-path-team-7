<div align="center">

# Functional Specifications

**Title:** Quickest Path - TEAM 7

**Author:** Abderrazaq MAKRAN

**Team:** Team 7

**Reviewer:** Pierre GORIN

**Created on:** January 6<sup>th</sup>, 2025

**Last updated:** January 7<sup>th</sup>, 2025

---

## Document history
| Date     | Version | Document Revision Description | Document Author   |
|----------|---------|-------------------------------|-------------------|
|01/06/2025| 0       | Create template               | Abderrazaq MAKRAN | 
---

## Stakeholders
### Project members and Approvals

| Full name           | Occupation               |Aprovals| 
| ---------------     | ------------------------ |--------| 
| Elone DELILLE       | Project manager          | ✅ ❌  | 
|  Abderrazaq MAKRAN  | Program manager          | ✅ ❌  | 
| Guillaume DERAMCHI  | Tech lead                | ✅ ❌  | 
| Benoit DE KEYN      | Software engineer 1      | ✅ ❌  | 
| Axel DAVID          | Software engineer 2      | ✅ ❌  | 
| Pierre GORIN        | Quality assurance        | ✅ ❌  | 
| Tino GABET          | Technical writer         | ✅ ❌  | 

### Other stakeholders

| Name           | Occupation                  | Links                          |
| -------------- | --------------------------- | ------------------------------ |
| Franck JEANNIN | Client (ALGOSUP's director) | [Website](https://algosup.com) |
---

</div>

<br><details>
<summary><h2 id="toc"> Table of Contents <i>(Click to expand)</i></h2></summary>

</details>

# 1. Introduction
## 1.1 Purpose of the document
This document defines the functional specifications of the Quickest Path project, which aims to develop a high-performance software solution in C++ that calculates the fastest path between two landmarks in the USA. The system will expose its functionality via a REST API supporting JSON and XML response formats. The main aim is to deliver fast, accurate results within the constraints specified by the client.

## 1.2 Project Scope
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

## 1.3 Deliverables
[*(Back to top)*](#toc)

The [deliverables](#glossary-deliverable) of this project are:

| Deliverable                               | Link to the Document                                          |
| ----------------------------------------- | -----------------------------------------------------------   |
| Functional Specifications                 | [🔗 Functional Specifications](./Functional-Specifications.md) |
| Technical Specifications                  | [🔗 Technical Specifications](./Technical-Specifications.md)   |
| Test Plan                                 | [🔗 Test Plan](./Test-Plan.md)                                 |
| User Manual                               | [🔗 User Manual](./User-Manual.md)                             |
| Code                                      | [🔗 Code](../../Src/software_team3/software_team3.ino)         |

## 1.4 Project Team
[*(Back to top)*](#toc)

| Role              | Description                                                                                                                                                                | Name                                                                 |
| ----------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- |
| Project Manager   | - In charge of organization, planning, and budgeting.<br>- Ensures communication within the team and keeps them motivated.                                                 | [Elone DELILLE](https://www.linkedin.com/in/elonedelille/)           |
| Program Manager   | - Ensures the project meets the client's expectations.<br>- Acts as the intermediary between the client and the development team.<br>- Responsible for writing the [Functional Specifications](#glossary-fs). | [Abderrazaq MAKRAN](https://www.linkedin.com/in/abderrazaq-makran/)  |
| Tech Lead         | - Makes technical decisions for the project.<br>- Translates the Functional Specifications into [Technical Specifications](#glossary-ts).                                  | [Guillaume DERAMCHI](https://www.linkedin.com/in/guillaume-deramchi/)|
| Technical Writer  | - Writes the documentation for the project.<br>- Based on the specifications, creates a manual for end-users.                                                              | [Tino GABET](https://www.linkedin.com/in/tino-gabet-5794bb32a/)      |
| Software Engineer | - Writes the code and unit tests, comments, and documents the code.<br>- Participates in technical decisions.                                                              | [Benoit DE KEYN](https://www.linkedin.com/in/benoît-de-keyn-71611b293/), [Axel DAVID](https://www.linkedin.com/in/axel-david-6384bb32a/) |
| Quality Assurance | - Tests all product functionalities to find bugs and issues.<br>- Defines the test strategy and writes the [Test Plan](#glossary-tp).<br>- Documents all encountered bugs and ensures their resolution. | [Pierre GORIN](https://www.linkedin.com/in/pierre-gorin-61a784221/)  |

## 1.5 Project Reviewers
[*(Back to top)*](#toc)

External project reviewers have been appointed by the project owner to review our specifications and provide us with feedback.

## 1.6 Milestones
[*(Back to top)*](#toc)

| n°  | Milestone                      | Date       |
| --- | ------------------------------ | ---------- |
| 1   | First Weekly Report            | 01/10/2025 |
| 2   | Functional Specifications (v1) | 01/20/2025 |
| 3   | Second Weekly Report           | 01/17/2025 |
| 4   | Technical Specifications (v1)  | 01/31/2025 |
| 5   | POC (Proof of Concept)         | 01/25/2025 |
| 6   | Third Weekly Report            | 01/25/2025 |
| 7   | MVP (Minimum Viable Product)   | 01/30/2025 |
| 8   | Fourth Weekly Report           | 01/31/2025 |
| 9   | Test Plan                      | 02/03/2025 |
| 10  | User Manual                    | 02/03/2025 |
| 11  | Final Code                     | 02/03/2025 |
| 12  | Fifth Weekly Report            | 02/07/2025 |
| 13  | Final Presentation             | 02/07/2025 |
| 14  | Project Post-Mortem            | 02/13/2025 |

## 1.7 Terms, Acronyms and Definitions 

| **Term/Acronym**           | **Definition**                                                                                   |
|---------------------------- |----------------------------------------------------------------------------------------------- |
| Functional Specifications  | A document outlining the functionalities and requirements of a system or application.          |
| Test Plan                  | A document describing the testing strategy, objectives, resources, and schedule for a project.  |
| Technical Specifications    | A document detailing the technical requirements and architecture of a system or application.    |
| User Manual                | A guide designed to help end-users understand and use the system or application.               |
| Software                   | Programs and operating information used by a computer to perform specific tasks.               |
| C++                        | A high-level programming language used for general-purpose software development.               |
| REST API                   | Representational State Transfer Application Programming Interface – A standard architecture for creating web services. |
| API                        | Application Programming Interface – A set of functions and protocols allowing software applications to communicate with each other. |
| JSON                       | JavaScript Object Notation – A lightweight data-interchange format that is easy for humans to read and machines to parse. |
| XML                        | Extensible Markup Language – A markup language that defines a set of rules for encoding documents in a format that is both human-readable and machine-readable. |
| Heuristic Algorithm        | An approach to problem-solving that employs practical, non-optimal methods to produce solutions quickly. |
| Batch Processing           | A method of running jobs or processing data in batches without manual intervention.            |
| Dataset                    | A collection of related data organized for analysis or computation.                            |
| Bidirectional Connections  | Connections allowing data to flow in both directions between two systems or devices.           |


## 1.8 Requirements, Constraints and Assumptions
### 1.8.1 Requirements:  
- Use of C++ for optimal performance.  
- REST API implementation with support for multi-format outputs.  
- Dataset validation for bidirectional connections and consistency.  

### 1.8.2 Constraints:  
- The project must be completed within the defined timeline.  
- The system must perform on a standard laptop with limited hardware resources.  

---

### 1.8.3 Assumptions  
1. Users will provide valid landmark IDs for queries.  
2. Internet connectivity will be stable for REST API usage.  
3. The provided dataset is accurate and complete.  

---

## 1.9 Risks and Challenges 
### 1.9.1 Risks:  
1. **Dataset Size**: Large datasets may impact performance or memory usage.  
2. **Algorithm Accuracy**: Heuristic methods might occasionally deviate beyond acceptable margins.  
3. **System Load**: High query volumes could strain the system under limited hardware conditions.  

### 1.9.2 Challenges:  
- Balancing speed and accuracy in path calculations.  
- Ensuring scalability for datasets with millions of nodes.  