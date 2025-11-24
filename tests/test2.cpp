#include <gtest/gtest.h>
#include "money.h"

TEST(MoneyTest, DefaultConstructor) {
    Money money;
    EXPECT_EQ(money.GetSize(), 0);
}

TEST(MoneyTest, InitializerListConstructor) {
    Money money{1, 2, 3, 4, 5};
    EXPECT_EQ(money.GetSize(), 5);
    EXPECT_EQ(money.ToString(), "12345");
}

TEST(MoneyTest, StringConstructor) {
    Money money("12345");
    EXPECT_EQ(money.GetSize(), 5);
    EXPECT_EQ(money.ToString(), "12345");
}

TEST(MoneyTest, StringConstructorWithLeadingZeros) {
    Money money("00123");
    EXPECT_EQ(money.GetSize(), 3); // Ведущие нули удаляются
    EXPECT_EQ(money.ToString(), "123");
}

TEST(MoneyTest, StringConstructorAllZeros) {
    Money money("0000");
    EXPECT_EQ(money.GetSize(), 1);
    EXPECT_EQ(money.ToString(), "0");
}

TEST(MoneyTest, CopyConstructor) {
    Money original("123");
    Money copy(original);
    EXPECT_EQ(copy.ToString(), "123");
    EXPECT_TRUE(original.isEqual(copy));
}

TEST(MoneyTest, MoveConstructor) {
    Money original("123");
    Money moved(std::move(original));
    EXPECT_EQ(moved.ToString(), "123");
    EXPECT_EQ(original.GetSize(), 0);
}

TEST(MoneyTest, AddOperation) {
    Money a("123");
    Money b("456");
    Money result = a.Plus(b);
    EXPECT_EQ(result.ToString(), "579");
}

TEST(MoneyTest, AddWithCarry) {
    Money a("999");
    Money b("1");
    Money result = a.Plus(b);
    EXPECT_EQ(result.ToString(), "1000");
}

TEST(MoneyTest, SubtractOperation) {
    Money a("456");
    Money b("123");
    Money result = a.Minus(b);
    EXPECT_EQ(result.ToString(), "333");
}

TEST(MoneyTest, SubtractWithBorrow) {
    Money a("1000");
    Money b("1");
    Money result = a.Minus(b);
    EXPECT_EQ(result.ToString(), "999");
}

TEST(MoneyTest, SubtractEqual) {
    Money a("123");
    Money b("123");
    Money result = a.Minus(b);
    EXPECT_EQ(result.ToString(), "0");
}

TEST(MoneyTest, SubtractLargerThrowsException) {
    Money a("100");
    Money b("200");
    EXPECT_THROW(a.Minus(b), std::logic_error);
}

TEST(MoneyTest, ComparisonOperations) {
    Money a("100");
    Money b("50");
    Money c("100");
    
    EXPECT_TRUE(a.isGreater(b));
    EXPECT_TRUE(b.isLess(a));
    EXPECT_TRUE(a.isEqual(c));
    EXPECT_FALSE(a.isLess(b));
    EXPECT_FALSE(b.isGreater(a));
}

TEST(MoneyTest, CopyMethod) {
    Money original("12345");
    Money copy = original.Copy();
    EXPECT_TRUE(original.isEqual(copy));
    EXPECT_EQ(copy.ToString(), "12345");
}

TEST(MoneyTest, AssignmentOperator) {
    Money a("123");
    Money b;
    b = a;
    EXPECT_TRUE(a.isEqual(b));
    EXPECT_EQ(b.ToString(), "123");
}

TEST(MoneyTest, MoveAssignmentOperator) {
    Money a("123");
    Money b;
    b = std::move(a);
    EXPECT_EQ(b.ToString(), "123");
    EXPECT_EQ(a.GetSize(), 0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}