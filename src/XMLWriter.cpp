#include "../include/XMLEntity.h"
#include "../include/XMLWriter.h"
#include "iostream"
#include <expat.h>

#define BUFFER_SIZE 1024

CXMLWriter::CXMLWriter(std::ostream &ou) : output(ou) {
    //nothing to initiate
}

CXMLWriter::~CXMLWriter() {
    //Don't have to close ofstream explicitly
    //https://stackoverflow.com/questions/23919912/how-to-close-ofstream-after-assigning-to-ostream?lq=1
}

bool CXMLWriter::Flush() {
    if (flush_element.empty())
        return false;
    for (auto header: this->flush_element) {
        output << "\n</" + header + ">";
    }
    return true;
}

std::string start_element_write(SXMLEntity entity) {
    std::string hi_chu_hung;

    hi_chu_hung += "<" + entity.DNameData;
    for (auto &attributes: entity.DAttributes) {
        for (size_t i = 0; i < attributes.second.length(); i++) {
            if (attributes.second[i] == '&') {
                attributes.second.insert(i + 1, "&amp;");
                attributes.second.erase(i, 1);
            }
        }
        for (size_t i = 0; i < attributes.second.length(); i++) {
            if (attributes.second[i] == '<') {
                attributes.second.insert(i + 1, "&lt;");
                attributes.second.erase(i, 1);
            } else if (attributes.second[i] == '>') {
                attributes.second.insert(i + 1, "&gt;");
                attributes.second.erase(i, 1);
            } else if (attributes.second[i] == '"') {
                attributes.second.insert(i + 1, "&quot;");
                attributes.second.erase(i, 1);
            } else if (attributes.second[i] == '\'') {
                attributes.second.insert(i + 1, "&apos;");
                attributes.second.erase(i, 1);
            }
        }
        hi_chu_hung += " " + attributes.first + "=" + '"' + attributes.second + '"';
    }
    hi_chu_hung += ">";
    return hi_chu_hung;
}

std::string end_element_write(const SXMLEntity &entity) {
    std::string hi_chu_hung;

    hi_chu_hung += "</" + entity.DNameData + ">";
    return hi_chu_hung;
}

std::string char_data_write(const SXMLEntity &entity) {
    std::string hi_chu_hung;
    std::string normalized = entity.DNameData;

    for (size_t i = 0; i < normalized.length(); i++) {
        if (normalized[i] == '&') {
            normalized.insert(i + 1, "&amp;");
            normalized.erase(i, 1);
        }
    }
    for (size_t i = 0; i < normalized.length(); i++) {
        if (normalized[i] == '<') {
            normalized.insert(i + 1, "&lt;");
            normalized.erase(i, 1);
        } else if (normalized[i] == '>') {
            normalized.insert(i + 1, "&gt;");
            normalized.erase(i, 1);
        } else if (normalized[i] == '"') {
            normalized.insert(i + 1, "&quot;");
            normalized.erase(i, 1);
        } else if (normalized[i] == '\'') {
            normalized.insert(i + 1, "&apos;");
            normalized.erase(i, 1);
        }

    }
    hi_chu_hung += normalized;

    return hi_chu_hung;
}

std::string complete_element_write(const SXMLEntity &entity) {
    std::string hi_chu_hung;
    hi_chu_hung += "<" + entity.DNameData;
    for (auto attributes: entity.DAttributes) {
        for (size_t i = 0; i < attributes.second.length(); i++) {
            if (attributes.second[i] == '&') {
                attributes.second.insert(i + 1, "&amp;");
                attributes.second.erase(i, 1);
            }
        }
        for (size_t i = 0; i < attributes.second.length(); i++) {
            if (attributes.second[i] == '<') {
                attributes.second.insert(i + 1, "&lt;");
                attributes.second.erase(i, 1);
            } else if (attributes.second[i] == '>') {
                attributes.second.insert(i + 1, "&gt;");
                attributes.second.erase(i, 1);
            } else if (attributes.second[i] == '"') {
                attributes.second.insert(i + 1, "&quot;");
                attributes.second.erase(i, 1);
            } else if (attributes.second[i] == '\'') {
                attributes.second.insert(i + 1, "&apos;");
                attributes.second.erase(i, 1);
            }
        }
        hi_chu_hung += " " + attributes.first + "=" + '"' + attributes.second + '"';
    }
    hi_chu_hung += "/>";
    return hi_chu_hung;
}

bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    std::string david;

    switch (entity.DType) {
        case SXMLEntity::EType::StartElement:
            flush_element.push_back(entity.DNameData);
            david = start_element_write(entity);
            break;
        case SXMLEntity::EType::EndElement:
            if (!flush_element.empty())
                flush_element.pop_back();
            david = end_element_write(entity);
            break;
        case SXMLEntity::EType::CharData:
            david = char_data_write(entity);
            break;
        case SXMLEntity::EType::CompleteElement:
            david = complete_element_write(entity);
            break;
    }

    output << david;
    return true;
}






