#The script is quite simple. It first checks if the  g++  compiler is installed. 
#If not, it installs it using  brew  on macOS or  apt  on Linux. 
#Then, it compiles the  main.cpp  file using  g++  and runs the compiled executable. 
#Finally, it opens the  index.html  file in the default browser. 
#To run the script, open a terminal, navigate to the  code  directory, and run the following command: 
#sh macLinuxProjectBuilder.sh
#
#If you are on macOS, you should see the following output: 
#Compiling main.cpp for macOS...
#
#!/bin/sh
#
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
cd "$PATH_FINDER_DIR"
if [ "$(uname)" = "Darwin" ]; then
    echo "Compiling main.cpp for macOS..."
    echo "g++ -std=c++17 -O3 -o main main.cpp"
    g++ -std=c++17 -O3 -o main main.cpp
elif [ "$(uname)" = "Linux" ]; then
    echo "Compiling main.cpp for Linux..."
    echo "g++ -std=c++17 -O3 -o main main.cpp -pthread"
    g++ -std=c++17 -O3 -o main main.cpp -pthread
else
    echo "Unsupported OS. Compilation failed."
    exit 1
fi

if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

echo "Compilation successful. Running main and opening HTML file..."
xdg-open "$FRONT_DIR/index.html" 2>/dev/null || open "$FRONT_DIR/index.html" 2>/dev/null
cd "$PATH_FINDER_DIR"
./main