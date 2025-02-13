// main.js

// Define boundaries: minimum 1, maximum 23,947,347
const INT32_MIN = 1;
const INT32_MAX = 23947347;

// DOM references
const pathForm = document.getElementById("pathForm");
const startInput = document.getElementById("start");
const endInput = document.getElementById("end");
const submitBtn = document.getElementById("submitBtn");
const errorMessage = document.getElementById("errorMessage");
const loadingSpinner = document.getElementById("loadingSpinner");
const requestInfo = document.getElementById("requestInfo");
const overviewDisplay = document.getElementById("overviewDisplay");
const detailsDisplay = document.getElementById("detailsDisplay");
const requestTypeInput = document.getElementById("requestType");
const requestFormatInput = document.getElementById("requestFormat");
const requestResponseOutput = document.getElementById("requestResponse");
const outputContainer = document.getElementsByClassName("output-container")[0];
const outputResponse = document.getElementsByClassName("request-response")[0];

/**
 * Check if the input is a valid integer within [1..23,947,347].
 */
function isValid32BitInteger(value) {
  if (!/^\d+$/.test(value.trim())) {
    return false;
  }
  const num = Number(value);
  return num >= INT32_MIN && num <= INT32_MAX;
}

/**
 * Display an error message and reset results.
 */
function displayError(message) {
  errorMessage.textContent = message;

  errorMessage.classList.remove("hidden");
  // Hide request info
  requestInfo.classList.add("hidden");
  // Hide the output content
  outputContainer.classList.remove("hidden");
  outputResponse.classList.add("hidden");

  // Optionally, clear out the overview/details sections
  overviewDisplay.innerHTML = "";
  detailsDisplay.innerHTML = "";
}

/**
 * Clear any existing error messages.
 */
function clearError() {
  errorMessage.textContent = "";
}

/**
 * Utility function: format microseconds into a string:
 * - If < 1000 => "XYZµs (0.000XYZs)"
 * - If < 1,000,000 => "XYZms (0.00XYZs)"
 * - Otherwise => "XYZs"
 */
function formatTimeMicroseconds(value) {
  const micro = parseInt(value, 10) || 0;
  if (micro < 1000) {
    // under 1000 µs => XXµs
    const seconds = (micro / 1_000_000).toFixed(6);
    return `${micro}µs (${seconds}s)`;
  } else if (micro < 1_000_000) {
    // between 1,000 and 999,999 µs => XX.XXXms
    const ms = (micro / 1000).toFixed(3);
    const seconds = (micro / 1_000_000).toFixed(6);
    return `${ms}ms (${seconds}s)`;
  } else {
    // 1,000,000 µs and above => seconds
    const s = (micro / 1_000_000).toFixed(6);
    return `${s}s`;
  }
}

/**
 * Returns an object with `overview` and `details` strings
 * for a Basic Path:
 * data = { path_length, path: string[] }
 */
function buildBasicPathHTML(data) {
  const length = data.path_length || "N/A";
  const nodesCount = Array.isArray(data.path) ? data.path.length : 0;

  const overview = `
    <div class="path-length"><span class="label">Path length:</span> ${length}</div>
    <div class="nodes-length"><span class="label">Path nodes:</span> ${nodesCount}</div>
    <br>
  `;

  let details = `
    <details class="accordion">
      <summary class="accordion-title">Path Nodes (${nodesCount})</summary>
      <div class="accordion-content">
  `;
  if (Array.isArray(data.path)) {
    data.path.forEach((node) => {
      details += `<div class="path-node">↳ ${node}</div>`;
    });
  }
  details += `
      </div>
    </details>
  `;

  return { overview, details };
}

/**
 * Returns an object with `overview` and `details` strings
 * for a Detailed Path:
 * data = {
 *   dataset, start, end, path_length, time_unit, calculation_time, ...
 *   landmarks: string[],
 *   path: [{id, weight}...]
 * }
 */
function buildDetailedPathHTML(data) {
  const length = data.path_length || "N/A";
  const nodeCount =
    data.nodes_quantity || (Array.isArray(data.path) ? data.path.length : 0);

  let calcTime = "";
  if (data.calculation_time !== undefined) {
    calcTime = formatTimeMicroseconds(data.calculation_time);
  }

  let overview = `
    <div class="path-length"><span class="label">Path length:</span> ${length}</div>
    <div class="nodes-length"><span class="label">Path nodes:</span> ${nodeCount}</div>
    <br>
  `;
  if (calcTime) {
    overview += `<div class="time"><span class="label">Calculation time:</span> ${calcTime}</div>`;
  }
  if (data.heuristic_weight) {
    overview += `<div class="heuristic-weight"><span class="label">Heuristic weight:</span> ${data.heuristic_weight}</div>`;
  }
  if (data.landmarks_quantity) {
    overview += `<div class="landmarks-quantity"><span class="label">Landmarks quantity:</span> ${data.landmarks_quantity}</div>`;
  }

  let details = "";
  // Landmarks accordion
  if (Array.isArray(data.landmarks) && data.landmarks.length > 0) {
    details += `
      <details class="accordion">
        <summary class="accordion-title">Landmarks (${data.landmarks.length})</summary>
        <div class="accordion-content">
    `;
    data.landmarks.forEach((lm) => {
      details += `<div class="landmark">${lm}</div>`;
    });
    details += `
        </div>
      </details>
    `;
  }

  // Path accordion
  details += `
    <details class="accordion">
      <summary class="accordion-title">Path Nodes (${nodeCount})</summary>
      <div class="accordion-content">
  `;
  if (Array.isArray(data.path)) {
    data.path.forEach((obj) => {
      details += `<div class="path-node">↳ ${obj.id} (${obj.weight})</div>`;
    });
  }
  details += `
      </div>
    </details>
  `;

  return { overview, details };
}

/**
 * Returns an object with `overview` and `details` strings
 * for a Compare Path:
 * data = {
 *   oversize_percentage, dijkstra_path_length, astar_path_length,
 *   time_unit, astar_calculation_time, dijkstra_calculation_time,
 *   astar_nodes_quantity, dijkstra_nodes_quantity, ...
 *   compared_path: [
 *     { astar: {id, weight}, dijkstra: {id, weight} }, ...
 *   ]
 * }
 */
function buildComparePathHTML(data) {
  const stepsCount = Array.isArray(data.compared_path)
    ? data.compared_path.length
    : 0;
  const astarTime = data.astar_calculation_time
    ? formatTimeMicroseconds(data.astar_calculation_time)
    : "";
  const dijkstraTime = data.dijkstra_calculation_time
    ? formatTimeMicroseconds(data.dijkstra_calculation_time)
    : "";

  const displayedPathLength =
    data.astar_path_length || data.dijkstra_path_length || "N/A";
  const displayedNodeCount =
    data.astar_nodes_quantity || data.dijkstra_nodes_quantity || "N/A";

  let overview = `
    <div class="path-length"><span class="label">Path length: ${displayedPathLength}</div>
    <div class="nodes-length"><span class="label">Path nodes: ${displayedNodeCount}</div>
    <br>
  `;
  if (astarTime) {
    overview += `<div class="time"><span class="label">A* calculation time:</span> ${astarTime}</div>`;
  }
  if (dijkstraTime) {
    overview += `<div class="time"><span class="label">Dijkstra calculation time:</span> ${dijkstraTime}</div>`;
  }
  if (data.heuristic_weight) {
    overview += `<div class="heuristic-weight"><span class="label">Heuristic weight:</span> ${data.heuristic_weight}</div>`;
  }
  if (data.landmarks_quantity) {
    overview += `<div class="landmarks-quantity"><span class="label">Landmarks quantity:</span> ${data.landmarks_quantity}</div>`;
  }

  let details = `
  <details class="accordion" style="overflow-y: hidden;">
    <summary class="accordion-title">Compared Path Nodes (${stepsCount})</summary>
    <div class="accordion-content">
      <table>
        <thead>
          <tr>
            <th rowspan="2">Step</th>
            <th colspan="2">A*</th>
            <th colspan="2">Dijkstra</th>
          </tr>
          <tr>
            <th>Id</th>
            <th>Weight</th>
            <th>Id</th>
            <th>Weight</th>
          </tr>
        </thead>
        <tbody>
`;

  if (Array.isArray(data.compared_path)) {
    data.compared_path.forEach((step, index) => {
      const stepNumber = index + 1;
      // Check if the IDs match
      const sameId = step.astar.id === step.dijkstra.id;
      // Pick the appropriate CSS class
      const rowClass = sameId ? "row-match" : "row-mismatch";

      details += `
          <tr class="${rowClass}">
            <td>${stepNumber}</td>
            <td>${step.astar.id}</td>
            <td>${step.astar.weight}</td>
            <td>${step.dijkstra.id}</td>
            <td>${step.dijkstra.weight}</td>
          </tr>
    `;
    });
  }

  details += `
        </tbody>
      </table>
    </div>
  </details>
`;

  return { overview, details };
}

/**
 * Determine whether data is Basic, Detailed or Compare,
 * then build separate `overview` and `details` strings.
 * Return: { overview, details }
 */
function formatPathResult(data) {
  // Compare path
  if (Array.isArray(data.compared_path)) {
    return buildComparePathHTML(data);
  }
  // Detailed path: data.path is an array of objects
  if (
    Array.isArray(data.path) &&
    data.path.length > 0 &&
    typeof data.path[0] === "object"
  ) {
    return buildDetailedPathHTML(data);
  }
  // Basic path: data.path is an array of strings
  if (
    Array.isArray(data.path) &&
    data.path.length > 0 &&
    typeof data.path[0] === "string"
  ) {
    return buildBasicPathHTML(data);
  }
  // Otherwise, unknown
  return {
    overview: "<div>Unknown or invalid data structure</div>",
    details: "",
  };
}

/**
 * Show the loading spinner and disable form interactions.
 */
function startLoading() {
  loadingSpinner.style.display = "block";
  loadingSpinner.setAttribute("aria-hidden", "false");
  submitBtn.disabled = true;
}

/**
 * Hide the loading spinner and enable form interactions.
 */
function stopLoading() {
  loadingSpinner.style.display = "none";
  loadingSpinner.setAttribute("aria-hidden", "true");
  submitBtn.disabled = false;
}

/**
 * Get the selected request type (basic, detailed, compare).
 */
function getSelectedRequestType() {
  return requestTypeInput.value;
}

/**
 * Get the selected request format (json, xml).
 */
function getSelectedRequestFormat() {
  return requestFormatInput.value;
}

/**
 * Handle the form submission:
 */
async function handleSubmit(event) {
  event.preventDefault();

  clearError();
  // Clear old content
  overviewDisplay.innerHTML = "";
  detailsDisplay.innerHTML = "";

  const startValue = startInput.value.trim();
  const endValue = endInput.value.trim();
  const requestType = getSelectedRequestType();
  const requestFormat = getSelectedRequestFormat();

  // Validate inputs
  if (!isValid32BitInteger(startValue)) {
    displayError(
      "Please enter a valid integer from 1 to 23,947,347 for the Start value."
    );
    return;
  }
  if (!isValid32BitInteger(endValue)) {
    displayError(
      "Please enter a valid integer from 1 to 23,947,347 for the End value."
    );
    return;
  }

  // Determine the endpoint based on the request type
  let endpoint;
  switch (requestType) {
    case "detailed":
      endpoint = "debug_path";
      break;
    case "compare":
      endpoint = "comp_path";
      break;
    case "basic":
    default:
      endpoint = "path";
      break;
  }

  // Construct the API URL
  const url = `http://localhost:9500/${endpoint}?start=${encodeURIComponent(
    startValue
  )}&end=${encodeURIComponent(endValue)}&format=json`;

  try {
    startLoading();

    const response = await fetch(url);
    if (!response.ok) {
      throw new Error(`API request failed with status ${response.status}`);
    }

    // Show the request info
    requestInfo.textContent = `> GET /${endpoint}?start=${startValue}&end=${endValue}&format=${requestFormat}`;
    requestInfo.classList.remove("hidden");
    errorMessage.classList.add("hidden");

    // Parse JSON
    const data = await response.json();

    // Generate separate HTML chunks for overview & details
    const { overview, details } = formatPathResult(data);

    // Inject them into the new containers
    overviewDisplay.innerHTML = overview;
    detailsDisplay.innerHTML = details;
    
    // if the response format is JSON, display the raw JSON response
    // else if the response format is XML, redo the fetch request with XML format
    if (requestFormat === "json") {
      requestResponseOutput.textContent = JSON.stringify(data, null, 2);
    } else if (requestFormat === "xml") {
      // redo the fetch request with XML format after replacing the format=json with format=xml in the URL and wait 10ms before fetching
      const xmlUrl = url.replace("format=json", "format=xml");
      await new Promise((resolve) => setTimeout(resolve, 50));
      const xmlResponse = await fetch(xmlUrl);
      if (!xmlResponse.ok) {
        throw new Error(`API request failed with status ${xmlResponse.status}`);
      }
      const xmlData = await xmlResponse.text();
      requestResponseOutput.textContent = xmlData;
    }

    errorMessage.classList.add("hidden");
    outputContainer.classList.remove("hidden");
    outputResponse.classList.remove("hidden");
  } catch (error) {
    displayError(`Error: ${error.message}`);
  } finally {
    stopLoading();
  }
}

// Attach event listener
pathForm.addEventListener("submit", handleSubmit);
