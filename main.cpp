#include <iostream>
#include "DirectoryListing.h"
#include "FileSystemTree.h"
#include "StringUtils.h"

int main(int argc, char *argv[]) {

    CFileSystemTree Tree;

    /*Tree.Root().AddChild("Hello");
    Tree.Root().AddChild("etc");
    Tree.Root().AddChild("mai");
    Tree.Root().AddChild("maih");*/


    std::cout<<Tree.Root().AddChild("home");
    //std::cout<<Tree.Root().AddChild("dev")<<std::endl;
    //std::cout<<Tree.Root().AddChild("usr")<<std::endl;
    //std::cout<<Tree.Root().AddChild("var")<<std::endl;
    std::cout<<Tree.Root().AddChild("home");


    //std::cout << "Success";
    return 0;
}

