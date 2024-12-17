#include "Header.h"

ComplexNumber::ComplexNumber() : real(0), imaginary(0) {}

ComplexNumber::ComplexNumber(double r, double i) : real(r), imaginary(i) {}

ComplexNumber::ComplexNumber(const ComplexNumber& other) : real(other.real), imaginary(other.imaginary) {}
double ComplexNumber::getReal() const { return real; }
void ComplexNumber::setReal(double r) { real = r; }
double ComplexNumber::getImaginary() const { return imaginary; }
void ComplexNumber::setImaginary(double i) { imaginary = i; }

ComplexNumber ComplexNumber::operator+(const ComplexNumber& other) const {
    return ComplexNumber(real + other.real, imaginary + other.imaginary);
}

ComplexNumber ComplexNumber::operator-(const ComplexNumber& other) const {
    return ComplexNumber(real - other.real, imaginary - other.imaginary);
}

ComplexNumber ComplexNumber::operator*(const ComplexNumber& other) const {
    return ComplexNumber(real * other.real - imaginary * other.imaginary,
        real * other.imaginary + imaginary * other.real);
}

ComplexNumber ComplexNumber::operator/(const ComplexNumber& other) const {
    if (other.real == 0 && other.imaginary == 0) {
        cout << "на ноль делить не нужно" << endl;
        return *this;
    }

    double denominator = other.real * other.real + other.imaginary * other.imaginary;
    return ComplexNumber((real * other.real + imaginary * other.imaginary) / denominator,
        (imaginary * other.real - real * other.imaginary) / denominator);
}


bool ComplexNumber::operator==(const ComplexNumber& other) const {
    return real == other.real && imaginary == other.imaginary;
}

ostream& operator<<(ostream& os, const ComplexNumber& num) {
    os << num.getReal() << " + " << num.getImaginary() << "i";
    return os;
}