#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include "Value.h"

class Line {
public:
    std::string mnc;  // Mnemonic
    std::string lineText;

    Line(std::string lineText, std::string mnc) {
        this->lineText = lineText;
        this->mnc = mnc;
    }
};

// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
}

int main() {

    Register::registers[0].set(1);
    int sp; // stack pointer
    int pc; // program counter

    std::string myText;
    std::ifstream CodeFile("arm_examples/factorial.txt");
    if (!CodeFile.is_open()) {
        return 1;
    }
    std::vector<Line> lines;
    std::string splitOn = " ";
    while (std::getline(CodeFile, myText)) {
        ltrim(myText); // removes whitespace from start of line
        int current_index = myText.find(splitOn);
        std::string mnc;
        if ((current_index < 0 && myText[myText.length() - 1] == ':') ||
            (current_index > 0 && myText[current_index - 1] == ':')) {
            mnc = "label";
        } else {
            mnc = myText.substr(0, current_index);
        }
        lines.emplace_back(Line(myText, mnc));
    }
    for (auto & line : lines) {
        std::cout << line.mnc << ": " << line.lineText << "\n";
    }

// Close the file
    CodeFile.close();
    return 0;
}
