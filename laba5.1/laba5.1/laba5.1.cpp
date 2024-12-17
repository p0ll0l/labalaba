#include <iostream>
using namespace std;

void foo(int *pa, int *pb) {
	*pa = *pb;
	*pb = *pb;
}

int main() {
	int a = 5;
	int b = 6;
	cout << "================" << endl;
	cout << a << endl;
	cout << b << endl;
	cout << "================" << endl;
	foo(&a,&b);
	cout << a << endl;
	cout << b << endl;


	return 0;
}