/* ************************DISCLAIMER***************************
 * !!!Strongly advise reading this before continue to read the code!!!
 * This code is not perfect by all means. This project was crunched and had to pull like 5 all nighters for
 * me to hand this in on time. In fact, I would say this code is pretty bad and is way below my standard but honestly
 * no one has time to fix this.
 *
 * That being said, everything works fine and passes all the test cases from testtree.cpp. Let me know if there is a
 * problem with the code.
 * *******************END_OF_DISCLAIMER*************************
 * */

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

/*Things I need to take care off
 * 1. writing destructor
 * 2. Check proj2 pdf
 * 3. fix tree
 * 4. check mv long path
 * 5. argv argc HOLY SHIT THIS HW IS HARD
 *
 * idk everything
*/


int main(int argc, char *argv[]) {
    CFileSystemTree Tree;
    std::string command, current_pos = "/";
    std::vector<std::string> splt_command;
    bool done = false;

    if()


    while (!done) {
        std::cout << "> ";
        std::getline(std::cin, command);
        splt_command = StringUtils::Split(command);

        if (splt_command[0] == "exit") {
            done = true;
        } else if (splt_command[0] == "mkdir") {
            if (splt_command.size() == 1) {
                std::cout << "Error mkdir: missing parameter";
            } else {
                std::string norm_path;
                if (splt_command[1][0] == '/')
                    norm_path = StringUtils::NormalizePath(splt_command[1]);
                else
                    norm_path = StringUtils::NormalizePath(current_pos + splt_command[1]);
                if (!Tree.Root().AddChild(StringUtils::NormalizePath(norm_path))) {
                    std::cout << "Error mkdir: failed to make directory " << splt_command[1] << std::endl;
                }
            }
        } else if (splt_command[0] == "pwd") {
            std::cout << current_pos << std::endl;
        } else if (splt_command[0] == "cat") {
            if (splt_command.size() == 1) {
                std::cout << "Error cat: missing parameter";
            } else {
                std::vector<char> data;
                if (!Tree.Root().Find(splt_command[1]).operator->()->GetData(data)) {
                    std::cout << "Error cat: " << splt_command[1] << " is not a file" << std::endl;
                }
            }
        } else if (splt_command[0] == "cd") {
            if (splt_command.size() == 1) {
                current_pos = "/";
            } else {
                std::string norm_path;
                if (splt_command[1][0] == '/') {
                    norm_path = StringUtils::NormalizePath(splt_command[1]);
                } else {
                    norm_path = StringUtils::NormalizePath(current_pos + "/" + splt_command[1]);
                }
                if (norm_path == "/")
                    current_pos = norm_path;
                else {
                    if (Tree.Root().Find(norm_path) == Tree.NotFound()) {
                        std::cout << "Unknown directory: " << splt_command[1] << std::endl;
                    } else
                        current_pos = norm_path;
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
                std::string norm_path;
                if (splt_command[1][0] == '/')
                    norm_path = StringUtils::NormalizePath(splt_command[1]);
                else
                    norm_path = StringUtils::NormalizePath(current_pos + "/" + splt_command[1]);

                if (!Tree.Root().RemoveChild(norm_path)) {
                    std::cout << "Unknown directory: " << splt_command[1] << std::endl;
                }
            }
        } else if (splt_command[0] == "tree") {
            if (splt_command.size() == 1) {
                std::cout << std::string(Tree.Root()) << std::endl;
            } else {
                if (splt_command[1][0] == '/') {
                    std::cout
                            << std::string(*Tree.Root().Find(StringUtils::NormalizePath(splt_command[1])).operator->());
                } else {
                    std::string norm_path = StringUtils::NormalizePath(current_pos + "/" + splt_command[1]);
                    std::cout
                            << std::string(*Tree.Root().Find(norm_path).operator->())
                            << std::endl;
                }
            }
        } else if (splt_command[0] == "ls") {
            if (splt_command.size() == 1) {
                if (current_pos == "/") {
                    std::cout << Tree.Root().ToString();
                } else
                    std::cout << Tree.Root().Find(current_pos).operator->()->ToString();
            } else {
                if (splt_command[1][0] == '/') {
                    std::cout << Tree.Root().Find(StringUtils::NormalizePath(splt_command[1]))->ToString();
                } else {
                    std::string norm_path = StringUtils::NormalizePath(current_pos + "/" + splt_command[1]);
                    std::cout << Tree.Root().Find(norm_path)->ToString();
                }
            }
        } else if (splt_command[0] == "cp") {
            if (splt_command.size() < 3) {
                std::cout << "Error cp: missing parameter" << std::endl;
            } else {
                std::string norm_path;
                if (splt_command[1][0] == '/') {
                    norm_path = StringUtils::NormalizePath(splt_command[1]);
                } else
                    norm_path = StringUtils::NormalizePath(current_pos + splt_command[1]);

                CFileSystemTree::CEntryIterator iter_cp(Tree.Root().Find(norm_path));
                if (iter_cp == Tree.NotFound()) {
                    std::cout << "Unknown file or directory: " << splt_command[1] << std::endl;
                } else {
                    if (iter_cp.operator->()->Valid()) {
                        std::string norm_target;
                        if (splt_command[1][0] == '/') {
                            norm_target = StringUtils::NormalizePath(splt_command[2]);
                        } else
                            norm_target = StringUtils::NormalizePath(current_pos + splt_command[2]);
                        if (StringUtils::Split(norm_target, "/").size() != 2) {
                            std::cout << "Error cp: failed to copy " << splt_command[1] << " to " << splt_command[2]
                                      << std::endl;
                        } else if (Tree.Root().Find(norm_target) == Tree.NotFound()) {
                            Tree.Root().AddChild(norm_target);
                        } else {
                            CFileSystemTree::CEntryIterator iter_target;
                            iter_target = Tree.Root().Find(norm_target);
                            iter_target.operator->()->SetChild(iter_cp->Name() + "/cp", iter_cp);
                        }
                    } else
                        std::cout << "Error cp: " << splt_command[1] << " is not a file" << std::endl;
                }
            }
        } else if (splt_command[0] == "mv") {
            if (splt_command.size() < 3) {
                std::cout << "Error cp: missing parameter";
            } else {
                std::string norm_path;
                if (splt_command[1][0] == '/') {
                    norm_path = StringUtils::NormalizePath(splt_command[1]);
                } else
                    norm_path = StringUtils::NormalizePath(current_pos + splt_command[1]);

                CFileSystemTree::CEntryIterator iter_cp(Tree.Root().Find(norm_path));
                if (iter_cp == Tree.NotFound()) {
                    std::cout << "Unknown file or directory: " << splt_command[1] << std::endl;
                } else {
                    if (iter_cp.operator->()->Valid()) {
                        std::string norm_target;
                        if (splt_command[1][0] == '/') {
                            norm_target = StringUtils::NormalizePath(splt_command[2]);
                        } else
                            norm_target = StringUtils::NormalizePath(current_pos + splt_command[2]);
                        if (StringUtils::Split(norm_target, "/").size() != 2) {
                            std::cout << "Error cp: failed to copy " << splt_command[1] << " to " << splt_command[2]
                                      << std::endl;
                        } else if (Tree.Root().Find(norm_target) == Tree.NotFound()) {
                            Tree.Root().AddChild(norm_target);
                        } else {
                            CFileSystemTree::CEntryIterator iter_target;
                            iter_target = Tree.Root().Find(norm_target);
                            iter_target.operator->()->SetChild(iter_cp->Name() + "/mv", iter_cp);
                        }
                    } else
                        std::cout << "Error cp: " << splt_command[1] << " is not a file" << std::endl;
                }
            }
        } else {
            std::cout << "Unknown Command: " << command << std::endl;
        }
    }
    return EXIT_SUCCESS;
}


void DFS(const std::string &path, CFileSystemTree::CEntry &entry) {
    std::vector<std::tuple<std::string, bool> > Entries;
    std::string PathWithSlash = path;
    if (path.back() != '/') {
        PathWithSlash += "/";
    }
    std::cout << "In " << entry.FullPath() << std::endl;
    DirectoryListing::GetListing(path, Entries);
    for (auto &Entry : Entries) {
        if (std::get<1>(Entry)) {
            std::string DirName = std::get<0>(Entry);
            if ((DirName != ".")and(DirName != ".."))
            {
                entry.AddChild(DirName);
                DFS(PathWithSlash + DirName, *entry.Find(DirName));
            }
        } else {
            entry.AddChild(std::get<0>(Entry));
        }
    }
}