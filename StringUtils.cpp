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
        std::string temp = str;
        int length = str.length() - 1;
        for (int counter = length; counter >= 0; counter--) {
            if (isblank(temp[counter]) or temp[counter] == '\n') {
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
        std::string temp = str;
        size_t replace = str.find(old);
        while (replace != std::string::npos) {
            temp.replace(replace, old.length(), rep);
            replace = temp.find(old);
            // Inspired by https://stackoverflow.com/questions/15740829/string-find-doesnt-return-1b
        }
        return temp;
    }

    std::vector<std::string> Split(const std::string &str, const std::string &splt) {
        std::vector<std::string> x;
        std::string temp = str;
        if (str.find("/") == std::string::npos) {
            x.push_back(str);
            return x;
        }
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
            size_t div = str.find(splt), div2;
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
        size_t counter = 0;
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
        size_t counter = 0;
        for (auto &comp : expanded) {
            counter++;
            if (counter == expanded.size()) {
                break;
            }
            if (int(comp.length()) >= tabsize) {
                comp = StringUtils::LJust(comp, tabsize * 2);
            } else
                comp = StringUtils::LJust(comp, tabsize);
        }
        return StringUtils::Join("", expanded);
    }


    int EditDistance(const std::string &left, const std::string &right, bool ignorecase) {
        const std::size_t len1 = left.size(), len2 = right.size();
        std::vector<std::vector<unsigned int>> matrix(len1 + 1, std::vector<unsigned int>(len2 + 1));
        std::string _left, _right;

        if (ignorecase) {
            for (char ch: left) {
                _left += ::toupper(ch);
            }
            for (char ch: right) {
                _right += ::toupper(ch);
            }
        } else {
            _left = left;
            _right = right;
        }

        matrix[0][0] = 0;
        for (int i = 1; i <= len1; ++i) {
            matrix[i][0] = i;
        }
        for (int i = 1; i <= len2; ++i) {
            matrix[0][i] = i;
        }

        for (int i = 1; i <= len1; ++i) {
            for (int j = 1; j <= len2; ++j) {
                matrix[i][j] = std::min({matrix[i - 1][j] + 1, matrix[i][j - 1] + 1,
                                         matrix[i - 1][j - 1] + (_left[i - 1] == _right[j - 1] ? 0 : 1)});

            }
        }

        return matrix[len1][len2];
    }


}