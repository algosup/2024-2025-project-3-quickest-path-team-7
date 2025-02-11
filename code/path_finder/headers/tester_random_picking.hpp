#ifndef TESTER
#define TESTER

#include "header.hpp"

// input in microseconds, output like "12.57 ms" keep 2 digits only
string microsecToString(long time) {
    if (time < 1000) {
        return to_string(time) + " µs";
    } else if (time < 1000000) {
        return to_string(time / 1000) + "." + to_string(time % 1000 / 100) + " ms";
    } else {
        return to_string(time / 1000000) + "." + to_string(time % 1000000 / 100000) + " s";
    }
}

void random_tester(Graph& Graph, Astar& Astar, Path& Path, Files& Files, int sample_size) {

    int average_delay = 0;
    int max_delay = 0;
    int heaviest_quantity = sample_size / 100;
    if (heaviest_quantity < 10) heaviest_quantity = 10;

    // for the test results
    string test_results = TEST_RESULTS;
    // replace the X by the number of samples
    test_results = replaceCharByStr(test_results, 'X', to_string(sample_size));
    test_results = Files.sub_folder + "/" + test_results + ".csv";

    ofstream results(test_results);
    if (!results.is_open()) {
        println("Failed to open the file for writing: " + test_results, type::ERROR);
        return;
    }

    results << "Results of random picking test ordered by calculation time" << endl;
    results << "Sample size, " << sample_size << " paths" << endl;
    results << "Landmarks qantity, " << landmarks_qty << endl;
    results << "\nStart, End, Distance, Calculation time (" << TIME_UNIT_STR << ")" << endl;
    results.close();

    Timer timer;

    struct PathKpi {
        long calculation_time;
        int nodes_qty;
        int start;
        int end;
        bool operator<(const PathKpi& other) const { // sort by delay from lowest to highest so the 
            return calculation_time < other.calculation_time;
        }
    };
    priority_queue<PathKpi> paths;

    PathKpi current_path;

    unsigned int counter = 0;
    unsigned int progression = 0;
    unsigned int progression_backup = 0;

    println("Starting the random test with " + to_string(sample_size) + " samples with " + to_string(landmarks_qty) + " landmarks");


    for (int i = 0; i < sample_size; ++i) {

        resetComputeData(Graph, Path, Astar);

        Path.start = rand() % Graph.nodes_qty + 1;
        Path.end = rand() % Graph.nodes_qty + 1;

        findPath(Graph, Path, Astar, timer);

        current_path.calculation_time = Path.calculation_time;
        current_path.nodes_qty = Path.path.size();
        current_path.start = Path.start;
        current_path.end = Path.end;
        paths.push(current_path);

        average_delay += Path.calculation_time;
        max_delay = max(max_delay, Path.calculation_time);

        // Show progression and count lines
        counter++;
        progression = counter * 100 / sample_size;
        if (progression != progression_backup) {
            print("\rCalculating random paths ... " + to_string(progression) + " %", type::INFO);
            progression_backup = progression;
        }
    }
    println("\rCalculating random paths ... 100 %", type::VALIDATION);

    average_delay /= sample_size;

    // Priority queue of the heaviest paths , copied from the complete one
    priority_queue<PathKpi> heaviest_paths = paths;

    // Fill the file
    results.open(test_results, ios::app);
    while (!paths.empty()) {
        current_path = paths.top();
        paths.pop();
        results << current_path.start << ", " << current_path.end << ", " << current_path.nodes_qty << ", " << current_path.calculation_time << endl;
    }
    results.close();

    println("Random test completed with an average delay of " + microsecToString(average_delay));
    
    println("The heaviest paths are in order : ");
    for (int i = 0; i < heaviest_quantity; ++i) {
        current_path = heaviest_paths.top();
        heaviest_paths.pop();
        print(" - Execution time : ");
        print(microsecToString(current_path.calculation_time), type::WARNING);
        print(" for the path from ");
        print(current_path.start, type::WARNING);
        print(" to ");
        print(current_path.end, type::WARNING);
        print(" with ");
        print(current_path.nodes_qty);
        println(" nodes");
    }

    println("");
    print("The maximum delay is ");
    print(max_delay, type::WARNING);
    print(" ");
    println(TIME_UNIT_STR, type::WARNING);
    println("Results saved in file://" + filesystem::absolute(test_results).string(), type::INFO);
}

#endif