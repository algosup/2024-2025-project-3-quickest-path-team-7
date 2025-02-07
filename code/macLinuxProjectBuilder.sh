#!/bin/sh

# Get the script's directory
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PATH_FINDER_DIR="$SCRIPT_DIR/path_finder"
FRONT_DIR="$SCRIPT_DIR/front"

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
if [ "$(uname)" = "Darwin" ]; then
    echo "Compiling main.cpp for macOS..."
    g++ -std=c++17 -O3 -o "$PATH_FINDER_DIR/main" "$PATH_FINDER_DIR/main.cpp"
elif [ "$(uname)" = "Linux" ]; then
    echo "Compiling main.cpp for Linux..."
    g++ -std=c++17 -O3 -o "$PATH_FINDER_DIR/main" "$PATH_FINDER_DIR/main.cpp" -pthread
else
    echo "Unsupported OS. Compilation failed."
    exit 1
fi

if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

echo "Compilation successful. Running main and opening HTML file..."
xdg-open "$FRONT_DIR/index.html" 2>/dev/null || open "$FRONT_DIR/index.html" 2>/dev/null &
"$PATH_FINDER_DIR/main"
