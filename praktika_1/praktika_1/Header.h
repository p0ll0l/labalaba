#ifndef SEARCH_ALGORITHMS_H
#define SEARCH_ALGORITHMS_H
using namespace std;

#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <chrono>
#include <string>
#include <algorithm>

int barier(vector<int> arr, int key);
vector<int> readFromFile();
void insertionSort(vector<int>& arr);
int binarySearch(const std::vector<int>& array, int target);
void osnova();
int find_fake_coin(const std::vector<int>& coins);
#endif // SEARCH_ALGORITHMS_H
