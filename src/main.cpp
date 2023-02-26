#include <iostream>
#include <fstream>
#include <map>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    map<string, int> symbolTable;
    string inFile = "";

    string instText[] = {"AND", "ADD", "LDA", "STA", "BUN", "BSA", "ISZ",
                         "CLA", "CLE", "CMA", "CME", "CIR", "CIL", "INC",
                         "SPA", "SNA", "SZA", "SZE", "HLT", "INP", "OUT",
                         "SKI", "SKO", "ION", "IOF", "ORG", "END", "HEX",
                         "DEC"};
    
    int instHex[] = {0x0000, 0x1000, 0x2000, 0x3000, 
                     0x4000, 0x5000, 0x6000, 0x7000,
                     0xF000};

    ifstream input(inFile);


    return 0;
}