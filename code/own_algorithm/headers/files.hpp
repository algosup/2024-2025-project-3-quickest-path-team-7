#ifndef FILES_HPP
#define FILES_HPP

#include "header.hpp"

string str_for_char(const string& input, char to_replace, const string& replacement) {
    string result = input;
    size_t pos = result.find(to_replace);
    while (pos != string::npos) {
        result.replace(pos, 1, replacement);
        pos = result.find(to_replace, pos + replacement.length());
    }
    return result;
}

void build_files_path(Files& files) {
    // build the full path for the dataset
    files.dataset.full = files.folder + files.dataset.base;
    // erase ".csv" from the dataset base and build a base for backup files
    string dataset_no_ext = files.dataset.full.substr(0, files.dataset.full.size() - 4);    
    // build the full path for the landmarks backup file
    files.landmarks.full = dataset_no_ext
                         + str_for_char(files.landmarks.base, 'X', to_string(landmarks_qty))
                         + ".bin";
    // for the graph backup file
    files.graph.full = dataset_no_ext + files.graph.base + ".bin";
    // for the output file
    files.output.full = files.folder + files.output.base + ".csv";
    // for the API icon
    files.api_icon.full = files.folder + files.api_icon.base;
}

void takeFolderInput(Files& files) {

    cout << "Working Directory: " << filesystem::current_path() << endl;
    cout << "Do you want to use another specific relative path (n = retry) ? (y/n) ";
    string answer;
    cin >> answer;
    if (answer == "y") {
        cout << "Please provide a relative path : ";
        cin >> files.folder;
        // if not ending with a slash, add it
        files.folder = files.folder.back() == '/' ? files.folder : files.folder + "/";
    } else {
        cout << "Trying again in the current folder ... " << endl;
    }

    build_files_path(files);

}
#endif