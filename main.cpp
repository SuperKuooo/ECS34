#include <iostream>
#include "StringUtils.h"
#include "Path.h"

int main(int argc, char *argv[]) {
    bool Done = false;
    do {
        std::string CommandString;
        std::cout << std::endl << "1 - Title String" << std::endl;
        std::cout << "2 - Compare Strings" << std::endl;
        std::cout << "3 - Absolute Path" << std::endl;
        std::cout << "4 - Relative Path" << std::endl;
        std::cout << "-----------------" << std::endl;
        std::cout << "Q - Quit" << std::endl;
        std::cout << "> ";

        std::getline(std::cin, CommandString);
        if (!std::cin.fail()) {
            // CommandString = StringUtils::Strip(CommandString);
            if (CommandString == "1") {
                //std::string TitleString;
                //std::cout << "Enter string> ";
                //std::getline(std::cin, TitleString);
                std::vector<std::string> x;
                x = StringUtils::Split("A  tougher\ttest\nto   pass!");
                //std::cout << "A  tougher\ttest\nto   pass!";
                std::cout << StringUtils::Join(" ", x);
                /*
                if(!std::cin.fail()){
                    std::cout<<"|"<<StringUtils::Center(std::string(" ") + StringUtils::Strip(TitleString) + std::string(" "),77,'*')<<"|"<<std::endl;   
                }*/
            } else if (CommandString == "2") {
                std::string String1, String2;
                std::cout << "Enter 1st string> ";
                std::getline(std::cin, String1);
                if (!std::cin.fail()) {
                    std::cout << "Enter 2nd string> ";
                    std::getline(std::cin, String2);
                    if (!std::cin.fail()) {
                        std::cout << "\"" << String1 << "\" requires " << StringUtils::EditDistance(String1, String2)
                                  << " edits to become \"" << String2 << "\"" << std::endl;
                    }
                }
            } else if (CommandString == "3") {
                CPath AbsPath("/usr/bin");
                std::cout << std::string(AbsPath.AbsolutePath());
                std::cout << std::string(CPath::AbsolutePath(AbsPath));
                /*std::string Path;
                std::cout << "Enter a path> ";
                std::getline(std::cin, Path);
                if (!std::cin.fail()) {
                    std::cout << "The absolute path of \"" << Path << "\" is \""
                              << std::string(CPath::AbsolutePath(CPath(Path))) << "\"" << std::endl;
                }*/
            } else if (CommandString == "4") {
                std::string Path;
                std::cout << "Enter a path> ";
                std::getline(std::cin, Path);
                if (!std::cin.fail()) {
                    std::cout << "The relative path of \"" << Path << "\" is \""
                              << std::string(CPath::RelativePath(CPath(Path))) << "\"" << std::endl;
                }
            } else if ((CommandString == "Q") or (CommandString == "q")) {
                Done = true;
            }
        }

    } while (!Done);


    return EXIT_SUCCESS;
}
