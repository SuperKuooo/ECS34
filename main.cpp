#include <iostream>
#include "DirectoryListing.h"
#include "FileSystemTree.h"
#include "StringUtils.h"
#include <map>


int main(int argc, char *argv[]) {

    CFileSystemTree Tree;

    Tree.Root().AddChild("bin");
    Tree.Root().AddChild("etc");
    Tree.Root().AddChild("home");
    Tree.Root().AddChild("dev");

    CFileSystemTree::CEntryIterator Iter = Tree.Root().begin();

    std::cout << Iter-> Name();

    return 0;
}

