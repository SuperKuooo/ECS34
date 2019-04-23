#include <iostream>
#include "DirectoryListing.h"
#include "FileSystemTree.h"
#include "StringUtils.h"
#include <map>


int main(int argc, char *argv[]) {

    CFileSystemTree Tree;



    /*for(auto elem : child_node)
    {
        std::cout << elem.first << " " << elem.second<< "\n";
    }*/
    /*Tree.Root().AddChild("Hello");
    Tree.Root().AddChild("etc");
    Tree.Root().AddChild("mai");
    Tree.Root().AddChild("maih");*/


    //std::cout << Tree.Root().AddChild("home") << std::endl;
    //std::cout << Tree.Root().AddChild("home") << std::endl;
    //std::cout << Tree.Root().AddChild("/home") << std::endl;

    //std::cout << Tree.Root().AddChild("dev") << std::endl;

    //std::cout << Tree.Root().AddChild("dev") << std::endl;
    // std::cout << Tree.Root().AddChild("usr") << std::endl;
    //std::cout << Tree.Root().AddChild("/temp/cjnitta/ecs34", true) << std::endl;
    //std::cout << Tree.Root().AddChild("/home1/proj1", true) << std::endl;
    //std::cout << Tree.Root().AddChild("/home3/proj3", true) << std::endl;
    //std::cout << Tree.Root().AddChild("/home2/proj2", true) << std::endl;
    //std::cout << Tree.Root().AddChild("/aome/proj2/ecs34", true) << std::endl;
    //std::cout << Tree.Root().Parent().Valid();

    //std::cout << Tree.Root().AddChild("temp/cjnitta/ecs160", true) << std::endl;
    //std::cout << Tree.Root().AddChild("eev") << std::endl;

    std::cout << std::string(Tree) << std::endl;
    std::cout << Tree.Root().FullPath() << std::endl;
    std::cout << Tree.Root().Valid() << std::endl;
    std::cout << Tree.Root().Parent().Valid() << std::endl;

    return 0;
}

