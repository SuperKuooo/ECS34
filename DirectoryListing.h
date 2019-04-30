#ifndef DIRECTORYLISTING_H
#define DIRECTORYLISTING_H

#include <string>
#include <tuple>
#include <vector>

namespace DirectoryListing {
    bool GetListing(const std::string &path, std::vector<std::tuple<std::string, std::string>> &entries);
}

#endif
