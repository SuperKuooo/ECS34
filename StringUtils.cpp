#include "StringUtils.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstdio>

namespace StringUtils {

    std::string Slice(const std::string &str, ssize_t start, ssize_t end) {
        std::string temp;
        int counter = 0;
        int length = str.length();
        if (start < 0) {
            start += length;
        }
        if (end <= 0) {
            end += length;
        }
        for (char ch : str) {
            if (counter < start) {
                counter++;
                continue;
            } else if (start <= counter && counter < end) {
                counter++;
                temp += ch;
            } else {
                break;
            }
        }
        return temp;
    }

    std::string Capitalize(const std::string &str) {
        std::string temp;
        for (char ch : str) {
            temp += tolower(ch);
        }
        temp[0] = toupper(temp[0]);
        return temp;
    }

    std::string Title(const std::string &str) {
        std::string temp, lower_case;
        char prev;
        bool first = true;
        // First do a little bit a input manipulation.
        for (char ch : str) {
            lower_case += tolower(ch);
        }
        for (char ch: lower_case) {
            if (first) {
                first = false;
                prev = ch;
                temp += toupper(ch);
                continue;
            }
            if (ispunct(prev) or prev == ' ' or !isalpha(prev)) {
                prev = ch;
                temp += toupper(ch);
                continue;
            }
            temp += ch;
            prev = ch;
        }
        return temp;
    }

    std::string LStrip(const std::string &str) {
        std::string temp;
        bool stripped = true;
        for (char ch : str) {
            if (isblank(ch) and stripped) {
                continue;
            } else {
                stripped = false;
                temp += ch;
            }
        }
        return temp;
    }

    std::string RStrip(const std::string &str) {
        std::string temp;
        int length = str.length() - 1;
        temp = str;
        for (int counter = length; counter >= 0; counter--) {
            if (isblank(temp[counter])) {
                temp.erase(counter, 1);
            } else {
                break;
            }
        }
        return temp;
    }

    std::string Strip(const std::string &str) {
        std::string temp;
        temp = LStrip(str);
        temp = RStrip(temp);
        return temp;
    }

    std::string Center(const std::string &str, int width, char fill) {
        std::string temp;
        temp = Strip(str);
        int length = temp.length() + 1;
        if ((width - temp.length()) % 2) {
            temp += fill;
        }
        for (int counter = length; counter < width; counter += 2) {
            temp += fill;
            temp = fill + temp;
        }
        return temp;
    }

    std::string LJust(const std::string &str, int width, char fill) {
        std::string temp;
        temp = Strip(str);
        int length = temp.length();
        for (int counter = length; counter < width; counter++) {
            temp += fill;
        }
        return temp;
    }

    std::string RJust(const std::string &str, int width, char fill) {
        std::string temp, just;
        temp = Strip(str);
        int length = temp.length();
        for (int counter = length; counter < width; counter++) {
            temp = fill + temp;
        }

        return temp;
    }

    std::string Replace(const std::string &str, const std::string &old, const std::string &rep) {
        std::string temp;
        int replace = str.find(old);
        temp = str;
        while (replace != std::string::npos) {
            temp.replace(replace, rep.length(), rep);
            replace = temp.find(old);
            // Inspired by https://stackoverflow.com/questions/15740829/string-find-doesnt-return-1b
        }
        return temp;
    }

    std::vector<std::string> Split(const std::string &str, const std::string &splt) {
        std::vector<std::string> x;
        std::string temp;

        int div = str.find(splt), div2;
        temp = str;
        while (true) {
            temp = &temp[div+1];
            div2 = temp.find(splt);
            if (div2 != std::string::npos) {
                x.push_back(Slice(temp, 0, div2));
                std::cout << x.size();
                div = div2;
            } else{
                x.push_back(temp);
                break;
            }
        }
        return x;
    }

    std::string Join(const std::string &str, const std::vector<std::string> &vect) {
        // Your code goes here
    }

    std::string ExpandTabs(const std::string &str, int tabsize) {
        // Your code goes here
    }

    int EditDistance(const std::string &left, const std::string &right, bool ignorecase) {
        // Your code goes here
    }

}
