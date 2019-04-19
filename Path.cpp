#include "Path.h"
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include "StringUtils.h"
#include <iostream>


struct CPath::SImplementation {
    std::string input_path;
    std::string conj_path;
};


//Done Constructor
CPath::CPath() : DImplementation(std::make_unique<SImplementation>()) {
    DImplementation->input_path = "";
    DImplementation->conj_path = "";
}

CPath::CPath(const CPath &path) : DImplementation(std::make_unique<SImplementation>()) {
    DImplementation->input_path = path.DImplementation->input_path;
    DImplementation->conj_path = path.DImplementation->conj_path;
}

CPath::CPath(const std::string &path) : DImplementation(std::make_unique<SImplementation>()) {
    DImplementation->input_path = path;
    if (path[0] == '.') {
        DImplementation->conj_path = GetCWD() + '/' + path;
    } else if (isalpha(path[0])) {
        DImplementation->conj_path = GetCWD() + "./" + path;
    } else {
        DImplementation->conj_path = path;
    }
    if (path[path.length() - 1] == '/') {
        DImplementation->input_path.erase(path.length() - 1, 1);
        DImplementation->conj_path.erase(path.length() - 1, 1);
    }
}

CPath::~CPath() {
    // Your code goes here
}


//Done
CPath &CPath::operator=(const CPath &path) {
    DImplementation->input_path = path.DImplementation->input_path;
    DImplementation->conj_path = path.DImplementation->conj_path;
    return *this;
}


//Done
CPath CPath::operator+(const CPath &path) const {
    CPath val;
    val.DImplementation->input_path = this->DImplementation->input_path + '/' + path.DImplementation->input_path;
    val.DImplementation->conj_path = this->DImplementation->conj_path + path.DImplementation->input_path;
    return val;
}


//Done
bool CPath::operator==(const CPath &path) const {
    bool result = true;
    if (DImplementation->input_path != path.DImplementation->input_path) {
        result = false;
    } else if (DImplementation->conj_path != path.DImplementation->conj_path) {
        result = false;
    }
    return result;
}


//Done
bool CPath::operator!=(const CPath &path) const {
    bool result = false;
    if (DImplementation->input_path != path.DImplementation->input_path) {
        result = true;
    } else if (DImplementation->conj_path != path.DImplementation->conj_path) {
        result = true;
    }
    return result;
}


//Done
CPath CPath::Directory() const {
    std::vector<std::string> path = StringUtils::Split(DImplementation->input_path, "/");

    if (!path[1].empty()){
        path.pop_back();
    }
    return CPath(StringUtils::Join("/", path));
}

//Done
std::string CPath::BaseName() const {
    std::vector<std::string> path = StringUtils::Split(DImplementation->input_path, "/");
    return path.back();
}

//Done
std::string CPath::Extension() const {
    std::vector<std::string> path = StringUtils::Split(DImplementation->input_path, "/");
    size_t pos = path.back().find('.');
    if (pos != std::string::npos) {
        return StringUtils::Slice(path.back(), pos);
    } else
        return "";
}

//Done
bool CPath::IsAbsolute() const {
    return DImplementation->input_path[0] == '/';
}

std::string CPath::ToString() const {
    return std::string(*this);
}


//Done
CPath::operator std::string() const {
    return DImplementation->input_path;
}

//Done
CPath CPath::AbsolutePath() const {
    return this->NormalizePath();
}

CPath CPath::CommonPath(const CPath &path) const {
    std::vector<std::string> dir1, dir2, cmn_path;
    dir1 = StringUtils::Split(this->NormalizePath(), "/");
    dir2 = StringUtils::Split(path.NormalizePath(), "/");
    for (int counter = 0; counter < int(dir1.size()); counter++) {
        if (dir1[counter] != dir2[counter]) {
            break;
        }
        cmn_path.push_back(dir1[counter]);
    }
    return CPath(StringUtils::Join("/", cmn_path));
}

//Done
CPath CPath::NormalizePath() const {
    std::string normalized;
    std::vector<std::string> dir;
    dir = StringUtils::Split(DImplementation->conj_path, "/");
    bool loop = true;
    while (loop) {
        for (int i = 0; i < int(dir.size()); i++) {
            if (dir[i].find("..") != std::string::npos) {
                dir.erase(dir.begin() + i - 1);
                dir.erase(dir.begin() + i - 1);
                loop = true;
                break;
            }
            loop = false;
        }
    }
    normalized = StringUtils::Join("/", dir);
    normalized = StringUtils::Replace(normalized, "./", "/");
    normalized = StringUtils::Replace(normalized, "//", "/");
    normalized = StringUtils::Replace(normalized, "~", std::string(CPath::HomePath()) + "/");

    return CPath(normalized);
}

CPath CPath::RelativePathTo(const CPath &path) const {
    std::string dir1, dir2, common, dist1, dist2, ret;
    dir1 = std::string(path.NormalizePath());
    dir2 = std::string(this->NormalizePath());
    common = std::string(CPath::CommonPath(CPath(dir1), CPath(dir2)));
    dist1 = StringUtils::Slice(dir1, common.length());
    dist2 = StringUtils::Slice(dir2, common.length());
    if (common == "" and (dist1 == "" or dist2 == "")) {
        for (char ch: dir1) {
            if (ch == '/') {
                ret += "../";
            }
        }
        ret += StringUtils::Slice(dir2, 1);
    } else if (common != "" and dist1 == "") {
        ret = "." + dist2;
    } else {
        for (char ch: dist1) {
            if (ch == '/') {
                ret += "../";
            }
        }
        ret += StringUtils::Slice(dist1, 1);
    }
    return CPath(ret);
}

// Helper function provided to get the current working directory
std::string CPath::GetCWD() {
    std::vector<char> CurrentPathName;

    CurrentPathName.resize(1024);
    while (NULL == getcwd(CurrentPathName.data(), CurrentPathName.size())) {
        CurrentPathName.resize(CurrentPathName.size() * 2);
    }
    return CurrentPathName.data();
}

// Helper funciton provided to get the home directory
std::string CPath::GetHome() {
    return std::getenv("HOME");
}

//Done
CPath CPath::AbsolutePath(const CPath &path) {
    return path.AbsolutePath();
}

//Done
CPath CPath::CurrentPath() {
    return CPath(GetCWD());
}

CPath CPath::CommonPath(const CPath &path1, const CPath &path2) {
    return path1.CommonPath(path2);
}


//Done
CPath CPath::ExpandUserPath(const CPath &path) {
    std::string temp = GetHome() + StringUtils::Slice(path.DImplementation->input_path, 1);
    return CPath(temp);
}


//Done
CPath CPath::HomePath() {
    CPath val(GetHome());
    return val;
}

//Done
CPath CPath::NormalizePath(const CPath &path) {
    return path.NormalizePath();
}

CPath CPath::RelativePath(const CPath &path, const CPath &startpath) {
    return path.RelativePathTo(startpath);
}
