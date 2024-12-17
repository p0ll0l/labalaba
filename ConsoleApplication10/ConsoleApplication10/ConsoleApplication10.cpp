#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <complex>
using namespace std;
class Complex {
private:
    double real;
    double imag;

public:
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    Complex& operator=(const Complex& other) {
        if (this != &other) {
            real = other.real;
            imag = other.imag;
        }
        return *this;
    }

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
        return Complex((real * other.real + imag * other.imag) / denominator,
            (imag * other.real - real * other.imag) / denominator);
    }
    Complex operator^(int &exponent) const {
        double r = sqrt(real * real + imag * imag);
        double theta = atan2(imag, real);

        double new_r = pow(r, exponent);
        double new_theta = theta * exponent;

        return Complex(new_r * cos(new_theta), new_r * sin(new_theta));
    }

    Complex operator+(double value) const {
        return Complex(real + value, imag);
    }

    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }

    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }

    friend ostream& operator<<(std::ostream& os, const Complex& c) {
        os << c.real << (c.imag >= 0 ? "+" : "") << c.imag << "i";
        return os;
    }
};

class ComplexCollection {
private:
    vector<Complex> numbers;
public:
    void add(const Complex& c) {
        numbers.push_back(c);
    }
    Complex& operator[](size_t index) {
        if (index >= numbers.size()) throw out_of_range("Index out of range");
        return numbers[index];
    }


    friend ostream& operator<<(std::ostream& os, const ComplexCollection& collection) {
        for (const auto& num : collection.numbers) {
            os << num << " ";
        }
        return os;
    }
};

int main() {

    Complex c1(3, 4);
    Complex c2(1, -2);

    cout << "c1: " << c1 << endl;
    cout << "c2: " << c2 << endl;

    cout << "c1 + c2: " << (c1 + c2) << endl;
    cout << "c1 - c2: " << (c1 - c2) << endl;
    cout << "c1 * c2: " << (c1 * c2) << endl;
    cout << "c1 ^ 2: " << (c1 ^ 3) << endl;

    try {
        cout << "c1 / c2: " << (c1 / c2) << endl;
    }
    catch (const runtime_error& e) {
        cerr << e.what() << endl;
    }

    cout << "c1 == c2: " << (c1 == c2 ? "true" : "false") << std::endl;

    ComplexCollection collection;

    collection.add(c1);
    collection.add(c2);

    cout << "Collection: " << collection << endl;

cout << "First element in collection: " << collection[0] << std::endl;

    Complex c3;
    c3 = c1;
    std::cout << "After assignment, c3: " << c3 << endl;
    return 0;
}
