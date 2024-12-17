#pragma once


#ifdef MYDYNAMICLIB_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#include <iostream>
#include <string>
#include "StaticLib1.h"

using namespace std;

class DLL_API DateConverter {
public:
	static bool is_leap_year(int year);

	static int days_difference(const Date& start, const Date& end);

	static string format_date(const Date& date);

	static string convertToStandardFormat(const std::string& date);
};
