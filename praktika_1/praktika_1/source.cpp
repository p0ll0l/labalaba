#include <iostream>
#include <vector>
#include <stdexcept>

class Complex {
private:
    double real; 
    double imag; 

public:
    // Конструктор
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // Перегрузка оператора присвоения
    Complex& operator=(const Complex& other) {
        if (this != &other) {
            real = other.real;
            imag = other.imag;
        }
        return *this;
    }

    // Перегрузка операторов арифметических операций
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }

    Complex operator*(const Complex& other) const {
        return Complex(real * other.real - imag * other.imag,
            real * other.imag + imag * other.real);
    }

    Complex operator/(const Complex& other) const {
        double denominator = other.real * other.real + other.imag * other.imag;
        if (denominator == 0) throw std::runtime_error("Division by zero");
        return Complex((real * other.real + imag * other.imag) / denominator,
            (imag * other.real - real * other.imag) / denominator);
    }

    // Перегрузка операции сложения с числом
    Complex operator+(double value) const {
        return Complex(real + value, imag);
    }

    // Перегрузка операции сравнения
    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }

    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }

    // Перегрузка оператора вывода
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << c.real << (c.imag >= 0 ? "+" : "") << c.imag << "i";
        return os;
    }
};

class ComplexCollection {
private:
    std::vector<Complex> numbers; // Вектор для хранения комплексных чисел

public:
    // Добавить комплексное число в коллекцию
    void add(const Complex& c) {
        numbers.push_back(c);
    }

    // Индексация с проверкой границ
    Complex& operator[](size_t index) {
        if (index >= numbers.size()) throw std::out_of_range("Index out of range");
        return numbers[index];
    }

    const Complex& operator[](size_t index) const { // Константная версия
        if (index >= numbers.size()) throw std::out_of_range("Index out of range");
        return numbers[index];
    }

    // Вывод коллекции на экран
    friend std::ostream& operator<<(std::ostream& os, const ComplexCollection& collection) {
        for (const auto& num : collection.numbers) {
            os << num << " ";
        }
        return os;
    }
};

int main() {
    // Создаем несколько комплексных чисел
    Complex c1(3, 4);
    Complex c2(1, -2);
    // Демонстрация арифметических операций
    std::cout << "c1: " << c1 << "n";
    std::cout << "c2: " << c2 << "n";

    std::cout << "c1 + c2: " << (c1 + c2) << "n";
    std::cout << "c1 - c2: " << (c1 - c2) << "n";
    std::cout << "c1 * c2: " << (c1 * c2) << "n";
    try {
        std::cout << "c1 / c2: " << (c1 / c2) << "n";
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << "n";
    }
    // Сравнение комплексных чисел
    std::cout << "c1 == c2: " << (c1 == c2 ? "true" : "false") << "n";
    // Создаем коллекцию комплексных чисел
    ComplexCollection collection;
    // Добавляем числа в коллекцию
    collection.add(c1);
    collection.add(c2);
    std::cout << "Collection: " << collection << "n";
    // Используем индексирование
    std::cout << "First element in collection: " << collection[0] << "n";

    // Присваивание
    Complex c3;
    c3 = c1;
    std::cout << "After assignment, c3: " << c3 << "n";

    return 0;
}
