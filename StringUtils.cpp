#include "StringUtils.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstdio>

/* 1. Can we make a copy of the string? Or what is better way of doing this.
 * 2. Difference between char &ch:temp and char ch:temp
 *
 *
 */

namespace StringUtils {
    std::string Slice(const std::string &str, ssize_t start, ssize_t end) {
        if (start == end) {
            return "";
        }
        if (start < 0) {
            start += str.length();
        }
        if (end <= 0) {
            end += str.length();
        }
        ssize_t slice_length = end - start;
        std::string temp = str;
        if (slice_length <= 0) {
            return "";
        }
        return temp.substr(start, slice_length);
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
        std::string temp = str;
        if (splt.empty()) {
            temp = StringUtils::Replace(temp, "\n", " ");
            temp = StringUtils::Replace(temp, "\t", " ");
            temp = StringUtils::Replace(temp, "\r", " ");
            temp = StringUtils::Replace(temp, "\b", " ");
            int counter = 0;
            for (char &ch: temp) {
                if (ch == ' ' and *(&ch + 1) == ' ') {
                    int length = 1;
                    while (*(&ch + length) == ' ') {
                        length++;
                    }
                    temp.erase(counter, length - 1);
                }
                counter++;
            }
            return StringUtils::Split(temp, " ");
        } else {
            int div = str.find(splt), div2;
            x.push_back(Slice(temp, 0, div));
            while (true) {
                temp = &temp[div + 1];
                div2 = temp.find(splt);
                if (div2 != std::string::npos) {
                    x.push_back(Slice(temp, 0, div2));
                    div = div2;
                } else {
                    x.push_back(temp);
                    break;
                }
            }
            return x;
        }

    }

    std::string Join(const std::string &str, const std::vector<std::string> &vect) {
        std::string temp;
        int counter = 0;
        for (auto cont : vect) {
            counter++;
            if (counter >= vect.size())
                temp += cont;
            else
                temp += cont + str;
        }
        return temp;
    }

    std::string ExpandTabs(const std::string &str, int tabsize) {
        std::vector<std::string> expanded = StringUtils::Split(str, "\t");
        int counter = 0;
        for (auto &comp : expanded) {
            counter++;
            if (counter == expanded.size()) {
                break;
            }
            if (comp.length() >= tabsize) {
                comp = StringUtils::LJust(comp, tabsize * 2);
            } else
                comp = StringUtils::LJust(comp, tabsize);
        }
        return StringUtils::Join("", expanded);
    }


    int EditDistance(const std::string &A, const std::string &B, bool ignorecase) {
        size_t NA = A.size();
        size_t NB = B.size();

        std::vector<std::vector<size_t>> M(NA + 1, std::vector<size_t>(NB + 1));

        for (size_t a = 0; a <= NA; ++a)
            M[a][0] = a;

        for (size_t b = 0; b <= NB; ++b)
            M[0][b] = b;

        for (size_t a = 1; a <= NA; ++a)
            for (size_t b = 1; b <= NB; ++b) {
                size_t x = M[a - 1][b] + 1;
                size_t y = M[a][b - 1] + 1;
                size_t z = M[a - 1][b - 1] + (A[a - 1] == B[b - 1] ? 0 : 2);
                if (x < y)
                    M[a][b] = x < z ? x : z;
                else
                    M[a][b] = y < z ? y : z;
            }
        return M[A.size()][B.size()];
    }

}
