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
    //std::cout << Tree.Root().AddChild("var") << std::endl;
    //std::cout << Tree.Root().AddChild("home") << std::endl;
    //std::cout << Tree.Root().AddChild("/temp/cjnitta/ecs34", true) << std::endl;
    std::cout << Tree.Root().AddChild("temp/cjnitta/ecs34/proj1", true) << std::endl;
    std::cout << Tree.Root().AddChild("temp/cjnitta/ecs34/proj2", true) << std::endl;
    std::cout << Tree.Root().AddChild("temp/cjnitta/ecs160", true) << std::endl;
    //std::cout << Tree.Root().AddChild("eev") << std::endl;

    std::cout << std::string(Tree.Root());

    //std::cout << StringUtils::Split("temp", "/")[0];

    return 0;
}

