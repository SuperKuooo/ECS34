#include "FileSystemTree.h"
#include "StringUtils.h"
#include <iostream>
#include <algorithm>


bool CFileSystemTree::CEntry::Compare(CFileSystemTree::CEntry *left, CFileSystemTree::CEntry *right) {
    return std::string(*left) < std::string(*right);
}

struct CFileSystemTree::SImplementation {
    CFileSystemTree::CEntry root_node;
};

struct CFileSystemTree::CEntry::SImplementation { //This motherfuker is a doubly linked list
    CFileSystemTree::CEntry *parent_node;
    std::string current_node;
    std::vector<CFileSystemTree::CEntry *> child_node;
    int node_level = 0;
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
    DImplementation->child_node = entry.DImplementation->child_node;
    DImplementation->current_node = entry.DImplementation->current_node;
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

    std::sort(this->DImplementation->child_node.begin(), this->DImplementation->child_node.end(), Compare);

    for (int i = 1; i < this->DImplementation->node_level; i++) {
        if (this->DImplementation->parent_node !=
            (this->DImplementation->parent_node->DImplementation->parent_node->DImplementation->child_node
            [this->DImplementation->parent_node->DImplementation->parent_node->DImplementation->child_node.size() - 1]))
            temp += "|  ";
        else
            temp += "   ";
    }

    if (this->DImplementation->current_node == "/")
        temp += this->DImplementation->current_node + "\n";
    else if (this != this->DImplementation->parent_node->DImplementation
            ->child_node[this->DImplementation->parent_node->DImplementation->child_node.size() - 1])
        temp += "|--" + this->DImplementation->current_node + "\n";
    else
        temp += "`--" + this->DImplementation->current_node + "\n";


    if (!this->DImplementation->child_node.empty()) {
        for (CEntry *dir :this->DImplementation->child_node) {
            temp += std::string(*dir);
        }
        return temp;
    }
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
    std::vector<std::string> dir_vect = StringUtils::Split(path, "/");
    CEntry *new_node = new CEntry();
    CEntryIterator found(this->Find(dir_vect[0]));
    CFileSystemTree temp_tree;

    if (!path.empty()) {
        if (path[0] == '/') {
            if (this->DImplementation->parent_node == nullptr) {
                return this->AddChild(StringUtils::Slice(path, 1), true);
            } else
                return false;
        }
    } else
        return false;


    if (found != temp_tree.NotFound()) {
        CEntryIterator pos(this->Find(dir_vect[0]));
        dir_vect.erase(dir_vect.begin());
        if (dir_vect.size() <= 1)
            return pos.DImplementation->location_ref->AddChild(dir_vect[0]);
        else
            return pos.DImplementation->location_ref->AddChild(StringUtils::Join("/", dir_vect), true);
    }
    if (addall) {
        this->AddChild(dir_vect[0]);
        CEntryIterator pos(this->Find(dir_vect[0]));
        dir_vect.erase(dir_vect.begin());
        if (dir_vect.size() > 1) {
            return pos.DImplementation->location_ref->AddChild(StringUtils::Join("/", dir_vect), true);
        } else {
            return pos.DImplementation->location_ref->AddChild(dir_vect[0]);
        }
    } else {
        new_node->DImplementation->node_level = this->DImplementation->node_level + 1;
        new_node->DImplementation->parent_node = this;
        new_node->DImplementation->current_node = path;
        DImplementation->child_node.push_back(new_node);
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
    CFileSystemTree::CEntryIterator i;

    for (i = this->begin(); i != this->end(); i++) {
        if (i.DImplementation->location_ref->DImplementation->current_node == name) {
            return ++i;
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
    //DImplementation->location_ref = iter.DImplementation->location_ref;

    if (DImplementation->location_ref != nullptr) {
        DImplementation->location_ref = iter.DImplementation->location_ref;
    } else if (DImplementation->location_ref != nullptr and iter.DImplementation->location_ref != nullptr) {
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
    //tsk
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
    // You code here
}

CFileSystemTree::operator std::string() const {
    return std::string(this->DImplementation->root_node);
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
