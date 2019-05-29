//
// Created by Jerry Kuo on 2019-05-19.
//
#include "../include/XMLEntity.h"
#include "../include/XMLReader.h"
#include "iostream"
#include <expat.h>

#define BUFFER_SIZE 1024

CXMLReader::CXMLReader(std::istream &is) : input(is) {
    parser = XML_ParserCreate(nullptr);

    //Set the parser handling functions
    XML_SetUserData(parser, this);
    XML_SetElementHandler(parser, read_start, read_end);
    XML_SetCharacterDataHandler(parser, read_text);
}

CXMLReader::~CXMLReader() {
    XML_ParserFree(parser);
}

void CXMLReader::read_start(void *data, const char *element, const char *attribute[]) {
    int i = 0;
    auto *ptr = static_cast<CXMLReader *>(data);
    SXMLEntity xml_element;
    xml_element.DType = SXMLEntity::EType::StartElement;
    xml_element.DNameData = std::string(element);

    for (i = 0; attribute[0] && attribute[i + 2]; i += 2) {
        xml_element.SetAttribute(std::string(attribute[i]), std::string(attribute[i + 1]));
    }
    if (attribute[i])
        xml_element.SetAttribute(std::string(attribute[i]), std::string(attribute[i + 1]));


    ptr->buffered_XML.push(xml_element);
}

void CXMLReader::read_end(void *data, const char *el) {
    auto *ptr = static_cast<CXMLReader *>(data);
    SXMLEntity xml_element;
    xml_element.DType = SXMLEntity::EType::EndElement;
    xml_element.DNameData = std::string(el);
    ptr->buffered_XML.push(xml_element);
}

void CXMLReader::read_text(void *data, const char *content, int length) {
    auto *ptr = static_cast<CXMLReader *>(data);
    SXMLEntity xml_element;
    xml_element.DType = SXMLEntity::EType::CharData;
    xml_element.DNameData = std::string(content, length);
    ptr->buffered_XML.push(xml_element);
}

bool CXMLReader::End() {
    return buffered_XML.empty() and parsed;
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    if (!parsed) {
        while(!input.eof()){
            char c_xml[BUFFER_SIZE] = "";
            input.read(c_xml, BUFFER_SIZE);
            XML_Parse(parser, c_xml, input.gcount(), input.eof());
            parsed = true;
        }
    }
    if (!buffered_XML.empty()) {
        if (skipcdata) {
            while (buffered_XML.front().DType == SXMLEntity::EType::CharData) {
                buffered_XML.pop();
            }
        }
        entity = buffered_XML.front();
        buffered_XML.pop();
        return true;
    } else {
        return false;
    }
}




