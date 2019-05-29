#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <ostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <queue>
#include "../lib/csv.h"
#include <iostream>

class CCSVWriter {
protected:
    std::ostream &output;
public:
    CCSVWriter(std::ostream &ou);

    ~CCSVWriter();

    bool WriteRow(const std::vector<std::string> &row);
};

#endif
