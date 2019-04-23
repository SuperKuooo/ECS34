#include "FileSystemTree.h"
#include "StringUtils.h"
#include <iostream>
#include <map>
#include <algorithm>


struct CFileSystemTree::SImplementation {
    CFileSystemTree::CEntry root_node;
};

struct CFileSystemTree::CEntry::SImplementation { //This motherfuker is a doubly linked list
    CFileSystemTree::CEntry *parent_node;
    std::string current_node;
    std::map<CFileSystemTree::CEntry *, std::string> child_node;
    int node_level = 0;
};

struct CFileSystemTree::CEntryIterator::SImplementation {
    std::map<CFileSystemTree::CEntry *, std::string>::iterator iter;
};

struct CFileSystemTree::CConstEntryIterator::SImplementation {
    // You implementation here
};

CFileSystemTree::CEntry::CEntry() : DImplementation(std::make_unique<SImplementation>()) {

}

CFileSystemTree::CEntry::CEntry(const CEntry &entry) : DImplementation(std::make_unique<SImplementation>()) {
    DImplementation->child_node = entry.DImplementation->child_node;
    DImplementation->current_node = entry.DImplementation->current_node;
    DImplementation->parent_node = entry.DImplementation->parent_node;
    DImplementation->node_level = entry.DImplementation->node_level;
}

CFileSystemTree::CEntry::~CEntry() {
    // You code here
}

CFileSystemTree::CEntry &CFileSystemTree::CEntry::operator=(const CEntry &entry) {
    this->DImplementation->current_node = entry.DImplementation->current_node;
    this->DImplementation->child_node = entry.DImplementation->child_node;
    this->DImplementation->parent_node = entry.DImplementation->parent_node;
    this->DImplementation->node_level = entry.DImplementation->node_level;
}

bool CFileSystemTree::CEntry::Valid() const {
    // You code here
}

std::string CFileSystemTree::CEntry::Name() const {

}

std::string CFileSystemTree::CEntry::FullPath() const {
    // You code here
}

std::string CFileSystemTree::CEntry::ToString() const {
    // You code here
}


CFileSystemTree::CEntry::operator std::string() const { //This code is garbo LOL. It is actually very very bad.
    //But honestly, nobody have time to fix it. It works. That's all it matters.

    std::string temp;

    for (int i = 1; i < this->DImplementation->node_level; i++) {
        temp += "   ";
    }

    temp += this->DImplementation->current_node + "\n";

    if (!this->DImplementation->child_node.empty()) {
        for (auto it :this->DImplementation->child_node) {
            temp += std::string(*it.first);
        }
        return temp;
    }


    return temp;
}

bool CFileSystemTree::CEntry::Rename(const std::string &name) {
    CEntryIterator it(this->Find(name));
    CFileSystemTree temp_tree;
    if (it == temp_tree.NotFound())
        this->DImplementation->current_node = name;
    else
        return false;
    return true;

}

size_t CFileSystemTree::CEntry::ChildCount() const {
    return size_t(DImplementation->child_node.size());
}

bool CFileSystemTree::CEntry::SetChild(const std::string &name, CEntryIterator &iter) {
    // You code here
}

bool CFileSystemTree::CEntry::AddChild(const std::string &path, bool addall) {
    std::vector<std::string> dir_vect = StringUtils::Split(path, "/");
    CEntry *new_node = new CEntry();
    CEntryIterator found(this->Find(dir_vect[0]));
    CFileSystemTree temp_tree;

    if (path.empty()) {
        return false;
    } else {
        if (path[0] == '/') {
            if (this->DImplementation->parent_node == nullptr) {
                return this->AddChild(StringUtils::Slice(path, 1), true);
            } else {
                return false;
            }
        }
    }

    if (found != temp_tree.NotFound()) {
        dir_vect.erase(dir_vect.begin());
        return found.DImplementation->iter->first->AddChild(StringUtils::Join("/", dir_vect), true);
    }
    if (addall) {
        this->AddChild(dir_vect[0]);
        CEntryIterator temp(this->Find(dir_vect[0]));
        dir_vect.erase(dir_vect.begin());
        temp.DImplementation->iter->first->AddChild(StringUtils::Join("/", dir_vect), true);
        return true;
    } else {
        new_node->DImplementation->node_level = this->DImplementation->node_level + 1;
        new_node->DImplementation->parent_node = this;
        new_node->DImplementation->current_node = path;
        this->DImplementation->child_node.insert(std::pair<CEntry *, std::string>(new_node, ""));
        return true;
    }
}


bool CFileSystemTree::CEntry::RemoveChild(const std::string &path) {
    // You code here
}

bool CFileSystemTree::CEntry::SetData(const std::vector<char> &data) {
    // You code here
}

bool CFileSystemTree::CEntry::GetData(std::vector<char> &data) const {
    // You code here
}

CFileSystemTree::CEntry &CFileSystemTree::CEntry::Parent() {
    return *this->DImplementation->parent_node;
}

const CFileSystemTree::CEntry &CFileSystemTree::CEntry::Parent() const {
    return *this->DImplementation->parent_node;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::Find(const std::string &name) {
    CFileSystemTree temp;
    CEntryIterator it;

    for (it = this->begin(); it != this->end(); it++) {
        if (it.DImplementation->iter->first->DImplementation->current_node == name) {
            return it;
        }
    }
    return temp.NotFound();
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::Find(const std::string &name) const {
    // You code here
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::begin() {
    CEntryIterator temp;
    temp.DImplementation->iter = this->DImplementation->child_node.begin();
    return temp;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::begin() const {
    // You code here
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::cbegin() const {
    // You code here
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::end() {
    CEntryIterator temp;
    temp.DImplementation->iter = this->DImplementation->child_node.end();
    return temp;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::end() const {
    // You code here
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::cend() const {
    // You code here
}

CFileSystemTree::CEntryIterator::CEntryIterator() : DImplementation(std::make_unique<SImplementation>()) {

}

CFileSystemTree::CEntryIterator::CEntryIterator(const CEntryIterator &iter) : DImplementation(
        std::make_unique<SImplementation>()) {
    DImplementation->iter = iter.DImplementation->iter;
}

CFileSystemTree::CEntryIterator::~CEntryIterator() {
    // You code here
}

CFileSystemTree::CEntryIterator &CFileSystemTree::CEntryIterator::operator=(const CEntryIterator &iter) {
    DImplementation->iter = iter.DImplementation->iter;
    return *this;
}

bool CFileSystemTree::CEntryIterator::operator==(const CEntryIterator &iter) const {
    return DImplementation->iter == iter.DImplementation->iter;
}

bool CFileSystemTree::CEntryIterator::operator!=(const CEntryIterator &iter) const {
    return !(*this == iter);
}

CFileSystemTree::CEntryIterator &CFileSystemTree::CEntryIterator::operator++() {
    std::map<CFileSystemTree::CEntry *, std::string>::iterator iter_temp;
    iter_temp = DImplementation->iter;
    ++iter_temp;
    DImplementation->iter = iter_temp;
    return *this;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator++(int) {
    std::map<CFileSystemTree::CEntry *, std::string>::iterator iter_temp;
    iter_temp = DImplementation->iter;
    iter_temp++;
    DImplementation->iter = iter_temp;
    return *this;
}

CFileSystemTree::CEntryIterator &CFileSystemTree::CEntryIterator::operator--() {
    // You code here
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator--(int) {
    // You code here
}

CFileSystemTree::CEntry &CFileSystemTree::CEntryIterator::operator*() const {
    // You code here
}

CFileSystemTree::CEntry *CFileSystemTree::CEntryIterator::operator->() const {
    // You code here
}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator() : DImplementation(std::make_unique<SImplementation>()) {

}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator(
        const CConstEntryIterator &iter) : DImplementation(
        std::make_unique<SImplementation>()) {
    // You code here
}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator(
        const CEntryIterator &iter) : DImplementation(
        std::make_unique<SImplementation>()) {
    // You code here
}

CFileSystemTree::CConstEntryIterator::~CConstEntryIterator() {
    // You code here
}

CFileSystemTree::CConstEntryIterator &
CFileSystemTree::CConstEntryIterator::operator=(const CConstEntryIterator &iter) {
    // You code here
}

bool CFileSystemTree::CConstEntryIterator::operator==(const CConstEntryIterator &iter) const {
    // You code here
}

bool CFileSystemTree::CConstEntryIterator::operator!=(const CConstEntryIterator &iter) const {
    // You code here
}

CFileSystemTree::CConstEntryIterator &CFileSystemTree::CConstEntryIterator::operator++() {
    // You code here
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CConstEntryIterator::operator++(int) {
    // You code here
}

CFileSystemTree::CConstEntryIterator &CFileSystemTree::CConstEntryIterator::operator--() {
    // You code here
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CConstEntryIterator::operator--(int) {
    // You code here
}

const CFileSystemTree::CEntry &CFileSystemTree::CConstEntryIterator::operator*() const {
    // You code here
}

const CFileSystemTree::CEntry *CFileSystemTree::CConstEntryIterator::operator->() const {
    // You code here
}

CFileSystemTree::CFileSystemTree() : DImplementation(std::make_unique<SImplementation>()) {
    DImplementation->root_node.DImplementation->current_node = "/";
    DImplementation->root_node.DImplementation->parent_node = nullptr;

}

CFileSystemTree::CFileSystemTree(
        const CFileSystemTree &tree) : DImplementation(std::make_unique<SImplementation>()) {
    // You code here
}

CFileSystemTree::~CFileSystemTree() {
    // You code here
}

CFileSystemTree &CFileSystemTree::operator=(const CFileSystemTree &tree) {
    // You code here
}

CFileSystemTree::CEntry &CFileSystemTree::Root() {
    return DImplementation->root_node;
}

const CFileSystemTree::CEntry &CFileSystemTree::Root() const {
    // You code here
}

std::string CFileSystemTree::ToString() const {
    return this->DImplementation->root_node.DImplementation->current_node;//child_node.begin()
    //->first->DImplementation->current_node;
}

CFileSystemTree::operator std::string() const {
    return std::string(this->DImplementation->root_node);
}

CFileSystemTree::CEntryIterator CFileSystemTree::Find(const std::string &path) {
    return this->DImplementation->root_node.Find(path);
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::Find(const std::string &path) const {

}

CFileSystemTree::CEntryIterator CFileSystemTree::NotFound() {
    CFileSystemTree::CEntryIterator empty;
    return empty;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::NotFound() const {
    // You code here
}
