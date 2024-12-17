#include <iostream>
#include<cmath>
using namespace std;
int main()
{
    setlocale(LC_ALL, "ru");
    float x;
    while (true) {
        cout << "Введите значение x: ";
        if (!(cin >> x)) {
            cout << "Ошибка! Введено некорректное значение x." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        break;
    }
    if (x < -2)
    {
        cout << "Значение y = " << (2 + cos(pow(x, 3) + 3)) / (4 + pow(x, 2)) << endl;
    }
    else if (-2 <= x < 3)
    {
        cout << "Значение у = " << (2 - exp( - 2 * x)) / (2 * pow(x, 2) + pow(x, 3)) << endl;

    }
    else
    {
        cout << "Значение у = " << (cos(pow(x, 2) + 5 * x)) / (5 * pow(x, 2) - 2) << endl;
    }
    return 0;
}