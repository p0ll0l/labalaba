#include "Header1.h"

// ���������� ������� �������� �������/������� ��������
void createArrayOfEmployees(std::vector<Employee>& employees, int n) {
    employees.resize(n);
}

// ���������� ������� ���������� �������
void fillEmployeesWithRandomData(std::vector<Employee>& employees) {
    srand(static_cast<unsigned int>(time(nullptr))); // ������������� ���������� ��������� �����
    for (auto& employee : employees) {
        generateFullName(employee.fullName);
        employee.departmentNumber = rand() % 10; // �������� �� 0 �� 9
        employee.position = "Programmer"; // �������� ��� ���������
        employee.startDate = time(nullptr); // ������� ���� ������ ������
    }
}

// ���������� ������� ������ �� �����
void printEmployees(const std::vector<Employee>& employees) {
    for (const auto& employee : employees) {
        std::cout << "���: " << employee.fullName << std::endl;
        std::cout << "����� ������: " << employee.departmentNumber << std::endl;
        std::cout << "���������: " << employee.position << std::endl;
        std::cout << "���� ������ ������: " << asctime(gmtime(&employee.startDate)) << std::endl;
    }
}

// ������� ��������� ���
void generateFullName(std::string& fullName) {
    const std::string firstNames[] = { "����", "�������", "������", "�������", "������" };
    const std::string lastNames[] = { "������", "������", "�������", "��������", "�������" };
    const std::string patronymics[] = { "��������", "����������", "���������", "����������", "����������" };

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
