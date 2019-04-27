#include <iostream>
#include "DirectoryListing.h"
#include "FileSystemTree.h"
#include "StringUtils.h"
#include <map>

/* 1. What do you mean by getting the data for a file? Do I need a file member for the struct? What are we storing in the file?
 * 2. There is no command that sets file content
 *
 *
 *
 * */
int main(int argc, char *argv[]) {

    CFileSystemTree Tree;
    std::vector<char> TempData;
    std::string temp;


    Tree.Root().AddChild("bin");
    Tree.Root().AddChild("etc");
    (Tree.Root().AddChild("home"));
    (Tree.Root().AddChild("dev"));
    (Tree.Root().AddChild("proc"));
    (Tree.Root().AddChild("usr"));
    (Tree.Root().AddChild("var"));
    (Tree.Root().AddChild("etc"));
    (Tree.Root().AddChild("/home/cjnitta/ecs34", true));
    (Tree.Root().AddChild("home/cjnitta/ecs34/proj1", true));
    (Tree.Root().AddChild("home/cjnitta/ecs34/proj2", true));
    (Tree.Root().AddChild("home/cjnitta/ecs36c", true));

    std::cout << std::string(Tree) << std::endl;


    return 0;
}