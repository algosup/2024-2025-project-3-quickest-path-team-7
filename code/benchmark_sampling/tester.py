import pandas as pd
import matplotlib.pyplot as plt

# Define the path to the uploaded CSV file
csv_file_path = "test-results.csv"


# Extract metadata from the first two rows containing the number of samples and landmarks quantity
with open(csv_file_path, "r") as file:
    lines = file.readlines()
    num_samples = lines[2].split(",")[1].strip()
    landmarks_quantity = lines[3].split(",")[1].strip()

# Read the CSV file while skipping metadata rows and handling column names properly
df = pd.read_csv(csv_file_path, skiprows=4)

# Strip column names to remove extra spaces
df.columns = df.columns.str.strip()

# Extract relevant columns
start_nodes = df["Start"]
end_nodes = df["End"]
nodes_visited = df["Distance"]
execution_times = df["Calculation time (µs)"] / 1e6  # Convert microseconds to seconds

# Find max execution time and corresponding path
max_time = execution_times.max()
max_time_index = execution_times.idxmax()
max_time_path = (start_nodes[max_time_index], end_nodes[max_time_index], nodes_visited[max_time_index], max_time)

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
    xy=(min(nodes_visited), 1),  # Position the annotation at y=1 and minimum x value
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
plt.title(f"A* Algorithm using {landmarks_quantity} landmarks tested with {num_samples} random samples through API")

# Adjust y-axis limits to reach 1.1 in height
plt.ylim(top=1.1)

# Use linear-linear scale for better visualization of trends
plt.xscale("linear")
plt.yscale("linear")

# Add gridlines
plt.grid(True, which="both", linestyle="--", linewidth=0.5)

# Save the plot as an image file high quality
plt.savefig("test-results.png", dpi=300)

# Show the plot
plt.tight_layout()
plt.show()

exit # Exit the script after plotting the results