#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <typeinfo>
using namespace std;
vector<int> readFromFile() {
    string fillename;
    cin >> fillename;
    ifstream file(fillename);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл '" << fillename << "'" << endl;
        return {};
    }
    vector<int> numbers;
    int number;
    while (file >> number) {
        numbers.push_back(number);
    }
    file.close();
    return numbers;
}
bool isInteger(string test_string) {
    char* pointer = &test_string[0];

    for (int i = 0; i < test_string.size(); i++) {
        if (isdigit(*pointer) == 0)
            return false;
        pointer++;
    }
    return true;
}
int getNaturalInteger() {
    string user_string;
    int array_size;
    while (true) {
        try {
            cin >> user_string;
            if (isInteger(user_string) != true) {
                throw "error";
            }
            array_size = stoi(user_string);
            return array_size;
        }
        catch (...) {
            cout << " Неккоректное значение! Попробуйте еще раз : ";
            cin.clear();
            while (cin.get() != '\n');

            continue;
        }
    }
}
int main()
{
    int a[2][5];
    a[0][0] = 5,
    a[0][1] = 4;
    a[0][2] = 3;
    cout << a[0][0];
    cout << a[0][1];
    cout << a[0][2];
	/*while (cin.fail()) {
		setlocale(LC_ALL, "ru");
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << cin.fail();
		cout << "Введите значение повторно:";
		cin >> a;
	}*/
}
