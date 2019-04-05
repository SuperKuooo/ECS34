#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtilsTest, SliceTest){
    EXPECT_EQ(StringUtils::Slice("Hello World!",1), "ello World!");
    EXPECT_EQ(StringUtils::Slice("Hello World!",0,5), "Hello");
    EXPECT_EQ(StringUtils::Slice("Hello World!",0,-1), "Hello World");
    EXPECT_EQ(StringUtils::Slice("Hello World!",3,-2), "lo Worl");
    EXPECT_EQ(StringUtils::Slice("Hello World!",-5,-2), "orl");
    EXPECT_EQ(StringUtils::Slice("Hello World!",14), "");
}

TEST(StringUtilsTest, CaseTest){
    EXPECT_EQ(StringUtils::Capitalize("hello world!"), "Hello world!");
    EXPECT_EQ(StringUtils::Title("hello world!"), "Hello World!");
    EXPECT_EQ(StringUtils::Capitalize("HELLO WORLD!"), "Hello world!");
    EXPECT_EQ(StringUtils::Title("HELLO WORLD!"), "Hello World!");
    EXPECT_EQ(StringUtils::Capitalize(" HELLO WORLD!"), " hello world!");
    EXPECT_EQ(StringUtils::Title(" HELLO   WORLD!"), " Hello   World!");
    EXPECT_EQ(StringUtils::Title("definitely-a'tough'test2pass!"), "Definitely-A'Tough'Test2Pass!");
}

TEST(StringUtilsTest, SpaceTest){
    std::string Str1 = "    Test String    ";
    EXPECT_EQ(StringUtils::LStrip(Str1), "Test String    ");
    EXPECT_EQ(StringUtils::RStrip(Str1), "    Test String");
    EXPECT_EQ(StringUtils::Strip(Str1),StringUtils::RStrip(StringUtils::LStrip(Str1)));
    EXPECT_EQ(StringUtils::LJust("Test String",19), "Test String        ");
    EXPECT_EQ(StringUtils::RJust("Test String",19), "        Test String");
    EXPECT_EQ(StringUtils::Center("Test String",19), "    Test String    ");
    EXPECT_EQ(StringUtils::Center("Test String",16,'-'), "--Test String---");
}

TEST(StringUtilsTest, ReplaceTest){
    EXPECT_EQ(StringUtils::Replace("\\Path\\To\\A\\Directory","\\","/"), "/Path/To/A/Directory");
    EXPECT_EQ(StringUtils::Replace("a = b + c * b + d","b","e"), "a = e + c * e + d");
    EXPECT_EQ(StringUtils::Replace("aabbccaaabbbcccaaaa","aa","ee"), "eebbcceeabbbccceeee");
}

TEST(StringUtilsTest, SplitJoinTest){
    auto PathComponents = StringUtils::Split("/Path/To/A/Directory","/");
    EXPECT_EQ(PathComponents.size(), 5);
    EXPECT_EQ(PathComponents[0], "");
    EXPECT_EQ(PathComponents[1], "Path");
    EXPECT_EQ(PathComponents[2], "To");
    EXPECT_EQ(PathComponents[3], "A");
    EXPECT_EQ(PathComponents[4], "Directory");
    EXPECT_EQ(StringUtils::Join("/",PathComponents), "/Path/To/A/Directory");
    
    auto SentenceComponents = StringUtils::Split("A  tougher\ttest\nto   pass!");
    EXPECT_EQ(SentenceComponents.size(), 5);
    EXPECT_EQ(SentenceComponents[0], "A");
    EXPECT_EQ(SentenceComponents[1], "tougher");
    EXPECT_EQ(SentenceComponents[2], "test");
    EXPECT_EQ(SentenceComponents[3], "to");
    EXPECT_EQ(SentenceComponents[4], "pass!");
    EXPECT_EQ(StringUtils::Join(" ",SentenceComponents), "A tougher test to pass!");
}

TEST(StringUtilsTest, TabTest){
    EXPECT_EQ(StringUtils::ExpandTabs("1\t2\t3\t4"), "1   2   3   4");
    EXPECT_EQ(StringUtils::ExpandTabs("1\t12\t123\t1234"), "1   12  123 1234");
    EXPECT_EQ(StringUtils::ExpandTabs("1234\t123\t12\t1"), "1234    123 12  1");
    EXPECT_EQ(StringUtils::ExpandTabs("1234\t123\t12\t1",3), "1234  123   12 1");
    EXPECT_EQ(StringUtils::ExpandTabs("1234\t123\t12\t1",0), "1234123121");
}

TEST(StringUtilsTest, DistanceTest){
    EXPECT_EQ(StringUtils::EditDistance("1234","1234"), 0);
    EXPECT_EQ(StringUtils::EditDistance("Test","test"), 1);
    EXPECT_EQ(StringUtils::EditDistance("Test","test",true), 0);
    EXPECT_EQ(StringUtils::EditDistance("12345","52341"), 2);
    EXPECT_EQ(StringUtils::EditDistance("This is an example","This is a sample"), 3);
    EXPECT_EQ(StringUtils::EditDistance("int Var = Other + 3.4;","int x = y + 3.4;"), 8);
}
