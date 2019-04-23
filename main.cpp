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


    //std::cout << Tree.Root().AddChild("home", true) << std::endl;
    //std::cout << Tree.Root().AddChild("home", true) << std::endl;
    //std::cout << Tree.Root().AddChild("dev") << std::endl;
    //std::cout << Tree.Root().AddChild("dev") << std::endl;
    //std::cout << Tree.Root().AddChild("usr") << std::endl;
    std::cout << Tree.Root().AddChild("c") << std::endl;
    std::cout << Tree.Root().AddChild("b") << std::endl;
    std::cout << Tree.Root().AddChild("a") << std::endl;
    //std::cout << Tree.Root().AddChild("/temp/cjnitta/ecs34", true) << std::endl;
    //std::cout << Tree.Root().AddChild("/home/proj2") << std::endl;
    //std::cout << Tree.Root().AddChild("/home/proj1/src", true) << std::endl;

    //std::cout << Tree.Root().AddChild("temp/cjnitta/ecs160", true) << std::endl;
    //std::cout << Tree.Root().AddChild("eev") << std::endl;

    std::cout << std::string(Tree.Root());

    //std::cout << StringUtils::Split("temp", "/")[0];

    return 0;
}

