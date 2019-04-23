#include <iostream>
#include "DirectoryListing.h"
#include "FileSystemTree.h"
#include "StringUtils.h"
#include <map>


int main(int argc, char *argv[]) {

    CFileSystemTree Tree;


    std::map<int, std::string> child_node;
    std::map<int, std::string>::iterator it;

    child_node.insert(std::pair<int, std::string>(5, "Hello"));
    child_node.insert(std::pair<int, std::string>(4, "Heldfsdflo"));
    child_node.insert(std::pair<int, std::string>(3, "Heo"));
    child_node.insert(std::pair<int, std::string>(3, "Helleeo"));


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
    std::cout << Tree.Root().AddChild("/home1/proj1", true) << std::endl;
    std::cout << Tree.Root().AddChild("/home3/proj3", true) << std::endl;
    std::cout << Tree.Root().AddChild("/home2/proj2", true) << std::endl;
    std::cout << Tree.Root().AddChild("/aome/proj2/ecs34", true) << std::endl;


    //std::cout << Tree.Root().AddChild("temp/cjnitta/ecs160", true) << std::endl;
    //std::cout << Tree.Root().AddChild("eev") << std::endl;

    std::cout << std::string(Tree.Root());

    return 0;
}

