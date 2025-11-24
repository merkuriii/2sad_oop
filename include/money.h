#pragma once

#include <string>
#include <initializer_list>
#include <cstddef> 

class Money 
{
private:
    unsigned char* data; // копейки
    size_t size;

    // вспомогат методы
    void RemoveLeadZeros();
    int compare(const Money&other) const;
    
public:
    // конструкторы
    Money();
    Money(const size_t & n, unsigned char value = 0);
    Money(const std::initializer_list<unsigned char>& values);
    Money(const std::string& s);
    Money(const Money& other);
    Money(Money&& other) noexcept;

    ~Money() noexcept;

    // методы доступа
    size_t GetSize() const;
    unsigned char GetDigits(size_t id) const;
    std::string ToString() const;

    // арифметич операции
    Money Plus(const Money& other) const;
    Money Minus(const Money& other) const;
    Money Copy() const;

    bool isGreater(const Money& other) const;
    bool isLess(const Money& other) const;
    bool isEqual(const Money& other) const;

    Money& operator=(const Money& other);
    Money& operator=(Money&& other) noexcept;
};