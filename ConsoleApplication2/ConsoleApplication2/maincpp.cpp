#include<iostream>
int main() {
	setlocale(LC_ALL, "RU");
	int a, b;
	std::cout << "Введите значение А" <<std::endl;
	std::cin >> a;
	std::cout << "Введите значение B" << std::endl;
	std::cin >> b;
	std::cout << "Знаение A:" << a << " Значение B:" << b << std::endl;
	return 0;
}