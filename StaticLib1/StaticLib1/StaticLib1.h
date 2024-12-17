#pragma once


#include <iostream>
#include <string>


// ��������� ��� �������� ����
struct Date {
    int day, month, year;
};

// ������� ��� ��������, �������� �� ��� ����������
bool is_leap_year(int year);

int days_difference(const Date& start, const Date& end);

std::string format_date(const Date& date);