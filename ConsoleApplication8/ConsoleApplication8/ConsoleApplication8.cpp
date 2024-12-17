#include <iostream> 
#include <windows.h> 
#include <string> 
#include <vector> 
#include <thread> 
#include <chrono> 
#include <io.h> 
#include <fcntl.h> 

using namespace std;

bool hasExtension(const wstring& fileName, const wstring& ext) {
    return fileName.size() >= ext.size() &&
        fileName.compare(fileName.size() - ext.size(), ext.size(), ext) == 0;
}

bool runShScript(const wstring& scriptPath) {
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    wstring fileName = wstring(scriptPath.begin() + scriptPath.find_last_of(L"\\") + 1, scriptPath.end());
    wstring dirPath = scriptPath.substr(0, scriptPath.find_last_of(L"\\"));

    wstring command = L"bash.exe " + fileName;

    if (CreateProcessW(NULL, const_cast<wchar_t*>(command.c_str()), NULL, NULL, FALSE, 0, NULL, dirPath.c_str(), &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return true;
    }
    else {
        return false;
    }
}


void monitorDirectory(const wstring& path) {
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    wstring searchPath = path + L"\\*.sh";

    while (true) {
        hFind = FindFirstFileW(searchPath.c_str(), &findFileData);

        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                wstring fileName = findFileData.cFileName;
                wstring fullPath = path + L"\\" + fileName;

                if (hasExtension(fileName, L".sh")) {
                    wcout << L"Запуск файла: " << fileName << endl;
                    if (runShScript(fullPath)) {
                        wcout << L"Файл " << fileName << L" выполнен. Удаление..." << endl;
                        DeleteFileW(fullPath.c_str());
                    }
                    else {
                        wcout << L"Ошибка при запуске файла " << fileName << endl;
                    }
                }
            } while (FindNextFileW(hFind, &findFileData) != 0);
            FindClose(hFind);
        }

        wcout << L"Ожидание новых файлов..." << endl;
        this_thread::sleep_for(chrono::seconds(5));
    }
}

void copyFile(const wstring& sourcePath, const wstring& destPath, const wstring& fileName) {
    wstring sourceFile = sourcePath + L"\\" + fileName;
    wstring destFile = destPath + L"\\" + fileName;

    if (CopyFileW(sourceFile.c_str(), destFile.c_str(), FALSE)) {
        wcout << L"Файл " << fileName << L" успешно скопирован." << endl;
    }
    else {
        wcout << L"Ошибка при копировании файла " << fileName << endl;
    }
}

void copyDirectory(const wstring& sourcePath, const wstring& destPath) {
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    wstring searchPath = sourcePath + L"\\*";

    hFind = FindFirstFileW(searchPath.c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                wstring sourceFile = sourcePath + L"\\" + findFileData.cFileName;
                wstring destFile = destPath + L"\\" + findFileData.cFileName;

                if (CopyFileW(sourceFile.c_str(), destFile.c_str(), FALSE)) {
                    wcout << L"Файл " << findFileData.cFileName << L" успешно скопирован." << endl;
                }
                else {
                    wcout << L"Ошибка при копировании файла " << findFileData.cFileName << endl;
                }
            }
        } while (FindNextFileW(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
}

int wmain(int argc, wchar_t* argv[]) {
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    if (argc < 2) {
        wcout << L"Использование: program.exe <команда> [параметры]" << endl;
        wcout << L"Команды:" <<endl;
        wcout << L"  monitor <путь>            - мониторинг каталога" << endl;
        wcout << L"  copy <источник> <назначение> - копирование файлов" << endl;
        return 1;
    }

    wstring command = argv[1];

    if (command == L"monitor" && argc == 3) {
        monitorDirectory(argv[2]);
    }
    else if (command == L"copy" && argc == 4) {
        copyDirectory(argv[2], argv[3]);
    }
    else {
        wcout << L"Неверные параметры." << endl;
        return 1;
    }

    return 0;
}