#include <iostream>
#include "include/money.h"
#include <locale>

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    std::cout << "\nСоздание объектов с помощью различных конструкторов" << std::endl;
    
    // конструктор из строки
    Money salary("12345");
    std::cout << "Зарплата: " << salary.ToString() << " рублей" << std::endl;

    // конструктор из initializer_list
    Money bonus{1, 2, 3, 4, 0};
    std::cout << "Бонус: " << bonus.ToString() << " рублей" << std::endl;

    // Конструктор с автоматическим удалением ведущих нулей
    Money smallAmount("00099");
    std::cout << "Небольшая сумма: " << smallAmount.ToString() << " рублей" << std::endl;

    // Конструктор копирования
    Money salaryCopy(salary);
    std::cout << "Копия зарплаты: " << salaryCopy.ToString() << " рублей" << std::endl;

    // === ДЕМОНСТРАЦИЯ АРИФМЕТИЧЕСКИХ ОПЕРАЦИЙ ===
    
    std::cout << "\nАрифметические операции" << std::endl;
    
    // Сложение
    Money totalIncome = salary.Plus(bonus);
    std::cout << salary.ToString() << " + " << bonus.ToString() << " = " << totalIncome.ToString() << std::endl;

    // Сложение с переносом
    Money bigNumber("999");
    Money smallNumber("1");
    Money sum = bigNumber.Plus(smallNumber);
    std::cout << bigNumber.ToString() << " + " << smallNumber.ToString() << " = " << sum.ToString() << std::endl;

    // Вычитание
    Money expenses("5000");
    Money netIncome = totalIncome.Minus(expenses);
    std::cout << totalIncome.ToString() << " - " << expenses.ToString() << " = " << netIncome.ToString() << std::endl;

    // Вычитание с заемом
    Money a("1000");
    Money b("1");
    Money result = a.Minus(b);
    std::cout << a.ToString() << " - " << b.ToString() << " = " << result.ToString() << std::endl;

    // === ДЕМОНСТРАЦИЯ ОПЕРАЦИЙ СРАВНЕНИЯ ===
    
    std::cout << "\nОперации сравнения" << std::endl;
    
    Money amount1("1000");
    Money amount2("500");
    Money amount3("1000");

    std::cout << amount1.ToString() << " > " << amount2.ToString() << " : " 
              << (amount1.isGreater(amount2) ? "true" : "false") << std::endl;
    
    std::cout << amount2.ToString() << " < " << amount1.ToString() << " : " 
              << (amount2.isLess(amount1) ? "true" : "false") << std::endl;
    
    std::cout << amount1.ToString() << " == " << amount3.ToString() << " : " 
              << (amount1.isEqual(amount3) ? "true" : "false") << std::endl;

    // === ДЕМОНСТРАЦИЯ ПЕРЕМЕЩАЮЩЕГО КОНСТРУКТОРА ===
    
    std::cout << "\nПеремещающий конструктор" << std::endl;
    
    Money temporary("8888");
    std::cout << "Временный объект до перемещения: " << temporary.ToString() << std::endl;
    
    // Перемещающий конструктор - "крадет" ресурсы у temporary
    Money newMoney(std::move(temporary));
    std::cout << "Новый объект после перемещения: " << newMoney.ToString() << std::endl;
    std::cout << "Временный объект после перемещения: " << temporary.ToString() << " (пустой)" << std::endl;

    // === ДЕМОНСТРАЦИЯ ОБРАБОТКИ ИСКЛЮЧЕНИЙ ===
    
    std::cout << "\nОбработка исключений" << std::endl;

    try {
        // Успешное вычитание
        Money x("1000");
        Money y("500");
        Money z = x.Minus(y);
        std::cout << "Успешное вычитание: " << x.ToString() << " - " << y.ToString() 
                  << " = " << z.ToString() << std::endl;

        // Попытка вычитания, которое приведет к исключению
        Money small("100");
        Money large("200");
        Money error = small.Minus(large);  // Должно бросить исключение
        std::cout << "Этот текст не должен появиться" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
    }

    try {
        // Попытка создания Money из невалидной строки
        Money invalid("12a34");  // Буква 'a' не является цифрой
        std::cout << "Этот текст не должен появиться" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
    }

    // === ДЕМОНСТРАЦИЯ КОПИРОВАНИЯ ===
    
    std::cout << "\nОперация копирования" << std::endl;
    
    Money original("7777");
    Money copy = original.Copy();
    std::cout << "Оригинал: " << original.ToString() << std::endl;
    std::cout << "Копия: " << copy.ToString() << std::endl;
    std::cout << "Оригинал == Копия: " << (original.isEqual(copy) ? "true" : "false") << std::endl;

    // === ДЕМОНСТРАЦИЯ НОРМАЛИЗАЦИИ ===
    
    std::cout << "\nНормализация (удаление ведущих нулей)" << std::endl;
    
    Money withZeros("00012300");
    std::cout << "До нормализации: 00012300" << std::endl;
    std::cout << "После нормализации: " << withZeros.ToString() << std::endl;
    
    Money allZeros("0000");
    std::cout << "Только нули '0000': " << allZeros.ToString() << std::endl;

    std::cout << "\nВыход из main() - вызов деструкторов" << std::endl;
    
    // Все объекты автоматически уничтожаются при выходе из main()
    // Деструкторы вызываются в обратном порядке создания
    return 0;
}