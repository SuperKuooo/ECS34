#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <sstream>
#include "../include/XMLReader.h"
#include "../include/XMLWriter.h"
#include "../include/XMLEntity.h"
#include "../include/CSVReader.h"
#include "../include/CSVWriter.h"


TEST(XMLTest, CorrectReadTest) {
    std::stringstream xml_ss("<course SUBJ=\"ABC\" CRSE=\"114\" SEC=\"2\">\n"
                             "    <student SEAT=\"1\" SID=\"409337710\" SURNAME=\"White\" PREFNAME=\"Victoria\" LEVEL=\"GR\" UNITS=\"12\" CLASS=\"D1\" MAJOR=\"ABCG\" GRADE=\"S\" STATUS=\"RE\" EMAIL=\"victoria.b.white@fakeu.edu\"></student>"
                             "</course>");
    CXMLReader xml_read(xml_ss);
    SXMLEntity read_data;

    //Reads the first element <course....>
    EXPECT_TRUE(xml_read.ReadEntity(read_data));
    EXPECT_FALSE(xml_read.End());
    EXPECT_EQ(read_data.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(read_data.DNameData, "course");

    EXPECT_TRUE(read_data.AttributeExists("SUBJ"));
    EXPECT_EQ(read_data.AttributeValue("SUBJ"), "ABC");

    EXPECT_TRUE(read_data.AttributeExists("SEC"));
    EXPECT_EQ(read_data.AttributeValue("SEC"), "2");

    EXPECT_TRUE(read_data.AttributeExists("CRSE"));
    EXPECT_EQ(read_data.AttributeValue("CRSE"), "114");

    EXPECT_FALSE(read_data.AttributeExists("HI NICK"));

    //Reads the second element <chardata>
    EXPECT_TRUE(xml_read.ReadEntity(read_data));
    EXPECT_FALSE(xml_read.End());
    EXPECT_EQ(read_data.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(read_data.DNameData, "\n");
    EXPECT_FALSE(read_data.AttributeExists("SUBJ"));

    //Reads the second element <chardata>
    EXPECT_TRUE(xml_read.ReadEntity(read_data));
    EXPECT_FALSE(xml_read.End());
    EXPECT_EQ(read_data.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(read_data.DNameData, "    ");
    EXPECT_FALSE(read_data.AttributeExists("SUBJ"));

    //Reads the student element <student>
    EXPECT_TRUE(xml_read.ReadEntity(read_data));
    EXPECT_FALSE(xml_read.End());
    EXPECT_EQ(read_data.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(read_data.DNameData, "student");

    EXPECT_TRUE(read_data.AttributeExists("SEAT"));
    EXPECT_EQ(read_data.AttributeValue("SEAT"), "1");

    EXPECT_TRUE(read_data.AttributeExists("SID"));
    EXPECT_EQ(read_data.AttributeValue("SID"), "409337710");

    EXPECT_TRUE(read_data.AttributeExists("STATUS"));
    EXPECT_EQ(read_data.AttributeValue("STATUS"), "RE");

    EXPECT_TRUE(read_data.AttributeExists("EMAIL"));
    EXPECT_EQ(read_data.AttributeValue("EMAIL"), "victoria.b.white@fakeu.edu");

    EXPECT_FALSE(read_data.AttributeExists("HI NICK"));
    EXPECT_FALSE(read_data.AttributeExists("SUBJ"));

    //Reads the end of the student element
    EXPECT_TRUE(xml_read.ReadEntity(read_data));
    EXPECT_FALSE(xml_read.End());
    EXPECT_EQ(read_data.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(read_data.DNameData, "student");
    EXPECT_FALSE(read_data.AttributeExists("SEAT"));

    //Reads the end of the course element
    EXPECT_TRUE(xml_read.ReadEntity(read_data));
    EXPECT_TRUE(xml_read.End());
    EXPECT_EQ(read_data.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(read_data.DNameData, "course");
    EXPECT_FALSE(read_data.AttributeExists("SEAT"));

    //Expect false since all lines have been read
    EXPECT_FALSE(xml_read.ReadEntity(read_data));
    EXPECT_TRUE(xml_read.End());

    std::stringstream xml_ss2("<course SUBJ=\"ABC\" CRSE=\"376\" SEC=\"36\">\n"
                              "    <student SEAT=\"1\" SID=\"409337710\" SURNAME=\"White\" PREFNAME=\"Victoria\" LEVEL=\"GR\" UNITS=\"12\" CLASS=\"D1\" MAJOR=\"ABCG\" GRADE=\"S\" STATUS=\"RE\" EMAIL=\"victoria.b.white@fakeu.edu\"/>\n"
                              "    <student SEAT=\"2\" SID=\"997678589\" SURNAME=\"Carroll\" PREFNAME=\"Jackson\" LEVEL=\"GR\" UNITS=\"12\" CLASS=\"D2\" MAJOR=\"ABCG\" GRADE=\"S\" STATUS=\"RE\" EMAIL=\"jackson.a.carroll@fakeu.edu\"/>\n"
                              "    <student SEAT=\"3\" SID=\"719554701\" SURNAME=\"English\" PREFNAME=\"Vincent\" LEVEL=\"GR\" UNITS=\"12\" CLASS=\"D1\" MAJOR=\"OT78\" GRADE=\"I\" STATUS=\"RE\" EMAIL=\"vincent.a.english@fakeu.edu\"/>\n"
                              "    <student SEAT=\"4\" SID=\"341825746\" SURNAME=\"Henderson\" PREFNAME=\"Caleb\" LEVEL=\"GR\" UNITS=\"8\" CLASS=\"D1\" MAJOR=\"ABCG\" GRADE=\"S\" STATUS=\"RE\" EMAIL=\"caleb.l.henderson@fakeu.edu\"/>\n"
                              "</course>");
    CXMLReader xml_read2(xml_ss2);
    SXMLEntity read_data2;

    EXPECT_TRUE(xml_read2.ReadEntity(read_data2, true));
    EXPECT_FALSE(xml_read2.End());
    EXPECT_EQ(read_data2.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(read_data2.DNameData, "course");

    EXPECT_TRUE(read_data2.AttributeExists("SUBJ"));
    EXPECT_EQ(read_data2.AttributeValue("SUBJ"), "ABC");

    EXPECT_TRUE(read_data2.AttributeExists("SEC"));
    EXPECT_EQ(read_data2.AttributeValue("SEC"), "36");

    EXPECT_FALSE(read_data2.AttributeExists("HI NICK"));

    EXPECT_TRUE(xml_read2.ReadEntity(read_data2, true));
    EXPECT_FALSE(xml_read2.End());
    EXPECT_EQ(read_data2.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(read_data2.DNameData, "student");

    EXPECT_TRUE(read_data2.AttributeExists("UNITS"));
    EXPECT_EQ(read_data2.AttributeValue("UNITS"), "12");

    EXPECT_TRUE(xml_read2.ReadEntity(read_data2, true));
    EXPECT_FALSE(xml_read2.End());
    EXPECT_EQ(read_data2.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(read_data2.DNameData, "student");
    EXPECT_FALSE(read_data2.AttributeExists("SEAT"));

    for (int i = 0; i < 6; i++) {
        EXPECT_TRUE(xml_read2.ReadEntity(read_data2, true));
    }

    EXPECT_TRUE(xml_read2.ReadEntity(read_data2, true));
    EXPECT_TRUE(xml_read2.End());
    EXPECT_EQ(read_data2.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(read_data2.DNameData, "course");


    std::stringstream xml_ss3("<course SUBJ=\">BC\" CRSE=\"&lt;114\" SEC=\"&quot;2\">\n"
                              "jerry Heyt&amp;&quot;&lt;\n"
                              "</course>");
    CXMLReader xml_read3(xml_ss3);
    SXMLEntity read_data3;

    EXPECT_TRUE(xml_read3.ReadEntity(read_data3, true));
    EXPECT_EQ(read_data3.DNameData, "course");
    EXPECT_EQ(read_data3.DNameData, "course");
    EXPECT_EQ(read_data3.AttributeValue("SUBJ"), ">BC");
    EXPECT_EQ(read_data3.AttributeValue("CRSE"), "<114");
    EXPECT_EQ(read_data3.AttributeValue("SEC"), "\"2");

    EXPECT_TRUE(xml_read3.ReadEntity(read_data3));
    EXPECT_EQ(read_data3.DNameData, "\n");
    EXPECT_EQ(read_data3.DType, SXMLEntity::EType::CharData);

    EXPECT_TRUE(xml_read3.ReadEntity(read_data3));
    EXPECT_EQ(read_data3.DNameData, "jerry Heyt");
    EXPECT_EQ(read_data3.DType, SXMLEntity::EType::CharData);

    EXPECT_TRUE(xml_read3.ReadEntity(read_data3));
    EXPECT_EQ(read_data3.DNameData, "&");
    EXPECT_EQ(read_data3.DType, SXMLEntity::EType::CharData);

    EXPECT_TRUE(xml_read3.ReadEntity(read_data3));
    EXPECT_EQ(read_data3.DNameData, "\"");
    EXPECT_EQ(read_data3.DType, SXMLEntity::EType::CharData);


    EXPECT_TRUE(xml_read3.ReadEntity(read_data3));
    EXPECT_EQ(read_data3.DNameData, "<");
    EXPECT_EQ(read_data3.DType, SXMLEntity::EType::CharData);

    EXPECT_TRUE(xml_read3.ReadEntity(read_data3));
    EXPECT_EQ(read_data3.DNameData, "\n");
    EXPECT_EQ(read_data3.DType, SXMLEntity::EType::CharData);
}

TEST(XMLTest, CorrectWriteTest) {
    std::string hey = "<course SUBJ=\">BC\" CRSE=\"&lt;114\" SEC=\"&quot;2\">\n"
                      "jerry Heyt&amp;&quot;&lt;\n"
                      "    <student SEAT=\"1\" SID=\"409337710\" SURNAME=\"White\" PREFNAME=\"Victoria\" LEVEL=\"GR\" UNITS=\"12\" CLASS=\"D1\" MAJOR=\"ABCG\" GRADE=\"S\" STATUS=\"RE\" EMAIL=\"victoria.b.white@fakeu.edu\"/>\n"
                      "</course>";
    std::stringstream xml_out;
    std::stringstream xml_in(hey);
    CXMLReader xml_read(xml_in);
    CXMLWriter xml_write(xml_out);
    SXMLEntity read_data;

    EXPECT_TRUE(xml_read.ReadEntity(read_data, true));
    EXPECT_TRUE(xml_write.WriteEntity(read_data));

    EXPECT_TRUE(xml_read.ReadEntity(read_data));
    EXPECT_TRUE(xml_write.WriteEntity(read_data));

    EXPECT_TRUE(xml_read.ReadEntity(read_data));
    EXPECT_TRUE(xml_write.WriteEntity(read_data));

    EXPECT_TRUE(xml_read.ReadEntity(read_data));
    EXPECT_TRUE(xml_write.WriteEntity(read_data));

    EXPECT_TRUE(xml_read.ReadEntity(read_data));
    EXPECT_TRUE(xml_write.WriteEntity(read_data));

    EXPECT_TRUE(xml_read.ReadEntity(read_data));
    EXPECT_TRUE(xml_write.WriteEntity(read_data));
    
    EXPECT_TRUE(xml_read.ReadEntity(read_data));
    EXPECT_TRUE(xml_write.WriteEntity(read_data));

    EXPECT_TRUE(xml_read.ReadEntity(read_data,true));
    read_data.DType = SXMLEntity::EType::CompleteElement;
    EXPECT_TRUE(xml_write.WriteEntity(read_data));

    EXPECT_FALSE(xml_read.End());
    EXPECT_EQ(xml_out.str(), "<course SUBJ=\"&gt;BC\" CRSE=\"&lt;114\" SEC=\"&quot;2\">\n"
                             "jerry Heyt&amp;&quot;&lt;\n"
                             "<student SEAT=\"1\" SID=\"409337710\" SURNAME=\"White\" PREFNAME=\"Victoria\" LEVEL=\"GR\" UNITS=\"12\" CLASS=\"D1\" MAJOR=\"ABCG\" GRADE=\"S\" STATUS=\"RE\" EMAIL=\"victoria.b.white@fakeu.edu\"/>");
}