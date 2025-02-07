// main.js

// Define boundaries: minimum 1, maximum 23,947,347
const INT32_MIN = 1;
const INT32_MAX = 23947347;

// DOM references
const pathForm = document.getElementById('pathForm');
const startInput = document.getElementById('start');
const endInput = document.getElementById('end');
const submitBtn = document.getElementById('submitBtn');
const errorMessage = document.getElementById('errorMessage');
const resultContent = document.getElementById('resultContent');
const loadingSpinner = document.getElementById('loadingSpinner');
const requestInfo = document.getElementById('requestInfo');
const resultDisplay = document.getElementById('resultDisplay');

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
  resultContent.textContent = '';
  // Hide the request info and result containers on error
  requestInfo.classList.add('hidden');
  resultDisplay.classList.add('hidden');
}

/**
 * Clear any existing error messages.
 */
function clearError() {
  errorMessage.textContent = '';
}

/**
 * Format JSON object to a readable string.
 */
function formatJSON(jsonObj) {
  return JSON.stringify(jsonObj, null, 2);
}

/**
 * Show the loading spinner and disable form interactions.
 */
function startLoading() {
  loadingSpinner.style.display = 'block';
  loadingSpinner.setAttribute('aria-hidden', 'false');
  submitBtn.disabled = true;
}

/**
 * Hide the loading spinner and enable form interactions.
 */
function stopLoading() {
  loadingSpinner.style.display = 'none';
  loadingSpinner.setAttribute('aria-hidden', 'true');
  submitBtn.disabled = false;
}

/**
 * Handle the form submission:
 */
async function handleSubmit(event) {
  event.preventDefault();

  clearError();
  resultContent.textContent = '';

  const startValue = startInput.value.trim();
  const endValue = endInput.value.trim();

  // Validate inputs
  if (!isValid32BitInteger(startValue)) {
    displayError("Please enter a valid integer from 1 to 23,947,347 for the Start value.");
    return;
  }
  if (!isValid32BitInteger(endValue)) {
    displayError("Please enter a valid integer from 1 to 23,947,347 for the End value.");
    return;
  }

  // Construct the API URL
  const url = `http://77.90.39.156/path?start=${encodeURIComponent(startValue)}&end=${encodeURIComponent(endValue)}&format=json`;

  try {
    startLoading();

    const response = await fetch(url);
    if (!response.ok) {
      throw new Error(`API request failed with status ${response.status}`);
    }

    // Show the request info + results on success only
    requestInfo.textContent = `GET /path?start=${startValue}&end=${endValue}`;
    requestInfo.classList.remove('hidden');
    resultDisplay.classList.remove('hidden');

    const data = await response.json();
    resultContent.textContent = formatJSON(data);
  } catch (error) {
    displayError(`Error: ${error.message}`);
  } finally {
    stopLoading();
  }
}

// Attach event listener
pathForm.addEventListener('submit', handleSubmit);