#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include "../lib/csv.h"

class CCSVReader {
protected:
    std::istream &input;
    struct csv_parser Parser;
    std::queue<std::vector<std::string>> BufferedRows;
    std::vector<std::string> RowBuffer;
    bool parsed = false;

    static void CallbackField(void *str, size_t len, void *data);

    static void CallbackRow(int ch, void *data);

public:
    int tot_line;

    CCSVReader(std::istream &in);

    ~CCSVReader();

    bool End() const;

    bool ReadRow(std::vector<std::string> &row);
};

#endif
