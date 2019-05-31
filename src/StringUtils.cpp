#include "StringUtils.h"
#include <algorithm> 
#include <cctype>
#include <cstdio>
#include <sstream>

namespace StringUtils{
    
std::string Slice(const std::string &str, ssize_t start, ssize_t end){
    if (start < 0){
        start = start + str.length();
    }
    if (end <= 0){
        end = end + str.length();
    }
    if (end - start < 0){
        return "";
    } else {
        return str.substr(start, end - start);
    }
}

std::string Capitalize(const std::string &str){
    std::string str2;
    str2 += toupper(str[0]);
    for (size_t i = 1; i < str.length(); i++){
        str2 += tolower(str[i]);
    }
    return str2;
}

std::string Title(const std::string &str){
    std::string str2;
    str2 += toupper(str[0]);
    for (size_t i = 1; i < str.length(); i++){
        if (!isalpha(str[i-1])){
            str2 += toupper(str[i]);
        } else{
            str2 += tolower(str[i]);
        }
    }
    return str2;
}

std::string LStrip(const std::string &str){
    int pos = 0;
    for (size_t i = 0; i < str.length(); i++) {
        if (isspace(str[i])){
            pos ++;
        } else {
            break;
        }
    }
    return str.substr(pos, str.length() - pos);
}

std::string RStrip(const std::string &str) {
    int pos = str.length();
    for (size_t i = str.length(); i >= 0; i--) {
        if (isspace(str[i-1])) {
            pos --;
        } else {
            break;
        }
    }
    return str.substr(0, pos);
}

std::string Strip(const std::string &str){
    std::string str2;
    std::string str3;
    for (size_t i = 0;  i < str.length(); i++) {
        if (isspace(str[i])) {
            continue;
        } else {
            str2 += str.substr(i, str.length() - i);
            break;
        }
    }
    for (size_t i = str2.length(); i >= 0; i--) {
        if (isspace(str2[i-1])) {
            continue;
        } else {
            str3 += str2.substr(0, i);
            break;
        }
    }
    return str3;
}

std::string Center(const std::string &str, int width, char fill){
    std::string str2;
    int spaceL;
    int spaceR;
    spaceL = (width - str.length()) / 2;
    spaceR = (width - str.length()) - spaceL;
    for (int i = 0; i < spaceL; i++){
        str2 += fill;
    }
    str2 += str;
    for (int i = 0; i < spaceR; i++){
        str2 += fill;
    }
    return str2;
}

std::string LJust(const std::string &str, int width, char fill){
    std::string str2;
    int spaceR;
    spaceR = width - str.length();
    str2 += str;
    for (int i = 0; i < spaceR; i++){
        str2 += fill;
    }
    return str2;
}

std::string RJust(const std::string &str, int width, char fill){
    std::string str2;
    int spaceL;
    spaceL = width - str.length();
    for (int i = 0; i < spaceL; i++){
        str2 += fill;
    }
    str2 += str;
    return str2;
}

//https://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string
std::string Replace(const std::string &str, const std::string &old, const std::string &rep){
    std::string str2 = str;
    size_t pos = 0;
    while((pos = str.find(old, pos)) != std::string::npos){
        str2.replace(pos, old.length(), rep);
        pos += rep.length();
    }
    return str2;
}

std::vector< std::string > Split(const std::string &str, const std::string &splt){
    std::vector<std::string> list;
    std::string tempStore;
    if (splt.empty()){
        for (size_t i = 0; i < str.length(); i++) {
            if ((str.substr(i, 1) == " " || str.substr(i, 1) == "\t" || str.substr(i, 1) == "\r" || str.substr(i, 1) == "\n") && i == 0) {
                list.push_back("");
            } else if ((str.substr(i, 1) == " " || str.substr(i, 1) == "\t" || str.substr(i, 1) == "\r" || str.substr(i, 1) == "\n") && i != 0) {
                if (tempStore.empty()){
                    continue;
                } else {
                    list.push_back(tempStore);
                    tempStore.clear();
                }
            } else if (i == str.length() - 1) {
                tempStore += str.substr(i);
                list.push_back(tempStore);
            } else {
                tempStore += str.substr(i, 1);
            }
        }
    }
    else {
        for (size_t i = 0; i < str.length(); i++) {
            if (splt == str.substr(i, 1) && i == 0) {
                list.push_back("");
            } else if (splt == str.substr(i, 1) && i != 0) {
                list.push_back(tempStore);
                tempStore.clear();
            } else if (i == str.length() - 1) {
                tempStore += str.substr(i);
                list.push_back(tempStore);
            } else {
                tempStore += str.substr(i, 1);
            }
        }
    }
    return list;
}

std::string Join(const std::string &str, const std::vector< std::string > &vect){
    std::string str2;
    for (auto elem:vect){
        if (elem.empty()){
            continue;
        } else {
            str2 += str + elem;
        }
    }
    str2 = StringUtils::Strip(str2);
    return str2;
}

std::string ExpandTabs(const std::string &str, int tabsize){
    std::string str2;
    std::string rep;
    int doubletab = tabsize * 2;
    for (int i = 0; i < tabsize; i++){
        rep += " ";
    }
    std::vector<std::string> list;
    std::vector<std::string> list2;
    list = StringUtils::Split(str, "\t");
    if (tabsize == 0){
        str2 = StringUtils::Join("", list);
    } else {
        for (auto elem:list) {
            int elemSize = elem.size();
            if (elemSize == tabsize) {
                elem.append(rep);
            } else if (elemSize > tabsize) {
                while (doubletab <= elemSize) {
                    doubletab += tabsize;
                }
                for (int i = 0; i < (doubletab - elemSize); i++) {
                    elem.append(" ");
                }
            } else {
                while (elemSize < tabsize) {
                    elem.append(" ");
                    elemSize ++;
                }
            }
            list2.push_back(elem);
        }
        str2 = StringUtils::Join("", list2);
        str2 = StringUtils::Strip(str2);
    }
    return str2;
}

//helper function to calculate minimum of three numbers
int min(int x, int y, int z){
    int minVal = x;
    if (y < x) {
        minVal = y;
        if (z < y) {
            minVal = z;
        }
    } else if (z < x){
        minVal = z;
    }
    return minVal;
}

//https://www.geeksforgeeks.org/edit-distance-dp-5/
int EditDistance(const std::string &left, const std::string &right, bool ignorecase){
    int m = left.length();
    int n = right.length();
    int dp[m+1][n+1]; // Create a table to store results of subproblems
    if (ignorecase) {
        std::string left2;
        std::string right2;
        for (int i = 0; i < m; i++) {
            left2 += tolower(left[i]);
        }
        for (int i = 0; i < n; i++) {
            right2 += tolower(right[i]);
        }
        for (int i = 0; i <= m; i++){ // Fill d[][] in bottom up manner
            for ( int j = 0; j <= n; j++){
                if (i == 0){
                    // If first string is empty, only option is to
                    // insert all characters of second string
                    dp[i][j] = j; // Min. operations = j
                } else if (j == 0){
                    // If second string is empty, only option is to
                    // remove all characters of second string
                    dp[i][j] = i; // Min. operations = i
                } else if (left2[i-1] == right2[j-1]){
                    // If last characters are same, ignore last char
                    // and recur for remaining string
                    dp[i][j] = dp[i-1][j-1];
                } else {
                    // If the last character is different, consider all
                    // possibilities and find the minimum
                    dp[i][j] = 1 + min(dp[i][j-1],dp[i-1][j],dp[i-1][j-1]);
                }
            }
        }
    } else {
        for (int i = 0; i <= m; i++) {
            for (int j = 0; j <= n; j++) {
                if (i == 0) {
                    dp[i][j] = j;
                } else if (j == 0) {
                    dp[i][j] = i;
                } else if (left[i - 1] == right[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    dp[i][j] = 1 + min(dp[i][j - 1], dp[i - 1][j], dp[i - 1][j - 1]);
                }
            }
        }
    }
    return dp[m][n];
}

}
