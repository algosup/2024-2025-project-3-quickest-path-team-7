<div align="center">

# Test Strategy 

---
**Title:** Quickest Path - Test Strategy

**Team:** Team 7

**Author:** Pierre GORIN

**Version:** 1.8

---

</div>

<br><details>
<summary><h2 id="toc"> Table of Contents <i>(Click to expand)</i></h2></summary>

- [Test Strategy](#test-strategy)
- [1. - Introduction](#1---introduction)
  - [1.1 - Overview](#11---overview)
  - [1.2 - Scope](#12---scope)
    - [1.2.1 - Constraints](#121---constraints)
    - [1.2.2 - Assumptions](#122---assumptions)
    - [1.2.3 - In Scope](#123---in-scope)
    - [1.2.4 - Out of Scope](#124---out-of-scope)
  - [1.3 - Objectives](#13---objectives)
  - [1.4 - Executive Summary](#14---executive-summary)
- [2. - Test Strategy](#2---test-strategy)
  - [2.1 - Test Approach](#21---test-approach)
    - [2.1.1 - Test Coverage](#211---test-coverage)
    - [2.1.2 - Test Automation](#212---test-automation)
    - [2.1.3 - Regression Testing](#213---regression-testing)
  - [2.2 - Test Types](#22---test-types)
    - [2.2.1 - Unit Testing](#221---unit-testing)
    - [2.2.2 - Integration Testing](#222---integration-testing)
    - [2.2.3 - Functional Testing](#223---functional-testing)
    - [2.2.4 - Performance Testing](#224---performance-testing)
    - [2.2.5 - Smoke Testing](#225---smoke-testing)
  - [2.3 - Test Tools](#23---test-tools)
    - [2.3.1 - Postman](#231---postman)
    - [2.3.2 - Google Test](#232---google-test)
    - [2.3.3 - GitHub Actions](#233---github-actions)
    - [2.3.4 - Custom Scripts](#234---custom-scripts)
  - [2.4 - Test Environment](#24---test-environment)
    - [2.4.1 - Development Environment](#241---development-environment)
    - [2.4.2 - Testing Environment](#242---testing-environment)
    - [2.4.3 - Staging Environment](#243---staging-environment)
  - [2.5 - Risks and Mitigation](#25---risks-and-mitigation)
- [3. - Test Plan](#3---test-plan)
  - [3.1 - Test Schedule](#31---test-schedule)
  - [3.2 - Responsibilities and Roles](#32---responsibilities-and-roles)
  - [3.3 - Test Cases](#33---test-cases)
  - [3.4 - Metrics and Reporting](#34---metrics-and-reporting)
- [4. - Deliverables](#4---deliverables)
  - [4.1 - Test Artifacts](#41---test-artifacts)
  - [4.2 - Final Test Report](#42---final-test-report)
- [5. - Dependencies](#5---dependencies)
  - [5.1 - Tools and Frameworks](#51---tools-and-frameworks)
  - [5.2 - Third-Party Services](#52---third-party-services)
- [6. - Appendix](#6---appendix)
  - [6.1 - Acronyms and Abbreviations](#61---acronyms-and-abbreviations)
  - [6.2 - References](#62---references)
  - [6.3 - Document History](#63---document-history)
  - [6.4 - Document Approval](#64---document-approval)

</details>

# 1. - Introduction

## 1.1 - Overview

This document defines the test strategy for the Quickest Path project, which aims to develop a software solution in C++ that calculates the fastest path between two landmarks. The system will expose its functionality via a REST API supporting JSON and XML response formats. The main goal is to deliver fast, accurate results within the constraints specified by the client.

## 1.2 - Scope

### 1.2.1 - Constraints

The project is subject to the following constraints:
- **Time:** The project must be completed within six weeks.
- **Performance:** The system must respond to requests within 1000 milliseconds.
- **Budget:** No budget is allocated for this project.
- **Resources:** The team consists of 1 project manager, 1 program manager, 1 technical lead, 2 developers, 1 QA engineer, and 1 technical writer.
- **Technology:** The system must be developed in C++.
- **Accuracy:** The system must provide the quickest path within a 10% margin of error.

### 1.2.2 - Assumptions

We assume that the client's requirements are well-defined and will not change significantly during development, the system will run locally on the developer's machines, no third-party services are required for the core functionality.

### 1.2.3 - In Scope

The following testing activities are in scope for this project:
- **Unit Testing:** Verifying individual components and functions.
- **Integration Testing:** Ensuring that different modules work together.
- **Functional Testing:** Validating the system against its requirements.
- **Performance Testing:** Evaluating the system's responsiveness and speed.
- **Smoke Testing:** Quickly checking critical functionalities after a build.
- **Regression Testing:** Ensuring that new changes do not break existing functionality.

### 1.2.4 - Out of Scope

The following testing activities are out of scope for this project:
- **Security Testing:** Evaluating the system's security vulnerabilities.
- **Usability Testing:** Assessing the system's user-friendliness.
- **Compatibility Testing:** Verifying the system's compatibility with different devices and browsers.
- **Load Testing:** Testing the system's performance under high loads.
- **Stress Testing:** Evaluating the system's stability under extreme conditions.
- **Acceptance Testing:** Validating the system against user requirements.

## 1.3 - Objectives

The main objectives of the test strategy are to ensure the system meets the client's requirements. The key objectives include:
- **Accuracy:** Verify that the system calculates the quickest path within the specified margin of error.
- **Performance:** Ensure that the system responds to requests within the specified time limit.
- **Reliability:** Validate that the system is stable and does not crash under normal conditions.
- **Usability:** Verify that the system is easy to use and understand for end-users.

## 1.4 - Executive Summary

The test strategy outlines the approach, objectives, and scope of the testing process for the Quickest Path project. It defines the test types, tools, and environments to be used, as well as the responsibilities and roles of the team members. The document also details the test schedule, metrics, and reporting mechanisms to be employed. The test strategy aims to ensure that the system meets the client's requirements and delivers accurate, reliable, and performant results.

# 2. - Test Strategy

## 2.1 - Test Approach

### 2.1.1 - Test Coverage

The tests will cover the following parts of the system:
- **API Endpoints:** Verify that the API endpoints return the correct results.
- **Algorithms:** Validate that the algorithms used to calculate the quickest path are accurate.
- **Error Handling:** Ensure that the system handles errors gracefully and provides informative error messages.
- **Performance:** Evaluate the system's performance under normal and peak loads.

### 2.1.2 - Test Automation

The tests will be automated using Google Test, a C++ testing framework that allows developers to write and run unit tests. The automation will cover unit tests, integration tests, and functional tests to ensure that the system behaves as expected. The automated tests will be run as part of the CI/CD pipeline using GitHub Actions to ensure that the code changes do not introduce regressions.

### 2.1.3 - Regression Testing

Regression testing will be performed after each time a new feature is added or a bug is fixed. The goal is to ensure that the changes do not negatively impact existing functionality. The regression tests will be automated and run as part of the CI/CD pipeline to catch any issues early in the development process.

## 2.2 - Test Types

### 2.2.1 - Unit Testing

Unit testing is the process of testing individual components or modules of the application in isolation. The goal is to verify that each unit of code works correctly on its own. Unit tests will be written for each function and class in the system to ensure that they behave as expected. Google Test will be used to write and run the unit tests.

Here is an example of a unit test for the `takeUserInput` function:

```cpp
TEST(TakeUserInputTest, Test1) {
    PathData path_data;
    Graph graph;
    Files files;
    bool break_early = false;
    EXPECT_EQ(takeUserInput(path_data, graph, files, break_early), -1);
}
```

If the `takeUserInput` function returns `-1`, the test will pass, otherwise, it will fail.

### 2.2.2 - Integration Testing

Integration testing is the process of testing how different components or systems work together. The goal is to ensure that the interactions between components are correct and that the system behaves as expected as a whole. Integration tests will be written to test the interactions between the API endpoints, algorithms, and error handling mechanisms.

Here is an example of an integration test for the `dijkstra_algorithm` function:

```cpp
TEST(DijkstraAlgorithmTest, Test1) {
    PathData path_data;
    Graph graph;
    bool break_early = false;
    dijkstra_algorithm(graph, path_data, break_early);
    EXPECT_EQ(path_data.distance[0], 0);
    EXPECT_EQ(path_data.prev[0], -1);
}
```

The test verifies that the `dijkstra_algorithm` function correctly calculates the shortest path distance and previous node for a given graph.

### 2.2.3 - Functional Testing

Functional testing is the process of testing the system against its functional requirements. The goal is to ensure that the system behaves as expected and meets the client's requirements. Functional tests will be written to test the API endpoints, algorithms, error handling, and performance of the system.

Here is an example of a functional test for the `/calculate` API endpoint:

```cpp
TEST(CalculateEndpointTest, Test1) {
    PathData path_data;
    Graph graph;
    Files files;
    bool break_early = false;
    EXPECT_EQ(calculateEndpoint(path_data, graph, files, break_early), -1);
}
```

The test verifies that the `/calculate` endpoint returns `-1` when called with invalid input. Otherways, it will fail.

### 2.2.4 - Performance Testing

Performance testing is the process of evaluating the system's responsiveness, speed, and stability under load. The goal is to ensure that the system can handle the expected number of requests within the specified time limit. Performance tests will be written to test the system's response time under normal and peak loads.

Here is an example of a performance test for the system's response time:

```cpp
TEST(PerformanceTest, Test1) {
    PathData path_data;
    Graph graph;
    Files files;
    bool break_early = false;
    EXPECT_LT(calculatePerformance(path_data, graph, files, break_early), 1000);
}
```

The test verifies that the system's response time is less than 1000 milliseconds. If the test fails, it indicates that the system is not meeting the performance requirements.

### 2.2.5 - Smoke Testing

Smoke testing is the process of quickly checking if the critical functionalities of the application are working after a build. The goal is to ensure that the system is stable and that no major issues have been introduced. Smoke tests will be written to test the core functionalities of the system, such as the API endpoints and algorithms.

Here is an example of a smoke test for the system's API endpoints:

```cpp
TEST(SmokeTest, Test1) {
    PathData path_data;
    Graph graph;
    Files files;
    bool break_early = false;
    EXPECT_EQ(smokeTest(path_data, graph, files, break_early), 0);
}
```

The test verifies that the system's API endpoints are working correctly. If the test fails, it indicates that there is a critical issue with the system.

## 2.3 - Test Tools

We will use some tools to facilitate and automate the testing process.

### 2.3.1 - Postman

Postman is a popular tool for testing APIs by making HTTP requests and validating responses. It provides a user-friendly interface for creating and running tests, as well as generating reports. Postman will be used to test the system's API endpoints and ensure that they return the correct results.

See more about Postman [here](https://www.postman.com/).

### 2.3.2 - Google Test

Google Test is a C++ testing framework that allows developers to write and run unit tests. It provides a rich set of assertions and utilities for testing C++ code. Google Test will be used to write and run the unit tests for the system to ensure that each component works correctly.

Here are some software that uses Google Test:
- Android Open Source Project operating system
- Chromium projects (behind the Chrome browser and ChromeOS)
- LLVM compiler
- Protocol Buffers (Google's data interchange format)
- OpenCV computer vision library
- Robot Operating System
- Gromacs molecular dynamics simulation package

See more about Google Test [here](https://google.github.io/googletest/) and [here](https://github.com/google/googletest).

### 2.3.3 - GitHub Actions

GitHub Actions is a feature of GitHub that automates workflows, including testing, during CI/CD pipelines. It allows developers to define custom workflows using YAML files and run them on GitHub's servers. GitHub Actions will be used to automate the testing process for the system, including running the unit tests, integration tests, and functional tests.

See more about GitHub Actions [here](https://github.com/features/actions)

### 2.3.4 - Custom Scripts

If needed, custom scripts will be developed to handle specific testing scenarios or automate repetitive tasks. These scripts will be written in Python or Bash and will be used to perform tasks such as setting up the test environment, generating test data, or running performance tests.

## 2.4 - Test Environment

The testing activities will take place in the following environments.

### 2.4.1 - Development Environment

The development environment is used by developers to build and test code locally. It consists of the following components:
- **Operating System:** Windows 11 or MacOS Sequoia 15.2
- **IDE:** Visual Studio Code
- **Compiler:** GCC 11.2 or Clang 14.0
- **Version Control:** Git

### 2.4.2 - Testing Environment

The testing environment is a dedicated setup for running and validating test cases. It consists of the following components:
- **Operating System:** Ubuntu 24.04 LTS
- **API Testing:** Postman
- **Unit Testing:** Google Test
- **CI/CD:** GitHub Actions
- **Custom Scripts:** Python 3.13.1

### 2.4.3 - Staging Environment

No staging environment is required for this project as the system will be deployed locally on the developer's machines.
But if needed, the staging environment will closely replicate the production setup for final testing.

## 2.5 - Risks and Mitigation

The following risks have been identified for the project and strategies to mitigate them:
- **Performance Issues:** To mitigate performance issues, performance tests will be run regularly to identify bottlenecks and optimize the system.
- **Algorithm Errors:** To mitigate algorithm errors, extensive unit and integration tests will be written to verify the correctness of the algorithms.
- **API Changes:** To mitigate API changes, the API endpoints will be thoroughly tested to ensure that they return the correct results and handle errors gracefully.
- **Resource Constraints:** To mitigate resource constraints, the team will be regularly updated on the project's progress and any issues that arise.
- **Time Constraints:** To mitigate time constraints, the team will prioritize testing activities based on their impact on the project's success.
- **Budget Constraints:** To mitigate budget constraints, the team will use open-source tools and frameworks to minimize costs.
- **Scope Creep:** To mitigate scope creep, the team will regularly review the project requirements and adjust the testing strategy as needed.

# 3. - Test Plan

## 3.1 - Test Schedule

No schedule is defined for this project as the testing activities will be performed continuously throughout the development process. But the testing activities will be completed within the six-week timeframe allocated for the project.

## 3.2 - Responsibilities and Roles

The following responsibilities and roles have been assigned for the testing process:
- **Program Manager:** Oversees the testing process and ensures that the system meets the client's requirements.
- **Project Manager:** Manages the testing activities and coordinates with the development team.
- **Technical Lead:** Provides technical guidance and support to the testing team.
- **Software Engineers:** Write and run the unit tests, integration tests, and functional tests.
- **Quality Assurance Engineer:** Ensures that the system meets the quality standards and performs the regression tests.
- **Technical Writer:** Documents the testing process and reports the results.

## 3.3 - Test Cases

The test cases are available in the [Test Cases](TestCases.md) document.

## 3.4 - Metrics and Reporting

The following metrics will be used to measure the success of the testing process:
- **Test Coverage:** The percentage of the system that is covered by the tests.
- **Defect Density:** The number of defects found per unit of code.
- **Test Execution Time:** The time taken to run the tests.
- **Test Pass Rate:** The percentage of tests that pass.
- **Defect Resolution Time:** The time taken to fix defects.
- **Test Automation Rate:** The percentage of tests that are automated.
- **Performance Metrics:** The system's response time, throughput, and error rate.

# 4. - Deliverables

## 4.1 - Test Artifacts

The following test artifacts will be delivered at the end of the testing process:
- **Test Strategy:** This document outlines the approach, objectives, and scope of the testing process.
- **Test Plan:** The document that details the schedule, responsibilities, and steps involved in the testing process.
- **Test Cases:** The document that describes the conditions and steps for each test.
- **Test Reports:** The reports that summarize the testing results and provide recommendations.
- **Github Issues Templates:** The templates that will be used to report issues found during testing.

## 4.2 - Final Test Report

At the end of the testing process, a final test report will be written to summarize the testing results and provide recommendations for further improvements.

# 5. - Dependencies

## 5.1 - Tools and Frameworks

The following tools and frameworks are required to execute the tests:
- **Google Test:** A C++ testing framework for writing and running unit tests.
- **Postman:** A tool for testing APIs by making HTTP requests and validating responses.
- **GitHub Actions:** A feature of GitHub that automates workflows, including testing, during CI/CD pipelines.

## 5.2 - Third-Party Services

No third-party services are required for this project as the system will be developed and tested locally on the developer's machines.

# 6. - Appendix

## 6.1 - Acronyms and Abbreviations

| Acronym | Definition                                     |
| ------- | ---------------------------------------------- |
| API     | Application Programming Interface              |
| CI/CD   | Continuous Integration/Continuous Deployment   |
| REST    | Representational State Transfer                |
| JSON    | JavaScript Object Notation                     |
| XML     | Extensible Markup Language                     |
| YAML    | Yet Another Markup Language or YAML Ain't Markup Language |
| IDE     | Integrated Development Environment             |
| OS      | Operating System                               |
| GCC     | GNU Compiler Collection                        |
| GNU     | GNU's Not Unix                                 |
| LTS     | Long-Term Support                              |
| QA      | Quality Assurance                              |

## 6.2 - References

- [Google Test](https://google.github.io/googletest/)
- [Postman](https://www.postman.com/)
- [GitHub Actions]([https://github.com/features/actions)
- [C++](https://en.wikipedia.org/wiki/C%2B%2B)
- [REST API](https://en.wikipedia.org/wiki/Representational_state_transfer)
- [JSON](https://en.wikipedia.org/wiki/JSON)
- [XML](https://en.wikipedia.org/wiki/XML)

## 6.3 - Document History
| Version | Date       | Author       | Changes Made                                   |
| ------- | ---------- | ------------ | ---------------------------------------------- |
| 1.0     | 2025-01-10 | Pierre GORIN | Document Created                               |
| 1.1     | 2025-01-13 | Pierre GORIN | Writing Skeleton                               |
| 1.2     | 2025-01-14 | Pierre GORIN | Beginning Intro. Sections(Overview, etc.)      |
| 1.3     | 2025-01-14 | Pierre GORIN | Finishing Intro. Sections                      |
| 1.4     | 2025-01-20 | Pierre GORIN | Beginning Deliverables & Dependencies Sections |
| 1.5     | 2025-01-20 | Pierre GORIN | Beginning Deliverables & Dependencies Sections |
| 1.6     | 2025-01-21 | Pierre GORIN | Beginning Test Strategy Sections               |
| 1.7     | 2025-01-22 | Pierre GORIN | Beginning Test Plan Sections                   |
| 1.8     | 2025-01-23 | Pierre GORIN | First Draft Completed                          |

## 6.4 - Document Approval

| Reviewer           | Role              | Approved | Date |
| ------------------ | ----------------- | -------- | ---- |
| Abderrazaq MAKRAN  | Program Manager   | ⏳        |      |
| Elone DELILLE      | Project Manager   | ⏳        |      |
| Guillaume DERAMCHI | Technical Lead    | ⏳        |      |
| Pierre GORIN       | Quality Assurance | ⏳        |      |
| Benoît DE KEYN     | Software Engineer | ⏳        |      |
| Axel DAVID         | Software Engineer | ⏳        |      |
| Tino GABET         | Technical Writer  | ⏳        |      |