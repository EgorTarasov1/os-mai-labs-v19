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

    std::ofstream outFile(argv[1]);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return 1;
    }

    std::string inputLine;
    while (std::getline(std::cin, inputLine)) {
        std::string output = removeVowels(inputLine);
        outFile << output << std::endl;
    }

    outFile.close();
    return 0;
}
