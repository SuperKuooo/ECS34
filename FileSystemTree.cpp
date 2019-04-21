#include "FileSystemTree.h"
#include "StringUtils.h"
#include <iostream>


struct CFileSystemTree::SImplementation {
    CFileSystemTree::CEntry root_node;
};

struct CFileSystemTree::CEntry::SImplementation {
    std::string current_node;
    std::vector<CFileSystemTree::CEntry *> child_node;
};

struct CFileSystemTree::CEntryIterator::SImplementation {
    std::vector<CFileSystemTree::CEntry *> nodes_list;
    CFileSystemTree::CEntry *location_ref;
    int counter = 0;
};

struct CFileSystemTree::CConstEntryIterator::SImplementation {
    // You implementation here
};

CFileSystemTree::CEntry::CEntry() : DImplementation(std::make_unique<SImplementation>()) {

}

CFileSystemTree::CEntry::CEntry(const CEntry &entry) : DImplementation(std::make_unique<SImplementation>()) {
    //DImplementation->child_node = std::make_shared<CFileSystemTree::CEntry>(entry);

}

CFileSystemTree::CEntry::~CEntry() {
    // You code here
}

CFileSystemTree::CEntry &CFileSystemTree::CEntry::operator=(const CEntry &entry) {
    this->DImplementation->current_node = entry.DImplementation->current_node;
    this->DImplementation->child_node = entry.DImplementation->child_node;
}

bool CFileSystemTree::CEntry::Valid() const {
    // You code here
}

std::string CFileSystemTree::CEntry::Name() const {
    // You code here
}

std::string CFileSystemTree::CEntry::FullPath() const {
    // You code here
}

std::string CFileSystemTree::CEntry::ToString() const {
    // You code here
}

CFileSystemTree::CEntry::operator std::string() const {
    std::string temp;
    temp += DImplementation->current_node;
    temp += "\n";
    for (auto str: DImplementation->child_node) {
        temp += str->DImplementation->current_node;
        temp += "/";
    }
    temp += "\n";
    return temp;
}

bool CFileSystemTree::CEntry::Rename(const std::string &name) {
    // You code here
}

size_t CFileSystemTree::CEntry::ChildCount() const {
    // You code here
}

bool CFileSystemTree::CEntry::SetChild(const std::string &name, CEntryIterator &iter) {
    // You code here
}

bool CFileSystemTree::CEntry::AddChild(const std::string &path, bool addall) {
    CEntry *new_node = new CEntry();
    CEntryIterator comp(this->Find(path));
    CFileSystemTree temp;

    if (comp == temp.NotFound()) {
        new_node->DImplementation->current_node = path;
        DImplementation->child_node.push_back(new_node);
        return true;
    }

    return false;
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
    // You code here
}

const CFileSystemTree::CEntry &CFileSystemTree::CEntry::Parent() const {
    // You code here
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::Find(const std::string &name) {
    CFileSystemTree temp;
    CFileSystemTree::CEntryIterator i;

    for (i = this->begin(); i != this->end(); i++) {
        if (i.DImplementation->location_ref->DImplementation->current_node == name) {
            //std::cout << i.DImplementation->nodes_list.size();
            return i;
        }
    }


    return temp.NotFound();
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::Find(const std::string &name) const {
    // You code here
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::begin() {
    CEntryIterator temp;

    temp.DImplementation->nodes_list = DImplementation->child_node;
    temp.DImplementation->counter = 0;

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

    temp.DImplementation->nodes_list = DImplementation->child_node;
    temp.DImplementation->counter = DImplementation->child_node.size();

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
    DImplementation->nodes_list = iter.DImplementation->nodes_list;
    DImplementation->counter = iter.DImplementation->counter;
    DImplementation->location_ref = iter.DImplementation->location_ref;

}

CFileSystemTree::CEntryIterator::~CEntryIterator() {
    // You code here
}

CFileSystemTree::CEntryIterator &CFileSystemTree::CEntryIterator::operator=(const CEntryIterator &iter) {
    DImplementation->nodes_list = iter.DImplementation->nodes_list;
    DImplementation->counter = iter.DImplementation->counter;
    if (DImplementation->location_ref != nullptr) {
        DImplementation->location_ref = iter.DImplementation->location_ref;
    } else {
        if (iter.DImplementation->nodes_list.size()) {
            DImplementation->location_ref = iter.DImplementation->nodes_list[0];
        }
    }
    return *this;
}

bool CFileSystemTree::CEntryIterator::operator==(const CEntryIterator &iter) const {
    if (DImplementation->nodes_list != iter.DImplementation->nodes_list) {
        return false;
    }
    if (DImplementation->counter != iter.DImplementation->counter) {
        return false;
    }

    return true;

}

bool CFileSystemTree::CEntryIterator::operator!=(const CEntryIterator &iter) const {
    return !(*this == iter);
}

CFileSystemTree::CEntryIterator &CFileSystemTree::CEntryIterator::operator++() {
    DImplementation->counter += 1;
    if (DImplementation->counter < DImplementation->nodes_list.size()) {
        DImplementation->location_ref = DImplementation->nodes_list[DImplementation->counter];
    }
    return *this;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator++(int) {
    CEntryIterator temp = *this;
    DImplementation->counter += 1;
    if (DImplementation->counter < DImplementation->nodes_list.size()) {
        DImplementation->location_ref = DImplementation->nodes_list[DImplementation->counter];
    }
    return temp;
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

CFileSystemTree::CConstEntryIterator::CConstEntryIterator(const CConstEntryIterator &iter) : DImplementation(
        std::make_unique<SImplementation>()) {
    // You code here
}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator(const CEntryIterator &iter) : DImplementation(
        std::make_unique<SImplementation>()) {
    // You code here
}

CFileSystemTree::CConstEntryIterator::~CConstEntryIterator() {
    // You code here
}

CFileSystemTree::CConstEntryIterator &CFileSystemTree::CConstEntryIterator::operator=(const CConstEntryIterator &iter) {
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
}

CFileSystemTree::CFileSystemTree(const CFileSystemTree &tree) : DImplementation(std::make_unique<SImplementation>()) {
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
    // You code here
}

CFileSystemTree::operator std::string() const {
    std::string temp;
    temp = DImplementation->root_node.DImplementation->current_node;
    //int var;
    //var = DImplementation->root_node.DImplementation->child_node.size();
    //std::cout << var;
    temp += DImplementation->root_node.DImplementation->child_node[0]->DImplementation->current_node;
    return temp;
}

CFileSystemTree::CEntryIterator CFileSystemTree::Find(const std::string &path) {
    /*std::vector<CFileSystemTree::CEntry *> dir = DImplementation->root_node.DImplementation->child_node;
    CFileSystemTree::CEntryIterator i;
    for (i = dir[0]; i)
*/

}

CFileSystemTree::CConstEntryIterator CFileSystemTree::Find(const std::string &path) const {
    // You code here
}

CFileSystemTree::CEntryIterator CFileSystemTree::NotFound() {
    CFileSystemTree::CEntryIterator empty;
    return empty;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::NotFound() const {
    // You code here
}
