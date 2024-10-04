#include <windows.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

int main() {
    HANDLE hWritePipe1, hReadPipe1, hWritePipe2, hReadPipe2;
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

    
    if (!CreatePipe(&hReadPipe1, &hWritePipe1, &sa, 0) || !CreatePipe(&hReadPipe2, &hWritePipe2, &sa, 0)) {
        std::cerr << "Failed to create pipes.\n";
        return 1;
    }

    std::string fileName1, fileName2;
    std::cout << "Enter the name of the file for child process 1: ";
    std::getline(std::cin, fileName1);
    std::cout << "Enter the name of the file for child process 2: ";
    std::getline(std::cin, fileName2);

    STARTUPINFO si1 = {sizeof(STARTUPINFO)};
    STARTUPINFO si2 = {sizeof(STARTUPINFO)};
    PROCESS_INFORMATION pi1, pi2;
    ZeroMemory(&pi1, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&pi2, sizeof(PROCESS_INFORMATION));

 
    si1.hStdInput = hReadPipe1;
    si1.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si1.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si1.dwFlags |= STARTF_USESTDHANDLES;

    si2.hStdInput = hReadPipe2;
    si2.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si2.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si2.dwFlags |= STARTF_USESTDHANDLES;

 
    char cmdLine1[256], cmdLine2[256];
    sprintf_s(cmdLine1, sizeof(cmdLine1), "child1.exe \"%s\"", fileName1.c_str());
    sprintf_s(cmdLine2, sizeof(cmdLine2), "child2.exe \"%s\"", fileName2.c_str());


    if (!CreateProcess(NULL, cmdLine1, NULL, NULL, TRUE, 0, NULL, NULL, &si1, &pi1)) {
        std::cerr << "Failed to create child process 1.\n";
        return 1;
    }


    if (!CreateProcess(NULL, cmdLine2, NULL, NULL, TRUE, 0, NULL, NULL, &si2, &pi2)) {
        std::cerr << "Failed to create child process 2.\n";
        return 1;
    }


    CloseHandle(hReadPipe1);
    CloseHandle(hReadPipe2);


    std::srand(static_cast<unsigned int>(std::time(0))); 
    std::string inputLine;


    while (std::getline(std::cin, inputLine)) {
        DWORD written;
        if (std::rand() % 100 < 80) { 
            WriteFile(hWritePipe1, inputLine.c_str(), static_cast<DWORD>(inputLine.length()), &written, NULL);
            WriteFile(hWritePipe1, "\n", 1, &written, NULL); 
        } else { 
            WriteFile(hWritePipe2, inputLine.c_str(), static_cast<DWORD>(inputLine.length()), &written, NULL);
            WriteFile(hWritePipe2, "\n", 1, &written, NULL); 
        }
    }


    CloseHandle(hWritePipe1);
    CloseHandle(hWritePipe2);


    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);


    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);

    return 0;
}
