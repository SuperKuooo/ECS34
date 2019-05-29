#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <sstream>
#include "../include/XMLReader.h"
#include "../include/XMLWriter.h"
#include "../include/XMLEntity.h"
#include "../include/CSVReader.h"
#include "../include/CSVWriter.h"

TEST(CSVTest, CorrectReadTest) {
    //1
    std::stringstream csv_ss(" 1,2 ,  3         ,4,5\x0d\x0a");
    std::vector<std::string> row_data;
    CCSVReader csv_read(csv_ss);
    EXPECT_TRUE(csv_read.ReadRow(row_data));
    EXPECT_EQ(row_data.size(), 5);
    if(row_data.size() >= 5){
        EXPECT_EQ(row_data[0], "1");
        EXPECT_EQ(row_data[1], "2");
        EXPECT_EQ(row_data[2], "3");
        EXPECT_EQ(row_data[3], "4");
        EXPECT_EQ(row_data[4], "5");
    }
    EXPECT_FALSE(csv_read.ReadRow(row_data));
    EXPECT_TRUE(csv_read.End());

    //2
    std::stringstream csv_ss2(",,,,,\x0a");
    std::vector<std::string> row_data2;
    CCSVReader csv_read2(csv_ss2);
    EXPECT_TRUE(csv_read2.ReadRow(row_data2));
    EXPECT_EQ(row_data2[0], "");
    EXPECT_EQ(row_data2[1], "");
    EXPECT_EQ(row_data2[2], "");
    EXPECT_EQ(row_data2[3], "");
    EXPECT_EQ(row_data2[4], "");
    EXPECT_EQ(row_data2[5], "");
    EXPECT_TRUE(csv_read2.End());

    //5
    std::stringstream csv_ss3("\"\"\"a,b\"\"\",,\" \"\"\"\" \",\"\"\"\"\" \",\" \"\"\"\"\",\"\"\"\"\"\"");
    std::vector<std::string> row_data3;
    CCSVReader csv_read3(csv_ss3);
    EXPECT_TRUE(csv_read3.ReadRow(row_data3));
    EXPECT_EQ(row_data3[0], "\"a,b\"");
    EXPECT_EQ(row_data3[1], "");
    EXPECT_EQ(row_data3[2], " \"\" ");
    EXPECT_EQ(row_data3[3], "\"\" ");
    EXPECT_EQ(row_data3[4], " \"\"");
    EXPECT_EQ(row_data3[5], "\"\"");
    EXPECT_TRUE(csv_read3.End());

    //12
    std::stringstream csv_ss4("\x0a\x0a\x0a\x0a");
    std::vector<std::string> row_data4;
    CCSVReader csv_read4(csv_ss4);
    EXPECT_FALSE(csv_read4.ReadRow(row_data4));
    EXPECT_TRUE(csv_read4.End());

    //14
    std::stringstream csv_ss5("1, 2, 3,\x0a\x0d\x0a  \"4\", \x0d,");
    std::vector<std::string> row_data5;
    CCSVReader csv_read5(csv_ss5);
    EXPECT_TRUE(csv_read5.ReadRow(row_data5));
    EXPECT_EQ(row_data5[0], "1");
    EXPECT_EQ(row_data5[1], "2");
    EXPECT_EQ(row_data5[2], "3");
    EXPECT_EQ(row_data5[3], "");
    EXPECT_FALSE(csv_read5.End());
    EXPECT_TRUE(csv_read5.ReadRow(row_data5));
    EXPECT_EQ(row_data5[0], "4");
    EXPECT_EQ(row_data5[1], "");
    EXPECT_FALSE(csv_read5.End());
    EXPECT_TRUE(csv_read5.ReadRow(row_data5));
    EXPECT_EQ(row_data5[0], "");
    EXPECT_EQ(row_data5[1], "");
    EXPECT_TRUE(csv_read5.End());

    //17
    std::stringstream csv_ss6(" a\0b\0c ");
    std::vector<std::string> row_data6;
    CCSVReader csv_read6(csv_ss6);
    EXPECT_TRUE(csv_read6.ReadRow(row_data6));
    EXPECT_EQ(row_data6[0], "a\0b\0c");
    EXPECT_TRUE(csv_read6.End());

    //4
    std::stringstream csv_ss7("\"I call our world Flatland,\x0a"
                              "not because we call it so,\x0a"
                              "but to make its nature clearer\x0a"
                              "to you, my happy readers,\x0a"
                              "who are privileged to live in Space.\"");
    std::vector<std::string> row_data7;
    CCSVReader csv_read7(csv_ss7);
    EXPECT_TRUE(csv_read7.ReadRow(row_data7));
    EXPECT_EQ(row_data7[0], "I call our world Flatland,\x0a"
                            "not because we call it so,\x0a"
                            "but to make its nature clearer\x0a"
                            "to you, my happy readers,\x0a"
                            "who are privileged to live in Space.");
    EXPECT_TRUE(csv_read7.End());

    //8
    std::stringstream csv_ss8("\" abc\"                                             "
                              "                                                     "
                              "                                                     "
                              "                                                     "
                              "                                                     "
                              "                                                     "
                              "                                                     "
                              "                                                     "
                              "                                          \", \"123\"");
    std::vector<std::string> row_data8;
    CCSVReader csv_read8(csv_ss8);
    EXPECT_TRUE(csv_read8.ReadRow(row_data8));
    EXPECT_EQ(row_data8[0], " abc\"                                               "
                            "                                                     "
                            "                                                     "
                            "                                                     "
                            "                                                     "
                            "                                                     "
                            "                                                     "
                            "                                                     "
                            "                                        ");
    EXPECT_EQ(row_data8[1], "123");
    EXPECT_TRUE(csv_read8.End());

}

TEST(CSVTest, CorrectWriteTest) { //passed
    std::stringstream csv_ss;
    std::vector<std::string> row_data{"abc"};
    CCSVWriter csv_write(csv_ss);
    EXPECT_TRUE(csv_write.WriteRow(row_data));
    EXPECT_EQ(csv_ss.str(), "\"abc\"\n");

    std::stringstream csv_ss2;
    std::vector<std::string> row_data2{"\"\"\"\"\"\"\"\""};
    CCSVWriter csv_write2(csv_ss2);
    EXPECT_TRUE(csv_write2.WriteRow(row_data2));
    EXPECT_EQ(csv_ss2.str(), "\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\n");

    std::stringstream csv_ss3;
    std::vector<std::string> row_data3{"1","2","3","4","5"};
    CCSVWriter csv_write3(csv_ss3);
    EXPECT_TRUE(csv_write3.WriteRow(row_data3));
    EXPECT_EQ(csv_ss3.str(), "\"1\",\"2\",\"3\",\"4\",\"5\"\n");

}
