#include <iostream>
#include <string>
#include <iostream>

#include "DynamicLib.h"


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
    std::string date2 = "2023-08-22";
    std::string date3 = "08-15-2023";

    std::cout << "Standard format for " << date1 << "---" << DateConverter::convertToStandardFormat(date1) << std::endl;
    std::cout << "Standard format for " << date2 << "---" << DateConverter::convertToStandardFormat(date2) << std::endl;
    std::cout << "Standard format for " << date3 << "---" << DateConverter::convertToStandardFormat(date3) << std::endl;

    return 0;
}