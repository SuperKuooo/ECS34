#ifndef XMLWRITER_H
#define XMLWRITER_H

#include "XMLEntity.h"
#include <stack>
#include <istream>
#include <vector>

class CXMLWriter {
    private:
        std::ostream &output;
        std::vector<std::string> flush_element;

    public:
        CXMLWriter(std::ostream &os);
        ~CXMLWriter();
        bool Flush();
        bool WriteEntity(const SXMLEntity &entity);
};

#endif
