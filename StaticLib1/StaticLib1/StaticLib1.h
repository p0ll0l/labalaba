#pragma once


#include <iostream>
#include <string>


// Структура для хранения даты
struct Date {
    int day, month, year;
};

// Функция для проверки, является ли год високосным
bool is_leap_year(int year);

int days_difference(const Date& start, const Date& end);

std::string format_date(const Date& date);