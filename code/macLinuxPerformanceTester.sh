# This script will compile the main.cpp in a way that it first runs a test on a random sample
# Then a python script is here to display a graphical view of the test results

#!/bin/sh

# Get the script's directory
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PATH_FINDER_DIR="$SCRIPT_DIR/path_finder"
BENCH_DIR="$SCRIPT_DIR/benchmark_sampling"

# Ask the user for landmark quantity and sample size
read -p "Enter the landmark quantity: " LANDMARKS_QUANTITY
read -p "Enter the sample size: " SAMPLE_SIZE


# Check if g++ is installed
if ! command -v g++ >/dev/null 2>&1; then
    echo "g++ compiler not found. Installing..."
    if [ "$(uname)" = "Darwin" ]; then
        brew install gcc
    elif [ "$(uname)" = "Linux" ]; then
        sudo apt update && sudo apt install -y g++
    else
        echo "Unsupported OS. Please install g++ manually."
        exit 1
    fi
fi

# Compile the C++ code
cd "$PATH_FINDER_DIR"
if [ "$(uname)" = "Darwin" ]; then
    echo "Compiling main.cpp for macOS..."
    echo "g++ -std=c++17 -O3 -DTEST -DSPLSIZ=$SAMPLE_SIZE -DLMQTY=$LANDMARKS_QUANTITY -DTOPY -o tester main.cpp"
    g++ -std=c++17 -O3 -DTEST -DSPLSIZ=$SAMPLE_SIZE -DLMQTY=$LANDMARKS_QUANTITY -DTOPY -o tester main.cpp
elif [ "$(uname)" = "Linux" ]; then
    echo "Compiling main.cpp for Linux..."
    echo "g++ -std=c++17 -O3 -DTEST -DSPLSIZ=$SAMPLE_SIZE -DLMQTY=$LANDMARKS_QUANTITY -DTOPY -o tester main.cpp -pthread"
    g++ -std=c++17 -O3 -DTEST -DSPLSIZ=$SAMPLE_SIZE -DLMQTY=$LANDMARKS_QUANTITY -DTOPY -o tester main.cpp -pthread
else
    echo "Unsupported OS. Compilation failed."
    exit 1
fi

if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi


echo "Compilation successful. Running main and opening HTML file..."
cd "$PATH_FINDER_DIR"
./tester

# Move into the benchmark directory
cd "$BENCH_DIR" || { echo "Directory $BENCH_DIR not found!"; exit 1; }

# Define virtual environment path inside the benchmark directory
VENV_DIR="./.venv"

# Check if Python is installed
if ! command -v python3 &> /dev/null; then
    echo "Python3 not found. Installing..."
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS
        if ! command -v brew &> /dev/null; then
            echo "Homebrew not found. Installing Homebrew..."
            /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
        fi
        brew install python
    elif [[ -f "/etc/debian_version" ]]; then
        # Debian/Ubuntu
        sudo apt update && sudo apt install -y python3 python3-venv
    elif [[ -f "/etc/redhat-release" ]]; then
        # Red Hat/Fedora
        sudo dnf install -y python3 python3-venv
    elif [[ -f "/etc/arch-release" ]]; then
        # Arch Linux
        sudo pacman -Sy --noconfirm python python-venv
    else
        echo "Unsupported Linux distribution. Please install Python manually."
        exit 1
    fi
fi

# Create a virtual environment inside the benchmark directory if it doesn't exist
if [ ! -d "$VENV_DIR" ]; then
    echo "Creating a virtual environment in $VENV_DIR..."
    python3 -m venv "$VENV_DIR"
fi

# Activate the virtual environment
echo "Activating the virtual environment..."
source "$VENV_DIR/bin/activate"

# Install required Python packages
echo "Installing required Python packages..."
pip install --upgrade pip
pip install pandas matplotlib

# Run the benchmark script (update the script name if needed)
echo "Running the python script..."
python3 tester.py

# Deactivate the virtual environment
deactivate

echo "Python script executed. A piture was saved here: $BENCH_DIR/test-results.png"
echo "Press any key to exit..."
read -n 1 -s -r -p ""
exit 0
