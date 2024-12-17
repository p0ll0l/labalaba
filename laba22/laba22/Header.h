#include <iostream>
using namespace std;

class ComplexNumber {
private:
    double real;
    double imaginary;

public:
    ComplexNumber();
    ComplexNumber(double r, double i);
    ComplexNumber(const ComplexNumber& other);

    double getReal() const;
    void setReal(double r);
    double getImaginary() const;
    void setImaginary(double i);

    ComplexNumber operator+(const ComplexNumber& other) const;
    ComplexNumber operator-(const ComplexNumber& other) const;
    ComplexNumber operator*(const ComplexNumber& other) const;
    ComplexNumber operator/(const ComplexNumber& other) const;
    bool operator==(const ComplexNumber& other) const;

    friend ostream& operator<<(ostream& os, const ComplexNumber& num);
};