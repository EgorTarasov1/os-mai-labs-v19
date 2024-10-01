#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

int main() {
    HANDLE hWritePipe1, hReadPipe1, hWritePipe2, hReadPipe2;
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
    
    // Создаем каналы для общения с дочерними процессами
    if (!CreatePipe(&hReadPipe1, &hWritePipe1, &sa, 0) || !CreatePipe(&hReadPipe2, &hWritePipe2, &sa, 0)) {
        std::cerr << "Failed to create pipes.\n";
        return 1;
    }

    std::string fileName1, fileName2;
    std::cout << "Enter the name of the file for child process 1: ";
    std::getline(std::cin, fileName1);
    std::cout << "Enter the name of the file for child process 2: ";
    std::getline(std::cin, fileName2);

    STARTUPINFO si1 = {sizeof(STARTUPINFO), 0}, si2 = {sizeof(STARTUPINFO), 0};
    PROCESS_INFORMATION pi1, pi2;
    ZeroMemory(&si1, sizeof(STARTUPINFO));
    ZeroMemory(&si2, sizeof(STARTUPINFO));
    si1.cb = sizeof(STARTUPINFO);
    si2.cb = sizeof(STARTUPINFO);
    si1.hStdError = si1.hStdOutput = hWritePipe1;
    si1.hStdInput = hReadPipe1;
    si1.dwFlags |= STARTF_USESTDHANDLES;
    si2.hStdError = si2.hStdOutput = hWritePipe2;
    si2.hStdInput = hReadPipe2;
    si2.dwFlags |= STARTF_USESTDHANDLES;

    char cmdLine1[256], cmdLine2[256];
    sprintf_s(cmdLine1, sizeof(cmdLine1), "ChildProcess1.exe %s", fileName1.c_str());
    sprintf_s(cmdLine2, sizeof(cmdLine2), "ChildProcess2.exe %s", fileName2.c_str());

    // Запуск дочерних процессов с именем файла в качестве аргумента
    if (!CreateProcess(NULL, cmdLine1, NULL, NULL, TRUE, 0, NULL, NULL, &si1, &pi1)) {
        std::cerr << "Failed to create child process 1.\n";
        return 1;
    }
    if (!CreateProcess(NULL, cmdLine2, NULL, NULL, TRUE, 0, NULL, NULL, &si2, &pi2)) {
        std::cerr << "Failed to create child process 2.\n";
        return 1;
    }

    // Закрытие неиспользуемых концов каналов в родительском процессе
    CloseHandle(hReadPipe1);
    CloseHandle(hReadPipe2);

    std::srand(std::time(0)); // инициализация генератора случайных чисел
    std::string inputLine;

    // Чтение строки от пользователя и отправка ее в один из каналов
    while (std::getline(std::cin, inputLine)) {
        if (std::rand() % 100 < 80) { // 80% вероятность
            DWORD written;
            WriteFile(hWritePipe1, inputLine.c_str(), inputLine.length(), &written, NULL);
        } else {
            DWORD written;
            WriteFile(hWritePipe2, inputLine.c_str(), inputLine.length(), &written, NULL);
        }
    }

    // Закрытие каналов
    CloseHandle(hWritePipe1);
    CloseHandle(hWritePipe2);

    // Ожидание завершения дочерних процессов
    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);

    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);

    return 0;
}