#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <vector>
#include <iostream>
#include <ctime>

// ����������� ��������� Employee
struct Employee {
    std::string fullName;
    int departmentNumber;
    std::string position;
    std::time_t startDate;
};

// ��������� �������
void createArrayOfEmployees(std::vector<Employee>& employees, int n);
void fillEmployeesWithRandomData(std::vector<Employee>& employees);
void printEmployees(const std::vector<Employee>& employees);

#endif // EMPLOYEE_H