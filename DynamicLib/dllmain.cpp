// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "DynamicLib.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}



bool DateConverter::is_leap_year(int year) {
    return ::is_leap_year(year);
}
int DateConverter::days_difference(const Date& start, const Date& end) {
    return ::days_difference(start, end);
}
string DateConverter::format_date(const Date& date) {
    return ::format_date(date);
}

string DateConverter::convertToStandardFormat(const std::string& date) {
    std::string year, month, day;

    // Проверка формата "DD/MM/YYYY"
    if (date.size() == 10 && date[2] == '/' && date[5] == '/') {
        day = date.substr(0, 2);
        month = date.substr(3, 2);
        year = date.substr(6, 4);
        return year + "-" + month + "-" + day;
    }

    // Проверка формата "YYYY-MM-DD"
    if (date.size() == 10 && date[4] == '-' && date[7] == '-') {
        return date; // Уже в стандартном формате
    }

    // Проверка формата "MM-DD-YYYY"
    if (date.size() == 10 && date[2] == '-' && date[5] == '-') {
        month = date.substr(0, 2);
        day = date.substr(3, 2);
        year = date.substr(6, 4);
        return year + "-" + month + "-" + day;
    }

    // Если формат не распознан, вернуть сообщение об ошибке
    return "Invalid date format";
}