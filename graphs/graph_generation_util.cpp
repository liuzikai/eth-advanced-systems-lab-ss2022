#include <string>
#include <fstream>
#include <iostream>

#include "graph_generation_util.h"

bool file_exists (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

// A method that takes as argument a file name and unzips the file using gzcat if it wasn't unziped yet.
// The method returns the file name of the unzipped file.
std::string unzip_file(std::string file_name) {
    if (file_name.find(".gz") != std::string::npos) {
        std::string unzipped_file_name = file_name.substr(0, file_name.find(".gz"));
        if (!file_exists(unzipped_file_name)) {
            std::cout << file_name << " is unzipping..." << std::endl;
            std::string command = "gzip -d -c " + file_name + " > " + unzipped_file_name;
            int exit_code = system(command.c_str());
            if (exit_code != 0) {
                throw std::runtime_error("gzip failed");
            }
            std::cout << file_name << " unzipped." << std::endl;
        }
        return unzipped_file_name;
    } else {
        return file_name;
    }
}