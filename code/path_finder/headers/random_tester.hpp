#ifndef TESTER
#define TESTER

#include "header.hpp"

// input in microseconds, output like "12.57 ms" keep 2 digits only
string microsecToString(long time) {
    ostringstream oss;
    if (time < 1000) {
        oss << time << " µs";
    } else if (time < 1000000) {
        oss << fixed << setprecision(2) << time / 1000.0 << " ms";
    } else {
        oss << fixed << setprecision(2) << time / 1000000.0 << " s";
    }
    return oss.str();
}

void random_tester(Graph& Graph, Astar& Astar, Path& Path, Files& Files, int sample_size) {

    int average_delay = 0;
    int max_delay = 0;
    int heaviest_quantity = sample_size / 100;
    if (heaviest_quantity < 10) heaviest_quantity = 10;

    // for the test results
    string base_test_results = TEST_RESULTS;
    // replace the X by the number of samples
    base_test_results = replaceCharByStr(base_test_results, 'X', to_string(sample_size));

    // Reneme the file (1).csv, (2).csv and so on if already existing
    int i = 1;
    string test_results = Files.sub_folder + "/" + base_test_results + ".csv";
    while (filesystem::exists(test_results)) {
        test_results = Files.sub_folder + "/" + base_test_results + "(" + to_string(i) + ").csv";
        i++;
    }

    ofstream results(test_results);
    if (!results.is_open()) {
        println("Failed to open the file for writing: " + test_results, type::ERROR_BOLD);
        return;
    }

    results << "Date and time : " << getCurrentTimestamp() << endl;
    results << "Results of random picking test ordered by calculation time" << endl;
    results << "Number of tested paths, " << sample_size << endl;
    results << "Landmarks qantity, " << landmarks_qty << endl;
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

    println("Starting the random test with " + to_string(sample_size) + " paths sample, with " + to_string(landmarks_qty) + " landmarks");

    print("Calculating random paths ... 0 %", type::INFO);

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
    results << "Average calculation time, " << microsecToString(average_delay) << endl;
    results << "Maximum calculation time, " << microsecToString(max_delay) << endl;
    results << "\nStart, End, Distance, Calculation time (" << TIME_UNIT_STR << ")" << endl;
    while (!paths.empty()) {
        current_path = paths.top();
        paths.pop();
        results << current_path.start << ", " << current_path.end << ", " << current_path.nodes_qty << ", " << current_path.calculation_time << endl;
    }
    results.close();
    
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
    println("Random test completed !", type::VALIDATION);
    print("The average calculation time is ");
    println(microsecToString(average_delay), type::WARNING);
    print("The maximum calculation time is ");
    println(microsecToString(max_delay), type::WARNING);
    println("Results saved in file://" + filesystem::absolute(test_results).string(), type::INFO);

    //copy the csv as test_results.csv, overwriting the one before
    filesystem::path source(test_results);
    filesystem::path destination(Files.sub_folder + "/test-results.csv");
    try {
        filesystem::copy(source, destination, filesystem::copy_options::overwrite_existing);
    } catch (filesystem::filesystem_error& e) {
        println("ERROR copying : " + string(e.what()), type::ERROR_BOLD);
    }

    // then copy this test_results.csv to "../benchmark_sampling/test_results.csv" if it exists
    filesystem::path benchmark_folder("../benchmark_sampling");
    if (!filesystem::exists(benchmark_folder)) {
        return;
    }

    destination = benchmark_folder / "test-results.csv";
    try {
        filesystem::copy(source, destination, filesystem::copy_options::overwrite_existing);
    } catch (filesystem::filesystem_error& e) {
        println("ERROR copying : " + string(e.what()), type::ERROR_BOLD);
    }

    println(" Now you can go to " + filesystem::absolute(benchmark_folder).string() + " to plot the results with python", type::INFO);
    
}

#endif