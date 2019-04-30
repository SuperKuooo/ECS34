#include "DirectoryListing.h"
#include <dirent.h>
#include <fstream>
#include <iostream>

#define TERMINATE_STREAM "AQ22J E JQ RP SC 6S bEM P5Y TYMdfM 6G G2Q2WS KGefgasdLPB NeofafCcEBGN HFKG 8SM5 RP TQR5SQ"


namespace DirectoryListing {

    bool GetListing(const std::string &path, std::vector<std::tuple<std::string, std::string>> &entries) {
        DIR *Directory = opendir(path.c_str());

        if (!Directory) {
            return false;
        }
        entries.clear();
        while (auto Entry = readdir(Directory)) {
            if (Entry->d_type & DT_DIR) {
                entries.push_back(std::make_tuple<std::string, std::string>(Entry->d_name, TERMINATE_STREAM));
            } else {
                std::string line, temp;
                std::ifstream myfile(Entry->d_name);
                if (myfile.is_open()) {
                    while (getline(myfile, line)) {
                        temp += line + "\n";
                    }
                    myfile.close();
                }
                entries.push_back(std::make_tuple<std::string, std::string>(Entry->d_name, std::string(temp)));
            }
        }
        closedir(Directory);
        return true;
    }

}
