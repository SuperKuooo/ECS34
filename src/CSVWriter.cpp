//#include <bits/ios_base.h>
#include "../include/CSVWriter.h"

#define BUFFER_SIZE 1024

CCSVWriter::CCSVWriter(std::ostream &ou) : output(ou) {

}

CCSVWriter::~CCSVWriter() {

}


bool CCSVWriter::WriteRow(const std::vector<std::string> &row) {
    std::string result;
    char joined[BUFFER_SIZE] = "";
    int counter = 0, end = row.size();

    for (const std::string &element : row) {
        char temp[element.size()];
        strcpy(temp, element.c_str());
        csv_write(static_cast<void *>(&joined), BUFFER_SIZE, static_cast<void *>(temp), element.size());
        if (counter == end - 1)
            result += joined;
        else {
            result += joined;
            result += ",";
        }
        counter++;
        memset(joined, '\0', sizeof(joined));
    }
    result += "\n";
    output << result;

    return true;
}