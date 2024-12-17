#include "Header1.h"

// Реализация функции создания массива/вектора структур
void createArrayOfEmployees(std::vector<Employee>& employees, int n) {
    employees.resize(n);
}

// Реализация функции заполнения данными
void fillEmployeesWithRandomData(std::vector<Employee>& employees) {
    srand(static_cast<unsigned int>(time(nullptr))); // Инициализация генератора случайных чисел
    for (auto& employee : employees) {
        generateFullName(employee.fullName);
        employee.departmentNumber = rand() % 10; // Диапазон от 0 до 9
        employee.position = "Programmer"; // Заглушка для должности
        employee.startDate = time(nullptr); // Текущая дата начала работы
    }
}

// Реализация функции вывода на экран
void printEmployees(const std::vector<Employee>& employees) {
    for (const auto& employee : employees) {
        std::cout << "ФИО: " << employee.fullName << std::endl;
        std::cout << "Номер отдела: " << employee.departmentNumber << std::endl;
        std::cout << "Должность: " << employee.position << std::endl;
        std::cout << "Дата начала работы: " << asctime(gmtime(&employee.startDate)) << std::endl;
    }
}

// Функция генерации ФИО
void generateFullName(std::string& fullName) {
    const std::string firstNames[] = { "Иван", "Алексей", "Сергей", "Николай", "Михаил" };
    const std::string lastNames[] = { "Иванов", "Петров", "Сидоров", "Кузнецов", "Смирнов" };
    const std::string patronymics[] = { "Иванович", "Алексеевич", "Сергеевич", "Николаевич", "Михайлович" };

    if (sizeof(firstNames) / sizeof(firstNames[0]) > 0) {
        size_t firstIndex = rand() % sizeof(firstNames) / sizeof(firstNames[0]);
        if (firstIndex < sizeof(firstNames) / sizeof(firstNames[0])) {
            size_t lastIndex = rand() % sizeof(lastNames) / sizeof(lastNames[0]);
            if (lastIndex < sizeof(lastNames) / sizeof(lastNames[0])) {
                size_t patronymicIndex = rand() % sizeof(patronymics) / sizeof(patronymics[0]);
                if (patronymicIndex < sizeof(patronymics) / sizeof(patronymics[0])) {
                    fullName = firstNames[firstIndex] + " " + lastNames[lastIndex] + " " + patronymics[patronymicIndex];
                }
            }
        }
    }
}
