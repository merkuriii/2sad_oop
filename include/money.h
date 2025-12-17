#pragma once

#include <string>
#include <initializer_list>
#include <stdexcept>

class Money
{
private:
    size_t size;
    unsigned char* data;

    void RemoveLeadZeros();
    void initializeFromString(const std::string& str);

public:
    Money();
    Money(const size_t& n, unsigned char value = 0);
    Money(const std::initializer_list<unsigned char>& values);
    Money(const std::string& str);

    Money(const Money& other);
    Money(Money&& other) noexcept;
    ~Money() noexcept;

    Money& operator=(const Money& other) = delete;
    Money& operator=(Money&& other) noexcept = delete;

    size_t GetSize() const;
    unsigned char GetDigits(size_t id) const;

    Money Plus(const Money& other) const;
    Money Minus(const Money& other) const;

    Money Copy() const;

    bool isGreater(const Money& other) const;
    bool isLess(const Money& other) const;
    bool isEqual(const Money& other) const;

    std::string ToString() const;
};