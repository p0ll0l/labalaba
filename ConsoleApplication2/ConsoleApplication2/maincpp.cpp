#include<iostream>
int main() {
	setlocale(LC_ALL, "RU");
	int a, b;
	std::cout << "������� �������� �" <<std::endl;
	std::cin >> a;
	std::cout << "������� �������� B" << std::endl;
	std::cin >> b;
	std::cout << "������� A:" << a << " �������� B:" << b << std::endl;
	return 0;
}