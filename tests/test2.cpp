#include <gtest/gtest.h>
#include "../include/money.h"
#include <sstream>
#include <stdexcept>

TEST(MoneyConstructorTest, DefaultConstructor) {
    Money money;
    EXPECT_EQ(money.ToString(), "0.00");
    EXPECT_GE(money.GetSize(), 3u);
}

TEST(MoneyConstructorTest, StringConstructorValid) {
    EXPECT_EQ(Money("0").ToString(), "0.00");
    EXPECT_EQ(Money("5").ToString(), "0.05");
    EXPECT_EQ(Money("50").ToString(), "0.50");
    EXPECT_EQ(Money("100").ToString(), "1.00");
    EXPECT_EQ(Money("12345").ToString(), "123.45");
    EXPECT_EQ(Money("1234567").ToString(), "12345.67");
    EXPECT_EQ(Money("0012345").ToString(), "123.45");
}

TEST(MoneyConstructorTest, StringConstructorEdgeCases) {
    EXPECT_EQ(Money("999999999999").ToString(), "9999999999.99");
    EXPECT_EQ(Money("99").ToString(), "0.99");
    EXPECT_EQ(Money("00000100").ToString(), "1.00");
    EXPECT_EQ(Money("00000000").ToString(), "0.00");
}

TEST(MoneyConstructorTest, StringConstructorInvalid) {
    EXPECT_THROW(Money(""), std::invalid_argument);
    EXPECT_THROW(Money("12a34"), std::invalid_argument);
    EXPECT_THROW(Money("-123"), std::invalid_argument);
    EXPECT_THROW(Money("12.34"), std::invalid_argument);
    EXPECT_THROW(Money(" 123"), std::invalid_argument);
}

TEST(MoneyConstructorTest, InitializerListConstructor) {
    Money m1({0, 0, 0});
    EXPECT_EQ(m1.ToString(), "0.00");
    
    Money m2({1, 0, 0});
    EXPECT_EQ(m2.ToString(), "1.00");
    
    Money m3({1, 2, 3, 4, 5});
    EXPECT_EQ(m3.ToString(), "123.45");
    
    EXPECT_THROW(Money({10, 0, 0}), std::invalid_argument);
    EXPECT_THROW(Money({1, 2, 15}), std::invalid_argument);
}

TEST(MoneyConstructorTest, CopyConstructor) {
    Money original("1234567");
    Money copy(original);
    
    EXPECT_EQ(original.ToString(), copy.ToString());
    EXPECT_TRUE(original.isEqual(copy));
    EXPECT_EQ(original.GetSize(), copy.GetSize());
}

TEST(MoneyConstructorTest, MoveConstructor) {
    Money original("12345");
    std::string originalStr = original.ToString();
    
    Money moved(std::move(original));
    EXPECT_EQ(moved.ToString(), originalStr);
    EXPECT_EQ(original.ToString(), "0.00");
}

TEST(MoneyArithmeticTest, AdditionBasic) {
    Money m1("100");
    Money m2("50");
    Money result = m1.Plus(m2);
    EXPECT_EQ(result.ToString(), "1.50");
}

TEST(MoneyArithmeticTest, AdditionWithCarry) {
    Money m1("199");
    Money m2("1");
    Money result = m1.Plus(m2);
    EXPECT_EQ(result.ToString(), "2.00");
    
    Money m3("999999999999"); 
    Money m4("1");
    Money result2 = m3.Plus(m4);
    EXPECT_EQ(result2.ToString(), "10000000000.00");
}

TEST(MoneyArithmeticTest, AdditionDifferentSizes) {
    Money m1("1234567");
    Money m2("100");
    Money result1 = m1.Plus(m2);
    Money result2 = m2.Plus(m1);
    EXPECT_EQ(result1.ToString(), "12346.67");
    EXPECT_EQ(result2.ToString(), "12346.67");
}

TEST(MoneyArithmeticTest, SubtractionBasic) {
    Money m1("150");
    Money m2("50");
    Money result = m1.Minus(m2);
    EXPECT_EQ(result.ToString(), "1.00");
}

TEST(MoneyArithmeticTest, SubtractionWithBorrow) {
    Money m1("100");
    Money m2("99");
    Money result = m1.Minus(m2);
    EXPECT_EQ(result.ToString(), "0.01");
    
    Money m3("1000");
    Money m4("1");
    Money result2 = m3.Minus(m4);
    EXPECT_EQ(result2.ToString(), "9.99");
}

TEST(MoneyArithmeticTest, SubtractionEqual) {
    Money m1("12345");
    Money m2("12345");
    Money result = m1.Minus(m2);
    EXPECT_EQ(result.ToString(), "0.00");
}

TEST(MoneyArithmeticTest, SubtractionInvalid) {
    Money small("100");
    Money large("200");
    
    EXPECT_THROW(small.Minus(large), std::invalid_argument);
    
    try {
        small.Minus(large);
        FAIL() << "Expected std::invalid_argument";
    } catch (const std::invalid_argument& e) {
        EXPECT_STREQ(e.what(), "Can't subtract larger amount");
    }
}

TEST(MoneyArithmeticTest, SubtractionLeadingZeros) {
    Money m1("10000");
    Money m2("9999");
    Money result = m1.Minus(m2);
    EXPECT_EQ(result.ToString(), "0.01");
    EXPECT_EQ(result.GetSize(), 3u);
}

TEST(MoneyArithmeticTest, Immutability) {
    Money m1("10000");
    Money m2("2500");
    
    Money sum = m1.Plus(m2);
    Money diff = m1.Minus(m2);

    EXPECT_EQ(m1.ToString(), "100.00");
    EXPECT_EQ(m2.ToString(), "25.00");
    
    EXPECT_EQ(sum.ToString(), "125.00");
    EXPECT_EQ(diff.ToString(), "75.00");
}


TEST(MoneyComparisonTest, GreaterLessEqual) {
    Money a("200");
    Money b("100");
    Money c("200");
    
    EXPECT_TRUE(a.isGreater(b));
    EXPECT_TRUE(b.isLess(a));
    EXPECT_TRUE(a.isEqual(c));
    EXPECT_FALSE(a.isLess(c));
    EXPECT_FALSE(a.isGreater(c));
}

TEST(MoneyComparisonTest, EqualWithLeadingZeros) {
    Money m1("12345");
    Money m2("0012345");
    EXPECT_TRUE(m1.isEqual(m2));
}

TEST(MoneyAccessTest, GetSizeAndDigits) {
    Money m("12345");  // [5,4,3,2,1]
    
    EXPECT_EQ(m.GetSize(), 5u);
    EXPECT_EQ(m.GetDigits(0), 5u);
    EXPECT_EQ(m.GetDigits(1), 4u);
    EXPECT_EQ(m.GetDigits(2), 3u);
    EXPECT_EQ(m.GetDigits(3), 2u);
    EXPECT_EQ(m.GetDigits(4), 1u);
    
    EXPECT_THROW(m.GetDigits(5), std::out_of_range);
}

TEST(MoneyAccessTest, CopyMethod) {
    Money original("1234567");
    Money copy = original.Copy();
    
    EXPECT_TRUE(original.isEqual(copy));
    EXPECT_EQ(original.ToString(), copy.ToString());
}

TEST(MoneyEdgeCasesTest, LargeNumbers) {
    Money max("999999999999");
    EXPECT_EQ(max.ToString(), "9999999999.99");
    
    Money a("500000000000");
    Money b("500000000000");
    Money sum = a.Plus(b);
    EXPECT_EQ(sum.ToString(), "10000000000.00");
}

TEST(MoneyEdgeCasesTest, ZeroOperations) {
    Money z1("0");
    Money z2("000");
    Money m("12345");
    
    EXPECT_TRUE(z1.isEqual(z2));
    EXPECT_EQ(z1.ToString(), "0.00");
    
    Money sum = m.Plus(z1);
    Money diff = m.Minus(z1);
    EXPECT_TRUE(m.isEqual(sum));
    EXPECT_TRUE(m.isEqual(diff));
}

TEST(MoneyIntegrationTest, ComplexChain) {
    Money salary("10000");
    Money bonus("2345");
    Money expenses("5050");
    
    Money total = salary.Plus(bonus);
    Money net = total.Minus(expenses);
    
    EXPECT_EQ(total.ToString(), "123.45");
    EXPECT_EQ(net.ToString(), "72.95");
    EXPECT_TRUE(total.isGreater(expenses));
    EXPECT_TRUE(net.isLess(salary));
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    return result;
}