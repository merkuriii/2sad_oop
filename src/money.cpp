#include "../include/money.h"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cctype>

Money::Money() : size(3), data(new unsigned char[3]{0, 0, 0}) {}

Money::Money(const size_t& n, unsigned char value) {
    if (n < 3) {
        throw std::invalid_argument("size should be at least 3");
    }
    size = n;
    data = new unsigned char[size];
    for (size_t i = 0; i < size; ++i) {
        data[i] = value;
    }
    RemoveLeadZeros();
}

Money::Money(const std::initializer_list<unsigned char>& values) {
    size = values.size();
    if (size < 3) {
        throw std::invalid_argument("should be at least 3 digits");
    }
    data = new unsigned char[size];
    size_t i = size - 1;
    for (const auto& value : values) {
        if (value > 9) {
            delete[] data;
            throw std::invalid_argument("0 <= digit <= 9");
        }
        data[i--] = value;
    }
    RemoveLeadZeros();
}

void Money::initializeFromString(const std::string& str) {
    for (char c : str) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            throw std::invalid_argument("String must contain only digits");
        }
    }
    if (str.empty()) {
        throw std::invalid_argument("Empty string");
    }

    std::string digits = str;
    size_t firstNonZero = 0;
    while (firstNonZero < digits.length() && digits[firstNonZero] == '0') {
        ++firstNonZero;
    }
    if (firstNonZero == digits.length()) {
        digits = "0";
    } else {
        digits = digits.substr(firstNonZero);
    }

    if (digits.length() == 1) digits = "00" + digits;
    else if (digits.length() == 2) digits = "0" + digits;

    size = digits.length();
    data = new unsigned char[size];
    for (size_t i = 0; i < size; ++i) {
        data[i] = digits[size - 1 - i] - '0';
    }
    RemoveLeadZeros();
}

Money::Money(const std::string& str) {
    initializeFromString(str);
}

Money::Money(const Money& other) : size(other.size), data(new unsigned char[other.size]) {
    for (size_t i = 0; i < size; ++i) {
        data[i] = other.data[i];
    }
}

Money::Money(Money&& other) noexcept : size(other.size), data(other.data) {
    other.size = 0;
    other.data = nullptr;
}

Money::~Money() noexcept {
    delete[] data;
}

size_t Money::GetSize() const { return size; }

unsigned char Money::GetDigits(size_t id) const {
    if (id >= size) {
        throw std::out_of_range("Index out of range");
    }
    return data[id];
}

void Money::RemoveLeadZeros() {
    if (size <= 3) return;

    size_t newSize = size;
    while (newSize > 3 && data[newSize - 1] == 0) {
        --newSize;
    }

    if (newSize != size) {
        unsigned char* newData = new unsigned char[newSize];
        for (size_t i = 0; i < newSize; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        size = newSize;
    }
}

Money Money::Plus(const Money& other) const {
    size_t maxSize = std::max(size, other.size);
    size_t resSize = maxSize + 1;
    unsigned char* resData = new unsigned char[resSize]{0};

    int carry = 0;
    for (size_t i = 0; i < maxSize; ++i) {
        int sum = carry;
        if (i < size) sum += data[i];
        if (i < other.size) sum += other.data[i];
        resData[i] = sum % 10;
        carry = sum / 10;
    }
    if (carry) {
        resData[maxSize] = carry;
    } else {
        resSize = maxSize;
    }

    Money result;
    result.size = resSize;
    result.data = resData;
    result.RemoveLeadZeros();
    return result;
}

Money Money::Minus(const Money& other) const {
    if (isLess(other)) {
        throw std::invalid_argument("Can't subtract larger amount");
    }
    if (isEqual(other)) {
        return Money();
    }

    size_t resSize = size;
    unsigned char* resData = new unsigned char[resSize]{0};

    int borrow = 0;
    for (size_t i = 0; i < size; ++i) {
        int minuend = data[i] - borrow;
        int subtrahend = (i < other.size) ? other.data[i] : 0;

        if (minuend < subtrahend) {
            minuend += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        resData[i] = minuend - subtrahend;
    }

    Money result;
    result.size = resSize;
    result.data = resData;
    result.RemoveLeadZeros();
    return result;
}

Money Money::Copy() const {
    return Money(*this);
}

std::string Money::ToString() const {
    if (size < 3) return "0.00";

    std::stringstream ss;
    bool nonZero = false;
    for (int i = static_cast<int>(size) - 1; i >= 2; --i) {
        if (data[i] != 0 || nonZero) {
            ss << static_cast<int>(data[i]);
            nonZero = true;
        }
    }
    if (!nonZero) ss << "0";
    ss << ".";

    ss << std::setw(2) << std::setfill('0')
       << (static_cast<int>(data[1]) * 10 + static_cast<int>(data[0]));

    return ss.str();
}

bool Money::isGreater(const Money& other) const {
    if (size != other.size) return size > other.size;
    for (int i = static_cast<int>(size) - 1; i >= 0; --i) {
        if (data[i] != other.data[i]) return data[i] > other.data[i];
    }
    return false;
}

bool Money::isLess(const Money& other) const {
    if (size != other.size) return size < other.size;
    for (int i = static_cast<int>(size) - 1; i >= 0; --i) {
        if (data[i] != other.data[i]) return data[i] < other.data[i];
    }
    return false;
}

bool Money::isEqual(const Money& other) const {
    if (size != other.size) return false;
    for (size_t i = 0; i < size; ++i) {
        if (data[i] != other.data[i]) return false;
    }
    return true;
}