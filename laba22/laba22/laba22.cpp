#include <iostream>
#include "Header.h"

int main() {
    setlocale(LC_ALL, "rus");
    ComplexNumber num1(4, 5);
    ComplexNumber num2(3, 2);


    ComplexNumber sum = num1 + num2;
    ComplexNumber diff = num1 - num2;
    ComplexNumber prod = num1 * num2;
    ComplexNumber quot = num1 / num2;

    cout << "Sum: " << sum << endl;
    cout << "Difference: " << diff << endl;
    cout << "Product: " << prod << endl;
    cout << "Quotient: " << quot << endl;


    ComplexNumber num3(0, 0);

    ComplexNumber exception = num1 / num3;

    cout << num1 << endl;
    cout << exception << endl;

    num1 == num2;
    return 0;
}