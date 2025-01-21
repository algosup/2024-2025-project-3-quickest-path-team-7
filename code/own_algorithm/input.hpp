#ifndef INPUT_HPP
#define INPUT_HPP

#include "header.hpp"

void takeFolderInput(Files& files, bool ask_folder) {

    if (ask_folder){
        cout << "Do you want to change the folder for input/output files ? (y/n)" << flush;
        char c = 'x';
        while (c!='y' && c!='n') {
            cin >> c;
            if (c == 'y') {
                cout << "\nPlease provide the folder path: ";
                cin >> files.folder_path;
                files.dataset = files.folder_path + "/" + DATASET;
                files.output = files.folder_path + "/" + OUTPUT;
                files.map_backup = files.folder_path + "/" + BACKUP;
            } else {
                if (c == 'n') {
                    files.folder_path = "";
                    files.dataset = DATASET;
                    files.output = OUTPUT;
                    files.map_backup = BACKUP;
                } else {
                    cout << "Invalid input. Please try again." << endl;
                }
            }
        }
    } else {
        files.folder_path = "";
        files.dataset = DATASET;
        files.output = OUTPUT;
        files.map_backup = BACKUP;
    }
    
    cout << "\nThe program will read this dataset: " << files.dataset << endl;
    cout << "The program will save the output to this file: " << files.output << endl;
    cout << "The program will save a backup of the graph to this file: " << files.map_backup << endl;
}

#endif