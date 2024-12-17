#include <iostream>
using namespace std;
 
class point {
public: 
    int GetA() {
        return a;
    }
    int GetB() {
        return b;
    }
    void SetA(int valueA) {
        a = valueA;
    }
    void SetB(int valueB) {
        b = valueB;
    }
    void print() {
        cout << "Значение A: " << a << endl;
        cout << "Значение B: " << b << endl;
    }
        
private:
    int a;
    int b;
};

int main()
{
    setlocale(LC_ALL, "ru");
    point one;
     while(true) {
        cout << "что хочешь?: " << endl;
        int i;
        cin >>i;
        if (i == 1) {
            cout << "Ведите значение для А" << endl;
            int a;
            cin >> a;
            one.SetA(a);
        }
        if (i == 2) {
            cout << "Ведите значение для B" << endl;
            int b;
            cin >> b;
            one.SetB(b);
        }
        if (i == 3) {
            cout << "Знчение A: " << one.GetA() << endl;
        }
        if (i == 4) {
            cout << "Знчение B: " << one.GetB() << endl;
        }
        if (i == 5)
            one.print();

    }

}
