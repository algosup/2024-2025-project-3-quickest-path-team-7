#ifndef INPUT_HPP
#define INPUT_HPP

#include "header.hpp"

void takeFolderInput(Files& files) {

    if (ASK_FOLDER){
        cout << "Do you want to change the folder for input/output files ? (y/n)" << flush;
        char c = 'x';
        while (c!='y' && c!='n') {
            cin >> c;
            if (c == 'y') {
                cout << "\nPlease provide the folder path: ";
                cin >> files.folder_path;
                files.dataset = files.folder_path + "/" + DATASET;
                files.output = files.folder_path + "/" + OUTPUT;
                files.map_backup = files.folder_path + "/" + MAP_BACKUP;
                files.landmarks_backup = files.folder_path + "/" + LANDMARKS_BACKUP;
            } else {
                if (c == 'n') {
                    files.folder_path = "";
                    files.dataset = DATASET;
                    files.output = OUTPUT;
                    files.map_backup = MAP_BACKUP;
                    files.landmarks_backup = LANDMARKS_BACKUP;
                } else {
                    cout << "Invalid input. Please try again." << endl;
                }
            }
        }
    } else {
        files.folder_path = "";
        files.dataset = DATASET;
        files.output = OUTPUT;
        files.map_backup = MAP_BACKUP;
        files.landmarks_backup = LANDMARKS_BACKUP;
    }
    
    cout << "\nThe program will read this dataset: " << files.dataset << endl;
    cout << "The program will save the output to this file: " << files.output << endl;
    cout << "The program will save a backup of the graph to these files: " << files.map_backup << " and " << files.landmarks_backup << endl;
}

#endif