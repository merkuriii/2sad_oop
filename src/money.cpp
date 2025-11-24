#include "../include/money.h"
#include <algorithm>
#include <iomanip>
#include <sstream>

// конструктор по умолчанию
Money::Money() : size(0), data(nullptr) {}

// конструктор с размером и значением
Money::Money(const size_t& n, unsigned char value) : size(n) {
    if (n == 0) {
        data = nullptr;
        return;
    }

    data = new unsigned char[n];
    for (size_t i = 0; i < n; i++) {
        data[i] = value;
    }
    RemoveLeadZeros();
}

// конструктор из initializer_list 
Money::Money(const std::initializer_list<unsigned char>& values) {
    size = values.size();
    if (size == 0) {
        data = nullptr;
        return;
    }

    data = new unsigned char[size];
    size_t i = 0;
    for (const auto& value : values) {
        if (value > 9) {
            delete[] data;
            throw std::invalid_argument("digit should be less than 9\n");
        }
        data[i++] = value;
    }
    RemoveLeadZeros();
}

// конструктор из строки
Money::Money(const std::string& str) {
    if (str.empty()) {
        size = 0;
        data = nullptr;
        return;
    }

    size = str.length();
    data = new unsigned char[size];

    for (size_t i = 0; i < size; ++i) {
        char c = str[i];
        if (c < '0' || c > '9') {
            delete[] data;
            throw std::invalid_argument("String cannot contain not digits\n");
        }
        data[i] = static_cast<unsigned char>(c - '0');
    }
    RemoveLeadZeros();
}

// конструктор копирования
Money::Money(const Money& other) {
    size = other.size;
    if (size == 0) {
        data = nullptr;
        return;
    }

    data = new unsigned char[size];
    for (size_t i = 0; i < size; ++i) {
        data[i] = other.data[i];
    }
}

// конструктор перемещения
Money::Money(Money&& other) noexcept {
    size = other.size;
    data = other.data;
    other.size = 0;
    other.data = nullptr;
}

// деструктор
Money::~Money() noexcept {
    if (data != nullptr) {
        delete[] data;
        data = nullptr;
    }
    size = 0;
}

size_t Money::GetSize() const {
    return size;
}

unsigned char Money::GetDigits(size_t id) const {
    if (id >= size) {
        throw std::out_of_range("Index out of range");
    }
    return data[id];
}

void Money::RemoveLeadZeros() {
    if (data == nullptr || size == 0) return;

    size_t no_zero_1 = 0;
    while (no_zero_1 < size && data[no_zero_1] == 0) {
        no_zero_1++;
    }

    if (no_zero_1 == size) {
        delete[] data;
        size = 1;
        data = new unsigned char[1];
        data[0] = 0;
        return;
    }

    if (no_zero_1 > 0) {
        size_t newSize = size - no_zero_1;
        unsigned char* newData = new unsigned char[newSize];
        for (size_t i = 0; i < newSize; ++i) {
            newData[i] = data[no_zero_1 + i];
        }
        
        delete[] data;
        data = newData;
        size = newSize;
    }
}

// сравнение (-1 - меньше, 0 - равно, 1 - больше)
int Money::compare(const Money&other) const {
    if (size != other.size) {
        return size > other.size ? 1 : -1; // тернарный оператор
    }

    for (size_t i = 0; i < size; ++i) {
        if (data[i] != other.data[i]) {
            return data[i] > other.data[i] ? 1 : -1;
        }
    }
    return 0;
}

// арифметика
Money Money::Plus(const Money& other) const {
    size_t maxSize = std::max(size, other.size);
    size_t resSize = maxSize + 1; // +1 для возможного переноса
    unsigned char* resData = new unsigned char[resSize]();

    int c = 0;
    for (size_t i = 0; i < resSize; ++i) {
        int sum = c;
        if (i < size) {
            sum += data[size - i - 1];
        }
        if (i < other.size) {
            sum += other.data[other.size - i -1];
        }
        resData[resSize - i - 1] = sum % 10;
        c = sum / 10;
    }
    Money result;
    result.size = resSize;
    result.data = resData;
    result.RemoveLeadZeros();
    return result;
}

Money Money::Minus(const Money& other) const {
    if (isLess(other)) {
        throw std::logic_error("Can't minus larger money");
    }
    if (isEqual(other)) {
        return Money(1, 0); // возвращаем ноль
    }
    size_t resSize = size;
    unsigned char* resData = new unsigned char[resSize]();

    int c = 0;
    for (size_t i = 0; i < size; ++i) {
        int mi = data[size - i -1] - c;
        int minus = (i < other.size) ? other.data[other.size - i - 1] : 0;
        if (mi < minus) {
            mi += 10;
            c = 1;
        } else {
            c = 0;
        }
        resData[resSize - i -1] = mi - minus;
    }
    Money res;
    res.size = resSize;
    res.data = resData;
    res.RemoveLeadZeros();
    return res;
}

Money Money::Copy() const {
    return Money(*this);
}

std::string Money::ToString() const {
    if (size == 0) return "0";
    std::stringstream ss;
    for (size_t i = 0; i < size; ++i) {
        ss << static_cast<int>(data[i]);
    }
    return ss.str();
}

// операции сравнения

bool Money::isGreater(const Money& other) const {
    return compare(other) > 0;
}

bool Money::isLess(const Money& other) const {
    return compare(other) < 0;
}

bool Money::isEqual(const Money& other) const {
    return compare(other) == 0;
}

// операторы присваивания 
Money& Money::operator=(const Money& other) {
    if (this != &other) {
        if (data != nullptr) {
            delete[] data;
        }
        
        size = other.size;
        if (size > 0) {
            data = new unsigned char[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        } else {
            data = nullptr;
        }
    }
    return *this;
}

Money& Money::operator=(Money&& other) noexcept {
    if (this != &other) {
        if (data != nullptr) {
            delete[] data;
        }
        
        size = other.size;
        data = other.data;
        
        other.size = 0;
        other.data = nullptr;
    }
    return *this;
}