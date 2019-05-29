#ifndef XMLREADER_H
#define XMLREADER_H

#include "XMLEntity.h"
#include <istream>
#include <queue>
#include <expat.h>
#include <vector>
#include <cstring>

class CXMLReader {
private:
    bool parsed = false;
    XML_Parser parser;
    std::istream &input;
    std::queue<SXMLEntity> buffered_XML;

    static void read_start(void *data, const char *element, const char *attribute[]);
    static void read_end(void *data, const char *el);
    static void read_text(void *data, const char *content, int length);

public:
    CXMLReader(std::istream &is);

    ~CXMLReader();

    bool End();

    bool ReadEntity(SXMLEntity &entity, bool skipcdata = false);
};

#endif
