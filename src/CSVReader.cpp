#include "../include/CSVReader.h"


#define BUFFER_SIZE 16384

CCSVReader::CCSVReader(std::istream &in) : input(in) {
    csv_init(&Parser, 0);
}

CCSVReader::~CCSVReader() {
    csv_free(&Parser);
}

void CCSVReader::CallbackField(void *str, size_t len, void *data) {
    auto s = std::string(static_cast<char *>(str), len);
    auto *ptr = static_cast<CCSVReader *>(data);
    ptr->RowBuffer.push_back(s);
}

void CCSVReader::CallbackRow(int ch, void *data) {
    CCSVReader *ptr = static_cast<CCSVReader *>(data);
    ptr->BufferedRows.push(ptr->RowBuffer);
    ptr->RowBuffer.clear();
}

bool CCSVReader::End() const {
    return BufferedRows.empty() and parsed;
}

bool CCSVReader::ReadRow(std::vector<std::string> &row) {
    char read_row[BUFFER_SIZE];

    input.read(read_row, BUFFER_SIZE);
    if (!parsed) {
        csv_parse(&Parser, read_row, input.gcount(), CallbackField, CallbackRow, this);
        csv_fini(&Parser, CallbackField, CallbackRow, this);
        tot_line = BufferedRows.size();
        parsed = true;
    }
    //double check return type and what to return.
    if (!BufferedRows.empty()) {
        row.clear();
        row = BufferedRows.front();
        BufferedRows.pop();
        return true;
    } else {
        return false;
    }
}