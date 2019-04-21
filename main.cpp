#include <iostream>
#include "DirectoryListing.h"
#include "FileSystemTree.h"
#include "StringUtils.h"

int main(int argc, char *argv[]) {

    CFileSystemTree Tree;
    //CFileSystemTree::CEntryIterator i;
    //++i;


    Tree.Root().AddChild("Hello");
    Tree.Root().AddChild("etc");
    Tree.Root().AddChild("home");
    Tree.Root().AddChild("dev");
    Tree.Root().AddChild("usr");
    Tree.Root().AddChild("var");


    std::cout <<"Success";
    return 0;
}

