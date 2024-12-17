#include <iostream>
#include <vector>
#include <stdexcept>

class Complex {
private:
    double real; 
    double imag; 

public:
    // �����������
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // ���������� ��������� ����������
    Complex& operator=(const Complex& other) {
        if (this != &other) {
            real = other.real;
            imag = other.imag;
        }
        return *this;
    }

    // ���������� ���������� �������������� ��������
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

    // ���������� �������� �������� � ������
    Complex operator+(double value) const {
        return Complex(real + value, imag);
    }

    // ���������� �������� ���������
    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }

    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }

    // ���������� ��������� ������
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << c.real << (c.imag >= 0 ? "+" : "") << c.imag << "i";
        return os;
    }
};

class ComplexCollection {
private:
    std::vector<Complex> numbers; // ������ ��� �������� ����������� �����

public:
    // �������� ����������� ����� � ���������
    void add(const Complex& c) {
        numbers.push_back(c);
    }

    // ���������� � ��������� ������
    Complex& operator[](size_t index) {
        if (index >= numbers.size()) throw std::out_of_range("Index out of range");
        return numbers[index];
    }

    const Complex& operator[](size_t index) const { // ����������� ������
        if (index >= numbers.size()) throw std::out_of_range("Index out of range");
        return numbers[index];
    }

    // ����� ��������� �� �����
    friend std::ostream& operator<<(std::ostream& os, const ComplexCollection& collection) {
        for (const auto& num : collection.numbers) {
            os << num << " ";
        }
        return os;
    }
};

int main() {
    // ������� ��������� ����������� �����
    Complex c1(3, 4);
    Complex c2(1, -2);
    // ������������ �������������� ��������
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
    // ��������� ����������� �����
    std::cout << "c1 == c2: " << (c1 == c2 ? "true" : "false") << "n";
    // ������� ��������� ����������� �����
    ComplexCollection collection;
    // ��������� ����� � ���������
    collection.add(c1);
    collection.add(c2);
    std::cout << "Collection: " << collection << "n";
    // ���������� ��������������
    std::cout << "First element in collection: " << collection[0] << "n";

    // ������������
    Complex c3;
    c3 = c1;
    std::cout << "After assignment, c3: " << c3 << "n";

    return 0;
}
