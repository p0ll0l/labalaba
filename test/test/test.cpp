﻿#include <iostream>
#include <string>
#include <iostream>

#include "DynamicLib.h"
//qqq
int daysToNewYear(int month, int day) {
    const int daysInMonth[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };  // Дни в месяцах 2024 года
    int totalDays = 0;

    for (int i = month - 1; i < 12; ++i) {
        totalDays += daysInMonth[i];
    }

    return totalDays - day + 1;  // Учитываем оставшиеся дни в текущем месяце
}


int main() {
    Date start = { 1, 1, 2020 };
    Date end = { 31, 12, 2024 };

    std::cout << "Разница между датами: " << days_difference(start, end) << " дней\n";
    std::cout << (is_leap_year(2024) ? "2024 год високосный\n" : "2024 год не високосный\n");
    std::cout << "Отформатированная дата: " << format_date(start) << std::endl;

    std::cout << "Разница  между датами: " << DateConverter::days_difference(start, end) << " дней\n";
    std::cout << (DateConverter::is_leap_year(2024) ? "2024 год високосный\n" : "2024 год не високосный\n");
    std::cout << "Отформатированная дата: " << DateConverter::format_date(start) << std::endl;


    std::string date1 = "15/08/2023";
    std::string date2 = "2023-08-28";
    std::string date3 = "08-15-2023";

    std::cout << "Standard format for " << date1 << ": " << DateConverter::convertToStandardFormat(date1) << std::endl;
    std::cout << "Standard format for " << date2 << ": " << DateConverter::convertToStandardFormat(date2) << std::endl;
    std::cout << "Standard format for " << date3 << ": " << DateConverter::convertToStandardFormat(date3) << std::endl;

    int daysUntilNewYear = daysToNewYear(month, day);
    if (daysUntilNewYear > 0) {
        std::cout << "До Нового Года осталось " << daysUntilNewYear << " дня." << std::endl;
    }
    else {
        std::cout << "Новый Год уже прошел!" << std::endl;
    }
    return 0;
}