#include "pch.h"
#include "framework.h"
#include <iostream>
#include <string>

// Структура для хранения даты
struct Date {
    int day, month, year;
};

// Функция для проверки, является ли год високосным
bool is_leap_year(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

// Функция для вычисления разницы между датами в днях
int days_difference(const Date& start, const Date& end) {
    int start_days = start.year * 365 + start.month * 30 + start.day;
    int end_days = end.year * 365 + end.month * 30 + end.day;

    // Добавляем високосные годы (каждый 4-й год добавляет один день)
    for (int year = start.year; year <= end.year; ++year) {
        if (is_leap_year(year)) {
            start_days++;
        }
    }

    return end_days - start_days;
}

// Функция для форматирования даты в строку
std::string format_date(const Date& date) {
    std::string formatted_date = "";
    formatted_date += (date.day < 10 ? "0" : "") + std::to_string(date.day) + "-";
    formatted_date += (date.month < 10 ? "0" : "") + std::to_string(date.month) + "-";
    formatted_date += std::to_string(date.year);

    return formatted_date;
}