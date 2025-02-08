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

    cout << "Copying " << source_file << "\n"
         << "To      " << Files.working_diectory << "/" << Files.sub_folder << " ... " << endl;
    filesystem::path source(source_file);
    filesystem::path destination = filesystem::path(Files.sub_folder) / source.filename();
    
    // check if the source is a valid file
    if (!filesystem::exists(source) || !filesystem::is_regular_file(source)) {
        cout << "ERROR copying : " << source_file << " is not a valid file" << endl;
        return;
    }

    // check if the destination is a CSV file
    if (destination.extension() != ".csv") {
        cout << "ERROR copying : " << destination << " is not a CSV file" << endl;
        return;
    }

    // Copy the file
    try {
        filesystem::copy(source, destination, filesystem::copy_options::overwrite_existing);
        cout << "File copied successfully to " << destination << endl;
    } catch (filesystem::filesystem_error& e) {
        cout << "Error copying file: " << e.what() << endl;
    }
}

void buildFilesPath(Files& Files) {
    Files.working_diectory = filesystem::current_path();
    // build the full path for the dataset
    Files.dataset.full = Files.sub_folder + "/" + Files.dataset.base;
    // erase ".csv" from the dataset base and build a base for backup Files
    string dataset_no_ext = Files.dataset.full.substr(0, Files.dataset.full.size() - 4);    
    // build the full path for the landmarks backup file
    Files.landmarks.full = dataset_no_ext
                         + replaceCharByStr(Files.landmarks.base, 'X', to_string(landmarks_qty))
                         + ".bin";
    // for the graph backup file
    Files.graph.full = dataset_no_ext + Files.graph.base + ".bin";
    // for the output Files
    Files.output.full = Files.sub_folder + "/" + Files.output.base + ".csv";
    Files.comp_output.full = Files.sub_folder + "/" + Files.comp_output.base + ".csv";
    // for the API icon
    Files.api_icon.full = Files.sub_folder + "/" + Files.api_icon.base;
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
                cout << "Current dataset : " << Files.dataset.full << endl;
            } else{ 
                cout << "Dataset " << Files.working_diectory << "/" << Files.dataset.full << " not found !" << endl;
            }
            cout << "Please provide : " << endl;
            cout << " 1 - A new filename within the same location (ex: new_dataset.csv)" << endl;
            cout << " 2 - The path/dataset.csv of a dataset you want to use" << endl;
            if (new_dataset) {
                cout << " 3 - Keep this dataset" << endl;
            } else{ 
                cout << " 3 - Rescan " << Files.working_diectory << "/" << Files.sub_folder << endl;
            }

            // Get the user choice
            cout << "Option : ";
            string option;
            cin >> option;
            try {
                int opt = stoi(option);
                switch (opt) {
                    case 1:
                        cout << "\nEnter the new dataset name: ";
                        cin.ignore();
                        getline(cin, Files.dataset.base);
                        buildFilesPath(Files);
                        cout << endl;
                        break;
                    case 2:
                        cout << "\nEnter the path/dataset.csv of the dataset : ";
                        cin.ignore();
                        getline(cin, full_path);
                        copyFileToSubFolder(Files, full_path);
                        Files.dataset.base = filesystem::path(full_path).filename();
                        buildFilesPath(Files);
                        cout << endl;
                        break;
                    case 3:
                        break;
                    default:
                        cout << "Invalid option\n" << endl;
                        cin.clear();
                        break;
                }

                new_dataset = false; // If the new one works, it will exit, if it doesn't, it will ask again

            } catch (const invalid_argument& e) {
                cout << "Invalid option\n" << endl;
                cin.clear();
            }
        } else {
            csv_found = true;
        }
        test.close();
    }
}
#endif