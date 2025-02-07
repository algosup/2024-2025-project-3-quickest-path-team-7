#ifndef Files_HPP
#define Files_HPP

#include "header.hpp"

string strForChar(const string& input, char to_replace, const string& replacement) {
    string result = input;
    size_t pos = result.find(to_replace);
    while (pos != string::npos) {
        result.replace(pos, 1, replacement);
        pos = result.find(to_replace, pos + replacement.length());
    }
    return result;
}

void buildFilesPath(Files& Files) {
    // build the full path for the dataset
    Files.dataset.full = Files.folder + Files.dataset.base;
    // erase ".csv" from the dataset base and build a base for backup Files
    string dataset_no_ext = Files.dataset.full.substr(0, Files.dataset.full.size() - 4);    
    // build the full path for the landmarks backup file
    Files.landmarks.full = dataset_no_ext
                         + strForChar(Files.landmarks.base, 'X', to_string(landmarks_qty))
                         + ".bin";
    // for the graph backup file
    Files.graph.full = dataset_no_ext + Files.graph.base + ".bin";
    // for the output Files
    Files.output.full = Files.folder + Files.output.base + ".csv";
    Files.comp_output.full = Files.folder + Files.comp_output.base + ".csv";
    // for the API icon
    Files.api_icon.full = Files.folder + Files.api_icon.base;
}

void takeFolderInput(Files& Files) {

    cout << "Working Directory: " << filesystem::current_path() << endl;
    cout << "Do you want to use another specific relative path (n = retry) ? (y/n) ";
    string answer;
    cin >> answer;
    if (answer == "y") {
        cout << "Please provide a relative path : ";
        cin >> Files.folder;
        // if not ending with a slash, add it
        Files.folder = Files.folder.back() == '/' ? Files.folder : Files.folder + "/";
    } else {
        cout << "Trying again in the current folder ... " << endl;
    }

    buildFilesPath(Files);

}

void requireDataset(Files& Files) {
    // Try to open the dataset
    bool csv_found = false;
    while(!csv_found) {
        ifstream test(Files.dataset.full);
        if (!test.is_open()) {
            cout << "Dataset " << Files.dataset.full << " not found !" << endl;
            cout << "Please provide : " << endl;
            cout << " 1 - A new dataset file" << endl;
            cout << " 2 - The correct path to the dataset" << endl;
            cout << " 3 - Retry" << endl;
            cout << "Option : ";
            int option;
            cin >> option;
            switch (option) {
                case 1:
                    cout << "Enter the new dataset name: ";
                    cin.ignore();
                    getline(cin, Files.dataset.base);
                    buildFilesPath(Files);
                    break;
                case 2:
                    takeFolderInput(Files);
                    break;
                case 3:
                    break;
                default:
                    cout << "Invalid option" << endl;
                    break;
            }
        } else {
            csv_found = true;
        }
        test.close();
    }
}
#endif