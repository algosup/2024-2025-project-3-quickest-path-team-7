import pandas as pd
import matplotlib.pyplot as plt

# Define the path to the uploaded CSV file
csv_file_path = "test-results.csv"

print(f"Reading data from {csv_file_path} ... ")

# Extract metadata from the first two rows containing the number of samples and landmarks quantity
with open(csv_file_path, "r") as file:
    lines = file.readlines()
    num_samples = lines[2].split(",")[1].strip()
    landmarks_quantity = lines[3].split(",")[1].strip()
    average_time = lines[4].split(",")[1].strip()
    max_time_str = lines[5].split(",")[1].strip()
    print ("average response time: ", average_time)

# Read the CSV file while skipping metadata rows and handling column names properly
df = pd.read_csv(csv_file_path, skiprows=6)

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

mem_size = [ # Memory size in bytes for each landmark quantity  
    None,  # Index 0 is unused to align indices with the provided data
    0.88, 0.97, 1.06, 1.15, 1.24, 1.32, 1.41, 1.50, 1.59, 1.68,
    1.77, 1.86, 1.95, 2.04, 2.13, 2.21, 2.30, 2.39, 2.48, 2.57,
    2.66, 2.75, 2.84, 2.93, 3.02, 3.10, 3.19, 3.28, 3.37, 3.46,
    3.55, 3.64, 3.73, 3.82, 3.91, 3.99, 4.08, 4.17, 4.26, 4.35,
    4.44, 4.53, 4.62, 4.71, 4.80, 4.88, 4.97, 5.06, 5.15, 5.24,
    5.33, 5.42, 5.51, 5.60, 5.69, 5.77, 5.86, 5.95, 6.04, 6.13,
    6.22, 6.31, 6.40, 6.49, 6.58, 6.66, 6.75, 6.84, 6.93, 7.02,
    7.11, 7.20, 7.29, 7.38, 7.47, 7.55, 7.64, 7.73, 7.82, 7.91,
    8.00
]

print("Plotting results ... ")
# Plot results
plt.figure(figsize=(14, 8))  # Adjust the width and height of the plot

# Scatter plot for measured execution times with adjusted dot size
plt.scatter(nodes_visited, execution_times, label="Measured Times", color="blue", alpha=0.6, s=10)  # Adjust 's' for dot size

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
plt.title(f"Team 7 algorithm tested using {landmarks_quantity} landmarks, with a sample of {num_samples} random paths\n\n\n\n", fontsize=14)
# sub title : text("Memory size : {mem_size[int(landmarks_quantity)]} GB\nAverage Execution Time: {average_time}")
# showing under the title please
plt.text(0, 1.15, f"Memory size : {mem_size[int(landmarks_quantity)]} GB\nAverage Execution Time: {average_time}\nMaxium Execution Time: {max_time_str}", fontsize=10, ha='left')
# Adjust y-axis limits to reach 1.1 in height
plt.ylim(top=1.1)

# Use linear-linear scale for better visualization of trends
plt.xscale("linear")
plt.yscale("linear")

# Add gridlines
plt.grid(True, which="both", linestyle="--", linewidth=0.5)

# resize window
plt.tight_layout()

# Save the plot as an image file high quality
plt.savefig("test-results.png", dpi=300)

# Show the plot
plt.show()

exit # Exit the script after plotting the results