<div align="center">

# Functional Specifications

**Title:** Quickest Path - TEAM 7

**Author:** Abderrazaq MAKRAN

**Team:** Team 7

**Reviewer:** Pierre GORIN

**Created on:** January 6<sup>th</sup>, 2025

**Last updated:** <sup>th</sup>, 2025

---

| Date     | Version | Document Revision Description | Document Author   |
|----------|---------|-------------------------------|-------------------|
|01/06/2025| 0       | Create template               | Abderrazaq MAKRAN | 
---

## Stakeholders
### Project members and Approvals

| Full name           | Occupation               |Aprovals| Links                                                             |
| ---------------     | ------------------------ |--------| ----------------------------------------------------------------- |
| Elone DELILLE       | Project manager          | ✅ ❌ | [LinkedIn](https://www.linkedin.com/in/elonedelille/)             |
|  Abderrazaq MAKRAN  | Program manager          | ✅ ❌ | [LinkedIn](https://www.linkedin.com/in/abderrazaq-makran/)        |
| Guillaume DERAMCHI  | Tech lead                | ✅ ❌ | [LinkedIn](https://www.linkedin.com/in/guillaume-deramchi/)       |
| Benoit DE KEYN      | Senior software engineer | ✅ ❌ | [LinkedIn](https://www.linkedin.com/in/benoît-de-keyn-71611b293/) |
| Axel DAVID          | Junior software engineer | ✅ ❌ | [LinkedIn](https://www.linkedin.com/in/axel-david-6384bb32a/)     |
| Pierre GORIN        | Quality assurance        | ✅ ❌ | [LinkedIn](https://www.linkedin.com/in/pierre-gorin-61a784221/)   |
| Tino GABET          | Technical writer         | ✅ ❌ | [LinkedIn](https://www.linkedin.com/in/tino-gabet-5794bb32a/)     |

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
This document defines the functional specifications for the **Quickest Path System**, which calculates the optimal travel path between two landmarks in the U.S. and exposes the functionality via a REST API.

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

