import requests
import time
import random
import matplotlib.pyplot as plt
import numpy as np

# Configuration
API_URL = "http://localhost:9500/path"
NUM_SAMPLES = 10000  # Number of queries to execute
MAX_NODES = 23947347  # Maximum node ID for random testing
V = 23947347  # Number of nodes in the graph
E = 28000000 # Number of edges in the graph
# Storage for results
nodes_visited = []
execution_times = []
max_time_path = (None, None, None, 0)  # (start, end, path_length, exec_time)

MAX_RETRIES = 3
DELAY_BETWEEN_REQUESTS = 0.01  # Half a second delay

def get_path(start, end):
    """Send a request to the API and retry if the connection fails."""
    url = f"{API_URL}?start={start}&end={end}"

    for attempt in range(MAX_RETRIES):
        try:
            response = requests.get(url, timeout=10)  # Ensure a timeout is set
            response.raise_for_status()  # Raise error for bad responses (4xx, 5xx)
            data = response.json()
            
            # Return the path length and execution time
            return int(data["path_length"]), response.elapsed.total_seconds()

        except requests.exceptions.ConnectionError as e:
            print(f"⚠️ Connection failed: {e} (Retry {attempt + 1}/{MAX_RETRIES})")
            print("-> Node IDs:", start, end)
            time.sleep(1)  # Wait before retrying

        except requests.exceptions.RequestException as e:
            print(f"❌ Request failed: {e}")
            return None, None  # Return None on fatal errors

    print("🚨 Max retries reached. Skipping request.")
    return None, None  # Return None if all retries fail


# Perform the tests
for i in range(NUM_SAMPLES):
    start = random.randint(1, MAX_NODES)
    end = random.randint(1, MAX_NODES)

    if start == end:
        continue  # Avoid self-paths

    time.sleep(DELAY_BETWEEN_REQUESTS)  # Add delay to avoid API overloading
    path_length, exec_time = get_path(start, end)
    if path_length is not None:
        nodes_visited.append(path_length)
        execution_times.append(exec_time)
        
        # Update max_time_path if current path took more time
        if exec_time > max_time_path[3]:
            max_time_path = (start, end, path_length, exec_time)
    
    # Print progress
    progress = (i + 1) / NUM_SAMPLES * 100
    print(f"\rProgress: {progress:.2f}%", end="")

print()  # Move to the next line after the loop

# Convert lists to NumPy arrays
nodes_visited = np.array(nodes_visited)
execution_times = np.array(execution_times)

# Sort the data to ensure the trend line looks smooth
sorted_indices = np.argsort(nodes_visited)
nodes_visited_sorted = nodes_visited[sorted_indices]
execution_times_sorted = execution_times[sorted_indices]

# Maximum execution time horizontal line
max_time = np.max(execution_times)

# Plot results
plt.figure(figsize=(10, 6))

# Scatter plot for measured execution times
plt.scatter(nodes_visited, execution_times, label="Measured Times", color="blue", alpha=0.6)

# Plot the maximum execution time as a horizontal line
plt.axhline(y=max_time, color="red", linewidth=2, label=f"Maximum time recorded")

# Limit line in green for 1 second
plt.axhline(y=1, color="green", linewidth=2, label="1 second threshold")

# Annotate the 1-second threshold line
plt.annotate(
    "1 second threshold",
    xy=(0, 1),  # Position the annotation at y=1
    xytext=(10, 10),
    textcoords='offset points',
    arrowprops=dict(arrowstyle="->", color='green'),
    bbox=dict(boxstyle="round,pad=0.3", edgecolor='green', facecolor='white')
)

# Display the path with the maximum execution time
plt.annotate(
    f"Max Time Path\nStart: {max_time_path[0]}\nEnd: {max_time_path[1]}\nLength: {max_time_path[2]}\nTime: {max_time_path[3]:.4f}s",
    xy=(max_time_path[2], max_time_path[3]),
    xytext=(10, 10),
    textcoords='offset points',
    arrowprops=dict(arrowstyle="->", color='red'),
    bbox=dict(boxstyle="round,pad=0.3", edgecolor='red', facecolor='white')
)

# Labels, title, and legend
plt.xlabel("Path Length (Number of Nodes in Path)")
plt.ylabel("Execution Time (Seconds)")
plt.title("A* Algorithm Complexity Analysis for " + str(NUM_SAMPLES) + " random samples through API")

# Use linear-linear scale for better visualization of trends
plt.xscale("linear")
plt.yscale("linear")

# Add gridlines
plt.grid(True, which="both", linestyle="--", linewidth=0.5)

# Show the plot
plt.tight_layout()
plt.show()