#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

std::string removeVowels(const std::string &input) {
    const std::string vowels = "aeiouAEIOU";
    std::string result;
    for (char ch : input) {
        if (vowels.find(ch) == std::string::npos) {
            result += ch;
        }
    }
    return result;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "File name argument is missing." << std::endl;
        return 1;
    }
    std::ofstream outFile(argv[1], std::ofstream::out);

    // Подготовка для чтения из стандартного ввода (канала)
    char buffer[256];
    DWORD bytesRead;

    while (true) {
        BOOL result = ReadFile(GetStdHandle(STD_INPUT_HANDLE), buffer, 255, &bytesRead, NULL);
        if (!result || bytesRead == 0) break; // Выход из цикла, если чтение завершилось
        buffer[bytesRead] = '\0'; // Обеспечение нуль-терминированной строки
        std::string output = removeVowels(std::string(buffer));
        if (outFile.is_open()) {
            outFile << output << std::endl;
        } else {
            std::cerr << "Failed to open file for writing." << std::endl;
            return 1;
        }
    }
    outFile.close();

    // Закрытие стандартного ввода
    CloseHandle(GetStdHandle(STD_INPUT_HANDLE));

    return 0;
}