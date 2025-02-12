#ifndef Files_HPP
#define Files_HPP

#include "header.hpp"

string replaceCharByStr(const string& input, char to_replace, const string& replacement) {
    string result = input;
    size_t pos = result.find(to_replace);
    while (pos != string::npos) {
        result.replace(pos, 1, replacement);
        pos = result.find(to_replace, pos + replacement.length());
    }
    return result;
}

void copyFileToSubFolder(Files& Files, const string& source_file) {

    // If not existing, create the subfolder
    if (!Files.sub_folder.empty() && !filesystem::exists(Files.sub_folder)) {
        filesystem::create_directory(Files.sub_folder);
    }

    println("Copying " + source_file + "\nTo      " + Files.working_directory + "/" + Files.sub_folder + " ... ");

    filesystem::path source(source_file);
    filesystem::path destination = filesystem::path(Files.sub_folder) / source.filename();
    
    // check if the source is a valid file
    if (!filesystem::exists(source) || !filesystem::is_regular_file(source)) {
        println("ERROR copying : " + source_file + " is not a valid file", type::ERROR_BOLD);
        return;
    }

    // check if the destination is a CSV file
    if (destination.extension() != ".csv") {
        println("ERROR copying : " + destination.string() + " is not a CSV file", type::ERROR_BOLD);
        return;
    }

    // Copy the file
    try {
        filesystem::copy(source, destination, filesystem::copy_options::overwrite_existing);
        println("File copied successfully to " + destination.string());
    } catch (filesystem::filesystem_error& e) {
        println("ERROR copying : " + string(e.what()), type::ERROR_BOLD);
    }
}

void buildFilesPath(Files& Files) {
    Files.working_directory = filesystem::current_path().string();
    string separator = Files.sub_folder.empty() ? "" : "/";
    // build the full path for the dataset
    Files.dataset.full = Files.sub_folder + separator + Files.dataset.base;
    // erase ".csv" from the dataset base and build a base for backup Files
    string dataset_no_ext = Files.dataset.full.substr(0, Files.dataset.full.size() - 4);    
    // build the full path for the landmarks backup file
    Files.landmarks.full = dataset_no_ext
                         + replaceCharByStr(Files.landmarks.base, 'X', to_string(landmarks_qty))
                         + ".bin";
    // for the graph backup file
    Files.graph.full = dataset_no_ext + Files.graph.base + ".bin";
    // for the output Files
    Files.output.full = Files.sub_folder + separator + Files.output.base + ".csv";
    Files.comp_output.full = Files.sub_folder + separator + Files.comp_output.base + ".csv";
    // for the API icon
    Files.api_icon.full = Files.sub_folder + separator + Files.api_icon.base;
}

void newLocation(Files& Files) {
    string path;

    println("Working directory : " + Files.working_directory);
    println("Sub folder for input and output files : /" + Files.sub_folder);
    println("Please choose an option : ");
    println(" 1 - Change the sub_folder for files");
    println(" 2 - Change the working_directory/sub_folder");
    println(" 3 - Change only the working_directory and keep the sub_folder");
    println(" 4 - Keep the current location");
    print("Option : ");

    string option;
    cin >> option;
    try {
        int opt = stoi(option);
        switch (opt) {
            case 1:
                print("\nPlease provide the relative path (just press enter to remove it) : ");
                cin.ignore();
                getline(cin, Files.sub_folder);

                // if ending with a slash, remove it
                Files.sub_folder = Files.sub_folder.back() == '/' ? Files.sub_folder.substr(0, Files.sub_folder.size() - 1) : Files.sub_folder;
                // if beginning with a slash, remove it
                Files.sub_folder = Files.sub_folder.front() == '/' ? Files.sub_folder.substr(1, Files.sub_folder.size() - 1) : Files.sub_folder;
                // if beginning with a dot, remove it
                Files.sub_folder = Files.sub_folder.front() == '.' ? Files.sub_folder.substr(1, Files.sub_folder.size() - 1) : Files.sub_folder;

                buildFilesPath(Files);
                if (Files.sub_folder.empty()) {
                    println("\nFiles are now handled in the working directory");
                } else {
                    println("\nFiles path changed to " + Files.working_directory + "/" + Files.sub_folder);
                }
                break;
            case 2:
                print("\nPlease provide the absolute path : ");
                cin.ignore();
                getline(cin, path);
                filesystem::current_path(path);
                Files.working_directory = filesystem::current_path().string();
                // if ending with a slash, remove it
                Files.sub_folder = "";
                buildFilesPath(Files);
                println("\nFiles path changed to " + Files.working_directory + "/" + Files.sub_folder);
                break;
            case 3:
                print("\nPlease provide the absolute path : ");
                cin.ignore();
                getline(cin, path);
                filesystem::current_path(path);
                // if ending with a slash, remove it
                Files.working_directory = filesystem::current_path().string();
                buildFilesPath(Files);
                println("\nWorking directory changed to " + Files.working_directory);
                break;
            case 4:
                break;
            default:
                println("Invalid option");
                cin.clear();
                break;
        }
    } catch (const invalid_argument& e) {
        println("Invalid option");
        cin.clear();
    }
}

void requireDataset(Files& Files, bool new_dataset = false) {
    string full_path;

    // Try to open the dataset
    bool csv_found = false;
    while(!csv_found) {
        ifstream test(Files.dataset.full);
        if (!test.is_open() || new_dataset) {

            // Display the different options
            if (new_dataset) {
                println("\nCurrent dataset : " + Files.dataset.full);
            } else{ 
                println("\nDataset " + Files.working_directory + "/" + Files.dataset.full + " not found !", type::WARNING);
            }
            println("Please choose an option : ");
            println(" 1 - A new filename within the same location (ex: new_dataset.csv)");
            println(" 2 - The path/dataset.csv of a dataset you want to use");
            println(" 3 - Change the files location");
            if (new_dataset) {
                println(" 4 - Keep this dataset");
            } else{ 
                println(" 4 - Rescan " + Files.working_directory + "/" + Files.sub_folder);
            }

            // Get the user choice
            print("Option : ");
            string option;
            cin >> option;
            try {
                int opt = stoi(option);
                switch (opt) {
                    case 1:
                        print("\nEnter the new dataset name: ");
                        cin.ignore();
                        getline(cin, Files.dataset.base);
                        buildFilesPath(Files);
                        println("");
                        break;
                    case 2:
                        print("\nEnter the path/dataset.csv of the dataset : ");
                        cin.ignore();
                        getline(cin, full_path);
                        copyFileToSubFolder(Files, full_path);
                        Files.dataset.base = filesystem::path(full_path).filename().string(); // Convert to string
                        buildFilesPath(Files);
                        println("");
                        break;
                    case 3:
                        println("");
                        newLocation(Files);
                        break;
                    case 4:
                        break;
                    default:
                        println("Invalid option\n");
                        cin.clear();
                        break;
                }

                new_dataset = false; // If the new one works, it will exit, if it doesn't, it will ask again

            } catch (const invalid_argument& e) {
                println("Invalid option\n");
                cin.clear();
            }
        } else {
            csv_found = true;
        }
        test.close();
    }
}
#endif