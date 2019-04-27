#include <iostream>
#include "DirectoryListing.h"
#include "FileSystemTree.h"
#include "StringUtils.h"
#include <map>
#include <iostream>

/*So what exactly do I need Directory Listing for? LOL
 *What do I need argc and argv for LOL
 *What do I need DFS for?
 *
 * */



int main(int argc, char *argv[]) {
    CFileSystemTree Tree;
    CFileSystemTree::CEntry *current_position;
    std::string command;
    std::vector<std::string> splt_command;
    bool done = false;

    current_position = &Tree.Root();
    //std::cout << StringUtils::Split("   mkdir     ")[0] << std::endl;

    while (!done) {
        std::cout << "> ";
        std::getline(std::cin, command);
        splt_command = StringUtils::Split(command);

        if (splt_command[0] == "exit") {
            done = true;
        } else if (splt_command[0] == "mkdir") {
            if (splt_command.size() == 1) {
                std::cout << "Error mkdir: missing parameter" << std::endl;
            } else {
                std::vector<std::string> splt_dir = StringUtils::Split(splt_command[1], "/");
                if (splt_dir.size() == 1) {
                    if (!current_position->AddChild(splt_command[1])) {
                        std::cout << "Error mkdir: failed to make directory " << splt_command[1] << std::endl;
                    }
                } else {
                    current_position->AddChild(splt_command[1], true);
                }
            }
        } else if (splt_command[0] == "pwd") {
            std::cout << current_position->FullPath() << std::endl;


        } else if (splt_command[0] == "cat") {
            if (splt_command.size() == 1) {
                std::cout << "Error cat: missing parameter";
            }else{
                std::vector<char> data;
                if (current_position->GetData(data)) {
                    for (auto ch: data) {
                        std::cout << ch << std::endl;
                    }
                } else {
                    std::cout << "Error cat: " << splt_command[1] << " is not a file";
                }
            }
        } else if (splt_command[0] == "cd") {
            std::string norm_path = current_position->FullPath() + splt_command[1];
            CFileSystemTree::CEntryIterator pos_pointer(Tree.Root().Find(norm_path));
            if (splt_command.size() > 1) {
                if (pos_pointer != Tree.NotFound()) {
                    current_position = pos_pointer.operator->();
                }
            }
        } else if (splt_command[0] == "clear") {
            std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl
                      << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl
                      << std::endl << std::endl << std::endl << std::endl << std::endl;
        } else if (splt_command[0] == "rm") {
            if (splt_command.size() == 1) {
                std::cout << "Error rm: missing parameter" << std::endl;
            } else {
                if (!current_position->RemoveChild(splt_command[1])) {
                    std::cout << "Error rm: failed to remove " << splt_command[1] << std::endl;
                }
            }
        } else if (splt_command[0] == "tree") {
            std::cout << std::string(*current_position) << std::endl;

        } else if (splt_command[0] == "ls") {
            CFileSystemTree::CEntryIterator ls;
            std::cout << current_position->ToString() << std::endl;
        } else {
            std::cout << "Unknown Command: " << command << std::endl;
        }


        std::cout << std::string(Tree) << std::endl;
    }

    return EXIT_SUCCESS;
}




//void DFS(const std::string &path, CFileSystemTree::CEntry &entry) {
//    std::vector<std::tuple<std::string, bool> > Entries;
//    std::string PathWithSlash = path;
//    if (path.back() != '/') {
//        PathWithSlash += "/";
//    }
//    std::cout << "In " << entry.FullPath() << std::endl;
//    DirectoryListing::GetListing(path, Entries);
//    for (auto &Entry : Entries) {
//        if (std::get<1>(Entry)) {
//            std::string DirName = std::get<0>(Entry);
//            if (DirName != ".")and(DirName != "..")
//            {
//                entry.AddChild(DirName);
//                DFS(PathWithSlash + DirName, *entry.Find(DirName));
//            }
//        } else {
//            entry.AddChild(std::get<0>(Entry));
//        }
//    }
//}