#include "FileSystemTree.h"
#include "StringUtils.h"
#include <iostream>
#include <map>
#include <algorithm>

#define TERMINATE_STREAM "AQ22J E JQ RP SC 6S bEM P5Y TYMdfM 6G G2Q2WS KGefgasdLPB NeofafCcEBGN HFKG 8SM5 RP TQR5SQ"
//LMAO I know. Like I said, it works. That's all it matters.


struct CFileSystemTree::SImplementation {
    CFileSystemTree::CEntry root_node;
};

struct CFileSystemTree::CEntry::SImplementation {
    CFileSystemTree::CEntry *parent_node;
    std::string current_node;
    std::map<std::string, CFileSystemTree::CEntry *> child_node;
    int node_level = 0;
};

struct CFileSystemTree::CEntryIterator::SImplementation {
    CEntry *parent;
    std::map<std::string, CFileSystemTree::CEntry *>::iterator iter;
};

struct CFileSystemTree::CConstEntryIterator::SImplementation {
    const CEntry *parent;
    std::map<std::string, CFileSystemTree::CEntry *>::iterator iter;
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
    if (this->DImplementation->current_node == TERMINATE_STREAM)
        return false;
    return !this->DImplementation->current_node.empty();
}

std::string CFileSystemTree::CEntry::Name() const {
    return this->DImplementation->current_node;
}

std::string CFileSystemTree::CEntry::FullPath() const {
    CEntry temp_node(*this);
    std::string ret_val;

    if (!this->DImplementation->node_level)
        return "/";
    while (temp_node.DImplementation->node_level) {
        ret_val = "/" + temp_node.DImplementation->current_node + ret_val;
        temp_node = *temp_node.DImplementation->parent_node;
    }
    return ret_val;
}

std::string CFileSystemTree::CEntry::ToString() const {
    return std::string(*this);
}

void CFileSystemTree::aux_string(std::string &tot_str, const CFileSystemTree::CEntry *node) {
    CEntry *temp;
    CFileSystemTree::CConstEntryIterator iter;
    for (iter = node->begin(); iter != node->end(); iter++) {
        temp = iter.DImplementation->iter->second;
        //std::cout << node->end()->DImplementation->current_node;
        for (int i = 0; i < temp->DImplementation->node_level; i++) {
            if (iter.DImplementation->iter->first ==
                iter.DImplementation->parent->DImplementation->child_node.end()->first) {
                tot_str += "|--";
            } else
                tot_str += "`--";
        }
        tot_str += iter.DImplementation->iter->first + "\n";
        if (!node->DImplementation->child_node.empty()) {
            aux_string(tot_str, iter.DImplementation->iter->second);
        }
    }
}

CFileSystemTree::CEntry::operator std::string() const {
    //This code is garbo LOL. It is actually very very bad.
    //But honestly, nobody have time to fix it. It works. That's all it matters.
    std::string tot_str;

    tot_str += this->DImplementation->current_node + "\n";
    aux_string(tot_str, this);

    return tot_str;
}

bool CFileSystemTree::CEntry::Rename(const std::string &name) {
    CEntryIterator it(this->DImplementation->parent_node->Find(name));
    CFileSystemTree temp_tree;

    if (it == temp_tree.NotFound()) {
        this->DImplementation->parent_node->AddChild(name);
        it = this->DImplementation->parent_node->Find(name);
        it.DImplementation->iter->second = this;
        this->DImplementation->current_node = name;
        this->DImplementation->parent_node->RemoveChild(name);
    } else
        return false;
    return true;

}

size_t CFileSystemTree::CEntry::ChildCount() const {
    return size_t(DImplementation->child_node.size());
}

bool CFileSystemTree::CEntry::SetChild(const std::string &name, CEntryIterator &iter) {
    CEntry *looper;
    CEntryIterator it, temp, erase;
    if (this->AddChild(name)) {
        temp = this->Find(name);
    } else
        return false;
    temp.DImplementation->iter->second = iter.DImplementation->iter->second;
    looper = temp.DImplementation->iter->second;
    erase = iter.DImplementation->parent->Find(name);
    iter.DImplementation->parent->DImplementation->child_node.erase(erase.DImplementation->iter->first);

    temp.DImplementation->iter->second->DImplementation->node_level--;
    for (temp = looper->begin(); temp != looper->end(); temp++) {
        temp.DImplementation->iter->second->DImplementation->node_level--;
    }

    return true;
}

bool CFileSystemTree::CEntry::AddChild(const std::string &path, bool addall) {
    std::vector<std::string> dir_vect = StringUtils::Split(path, "/");
    CEntry *new_node = new CEntry();
    CEntryIterator found(this->Find(dir_vect[0]));
    CFileSystemTree temp_tree;
    if (!this->DImplementation->child_node.empty()) {
        if (this->DImplementation->child_node.begin()->second->DImplementation->current_node == TERMINATE_STREAM) {
            return false;
        }
    }

    if (path.empty() || !this->Valid()) {
        return false;
    } else {
        if (path[0] == '/') {
            if (this->DImplementation->parent_node->DImplementation->current_node == TERMINATE_STREAM) {
                return this->AddChild(StringUtils::Slice(path, 1), true);
            } else {
                return false;
            }
        }
        if (dir_vect[0] == ".") {
            dir_vect.erase(dir_vect.begin());
            return this->AddChild(StringUtils::Join("/", dir_vect), true);
        }
        if (dir_vect[0] == "..") {
            dir_vect.erase(dir_vect.begin());
            return this->DImplementation->parent_node->AddChild(StringUtils::Join("/", dir_vect), true);
        }
    }

    if (found != temp_tree.NotFound()) {
        dir_vect.erase(dir_vect.begin());
        return found->AddChild(StringUtils::Join("/", dir_vect), true);
    }
    if (addall) {
        this->AddChild(dir_vect[0]);
        CEntryIterator temp(this->Find(dir_vect[0]));
        dir_vect.erase(dir_vect.begin());
        temp.DImplementation->iter->second->AddChild(StringUtils::Join("/", dir_vect), true);
        return true;
    } else {
        new_node->DImplementation->node_level = this->DImplementation->node_level + 1;
        new_node->DImplementation->parent_node = this;
        new_node->DImplementation->current_node = path;
        this->DImplementation->child_node.insert(std::pair<std::string, CEntry *>(path, new_node));
        return true;
    }
}


bool CFileSystemTree::CEntry::RemoveChild(const std::string &path) {
    std::vector<std::string> dir = StringUtils::Split(path);
    CFileSystemTree::CEntryIterator iter(this->Find(path));
    CFileSystemTree temp_tree;

    if (iter == temp_tree.NotFound())
        return false;
    else
        this->DImplementation->child_node.erase(iter.DImplementation->iter);
    return true;
}

bool CFileSystemTree::CEntry::SetData(const std::vector<char> &data) {
    CEntry *temp = new CEntry;
    temp->DImplementation->current_node = TERMINATE_STREAM;

    if (this->DImplementation->child_node.empty()) {
        for (auto elements: data) {
            this->DImplementation->child_node.insert(
                    std::pair<std::string, CEntry *>(std::string(1, elements), temp));
        }
        return true;
    }
    return false;
}

bool CFileSystemTree::CEntry::GetData(std::vector<char> &data) const {
    CConstEntryIterator iter;
    std::vector<char> temp;

    if (!this->DImplementation->child_node.empty()) {
        for (iter = this->begin(); iter != this->end(); iter++) {
            if (iter.DImplementation->iter->second->DImplementation->current_node != TERMINATE_STREAM) {
                return false;
            }
            temp.push_back(iter.DImplementation->iter->first[0]);
        }
        data = temp;
        return true;
    }
    return false;
}

CFileSystemTree::CEntry &CFileSystemTree::CEntry::Parent() {
    return *this->DImplementation->parent_node;
}

const CFileSystemTree::CEntry &CFileSystemTree::CEntry::Parent() const {
    return *this->DImplementation->parent_node;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::Find(const std::string &name) {
    std::vector<std::string> I_ran_out_of_ideas = StringUtils::Split(name, "/");
    CEntry *ref_node;
    CEntryIterator iter;
    CFileSystemTree temp_tree;
    ref_node = this;

    if (!name.empty()) {
        if (name[0] == '/') {
            return this->Find(StringUtils::Slice(name, 1));
        }
    }

    if (I_ran_out_of_ideas.size() == 1) {
        for (iter = this->begin(); iter != this->end(); iter++) {
            if (iter.DImplementation->iter->second->DImplementation->current_node == name) {
                return iter;
            }
        }
        return temp_tree.NotFound();
    } else {
        int counter = 0;
        do {
            if (!ref_node->DImplementation->child_node.empty()) {
                iter = ref_node->Find(I_ran_out_of_ideas[counter]);
                counter++;
                if (counter >= I_ran_out_of_ideas.size())
                    break;
                if (iter != temp_tree.NotFound())
                    ref_node = iter.DImplementation->iter->second;
                else {
                    iter = temp_tree.NotFound();
                    break;
                }

            } else
                break;
        } while (true);

        if (iter == temp_tree.NotFound())
            return iter;
        else if (iter.DImplementation->iter->second->DImplementation->current_node ==
                 I_ran_out_of_ideas[I_ran_out_of_ideas.size() - 1] and
                 iter.DImplementation->iter->second->DImplementation->node_level == I_ran_out_of_ideas.size())
            return iter;
        else
            return temp_tree.NotFound();
    }
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::Find(const std::string &name) const {
    //CConstEntryIterator (this->Find(name));

    //return temp;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::begin() {
    CEntryIterator temp;
    temp.DImplementation->parent = this;
    temp.DImplementation->iter = this->DImplementation->child_node.begin();
    return temp;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::begin() const {
    CConstEntryIterator temp;
    temp.DImplementation->parent = this;
    temp.DImplementation->iter = this->DImplementation->child_node.begin();
    return temp;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::cbegin() const {
    // You code here
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::end() {
    CEntryIterator temp;
    temp.DImplementation->parent = this;
    temp.DImplementation->iter = this->DImplementation->child_node.end();
    return temp;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::end() const {
    CConstEntryIterator temp;
    temp.DImplementation->iter = this->DImplementation->child_node.end();
    return temp;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::cend() const {
    // You code here
}

CFileSystemTree::CEntryIterator::CEntryIterator() : DImplementation(std::make_unique<SImplementation>()) {

}

CFileSystemTree::CEntryIterator::CEntryIterator(const CEntryIterator &iter) : DImplementation(
        std::make_unique<SImplementation>()) {
    DImplementation->iter = iter.DImplementation->iter;
    DImplementation->parent = iter.DImplementation->parent;
}

CFileSystemTree::CEntryIterator::~CEntryIterator() {
    // You code here
}

CFileSystemTree::CEntryIterator &CFileSystemTree::CEntryIterator::operator=(const CEntryIterator &iter) {
    DImplementation->iter = iter.DImplementation->iter;
    DImplementation->parent = iter.DImplementation->parent;
    return *this;
}

bool CFileSystemTree::CEntryIterator::operator==(const CEntryIterator &iter) const {
    return DImplementation->iter == iter.DImplementation->iter;
}

bool CFileSystemTree::CEntryIterator::operator!=(const CEntryIterator &iter) const {
    return !(*this == iter);
}

CFileSystemTree::CEntryIterator &CFileSystemTree::CEntryIterator::operator++() {
    std::map<std::string, CFileSystemTree::CEntry *>::iterator iter_temp;
    iter_temp = DImplementation->iter;
    ++iter_temp;
    DImplementation->iter = iter_temp;
    return *this;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator++(int) {
    std::map<std::string, CFileSystemTree::CEntry *>::iterator iter_temp;
    iter_temp = DImplementation->iter;
    iter_temp++;
    DImplementation->iter = iter_temp;
    return *this;
}

CFileSystemTree::CEntryIterator &CFileSystemTree::CEntryIterator::operator--() {
    std::map<std::string, CFileSystemTree::CEntry *>::iterator iter_temp;
    iter_temp = DImplementation->iter;
    --iter_temp;
    DImplementation->iter = iter_temp;
    return *this;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator--(int) {
    std::map<std::string, CFileSystemTree::CEntry *>::iterator iter_temp;
    iter_temp = DImplementation->iter;
    iter_temp--;
    DImplementation->iter = iter_temp;
    return *this;
}

CFileSystemTree::CEntry &CFileSystemTree::CEntryIterator::operator*() const {
    // You code here
}

CFileSystemTree::CEntry *CFileSystemTree::CEntryIterator::operator->() const {
    return DImplementation->iter->second;
}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator() : DImplementation(std::make_unique<SImplementation>()) {

}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator(
        const CConstEntryIterator &iter) : DImplementation(
        std::make_unique<SImplementation>()) {
    DImplementation->iter = iter.DImplementation->iter;
}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator(
        const CEntryIterator &iter) : DImplementation(
        std::make_unique<SImplementation>()) {
    DImplementation->iter = iter.DImplementation->iter;
}

CFileSystemTree::CConstEntryIterator::~CConstEntryIterator() {
}

CFileSystemTree::CConstEntryIterator &
CFileSystemTree::CConstEntryIterator::operator=(const CConstEntryIterator &iter) {
    DImplementation->iter = iter.DImplementation->iter;
    return *this;
}

bool CFileSystemTree::CConstEntryIterator::operator==(const CConstEntryIterator &iter) const {
    return DImplementation->iter == iter.DImplementation->iter;
}

bool CFileSystemTree::CConstEntryIterator::operator!=(const CConstEntryIterator &iter) const {
    return !(*this == iter);
}

CFileSystemTree::CConstEntryIterator &CFileSystemTree::CConstEntryIterator::operator++() {
    std::map<std::string, CFileSystemTree::CEntry *>::iterator iter_temp;
    iter_temp = DImplementation->iter;
    iter_temp++;
    DImplementation->iter = iter_temp;
    return *this;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CConstEntryIterator::operator++(int) {
    std::map<std::string, CFileSystemTree::CEntry *>::iterator iter_temp;
    iter_temp = DImplementation->iter;
    iter_temp++;
    DImplementation->iter = iter_temp;
    return *this;
}

CFileSystemTree::CConstEntryIterator &CFileSystemTree::CConstEntryIterator::operator--() {
    std::map<std::string, CFileSystemTree::CEntry *>::iterator iter_temp;
    iter_temp = DImplementation->iter;
    --iter_temp;
    DImplementation->iter = iter_temp;
    return *this;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CConstEntryIterator::operator--(int) {
    std::map<std::string, CFileSystemTree::CEntry *>::iterator iter_temp;
    iter_temp = DImplementation->iter;
    iter_temp--;
    DImplementation->iter = iter_temp;
    return *this;
}

const CFileSystemTree::CEntry &CFileSystemTree::CConstEntryIterator::operator*() const {
}

const CFileSystemTree::CEntry *CFileSystemTree::CConstEntryIterator::operator->() const {
    return DImplementation->iter->second;
}

CFileSystemTree::CFileSystemTree() : DImplementation(std::make_unique<SImplementation>()) {
    DImplementation->root_node.DImplementation->current_node = "/";
    DImplementation->root_node.DImplementation->parent_node = new(CEntry);
    DImplementation->root_node.DImplementation->parent_node->DImplementation->current_node = TERMINATE_STREAM;
    //lmao I mean it works.
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
    return this->DImplementation->root_node.DImplementation->current_node;
}

CFileSystemTree::operator std::string() const {
    return std::string(this->DImplementation->root_node);
}

CFileSystemTree::CEntryIterator CFileSystemTree::Find(const std::string &path) {
    return this->DImplementation->root_node.Find(path);
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::Find(const std::string &path) const {
    CConstEntryIterator temp(this->DImplementation->root_node.Find(path));
    return temp;
}

CFileSystemTree::CEntryIterator CFileSystemTree::NotFound() {
    CFileSystemTree::CEntryIterator empty;
    return empty;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::NotFound() const {
    CFileSystemTree::CConstEntryIterator empty;
    return empty;
}