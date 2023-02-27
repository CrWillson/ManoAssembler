#include <iostream>
#include <fstream>
#include <map>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    map<string, int> symbolTable;
    string inFile = "../test.asm";

    string instText[] = {"AND", "ADD", "LDA", "STA", "BUN", "BSA", "ISZ",
                         "CLA", "CLE", "CMA", "CME", "CIR", "CIL", "INC",
                         "SPA", "SNA", "SZA", "SZE", "HLT", "INP", "OUT",
                         "SKI", "SKO", "ION", "IOF", "ORG", "END", "HEX",
                         "DEC"};
    
    int instHex[] = {0x0000, 0x1000, 0x2000, 0x3000, 
                     0x4000, 0x5000, 0x6000, 0x7000,
                     0xF000};

    int lineCounter = 0x000;

    ifstream inStream(inFile);
    if (!inStream) {
        cerr << "Error opening input file." << endl;
        return 1;
    }

    string line;
    getline(inStream, line);
    if (line.substr(5, 3) == "ORG") {
        lineCounter = stoi(line.substr(9, 3), nullptr, 16);
    }
    else {
        cerr << "Memory location not initialized" << endl;
        return 2;
    }

    while(getline(inStream, line)) {
        string label = line.substr(0, 3);
        if (label != "   ") {
            symbolTable.insert(pair<string, int>(label, lineCounter));
        }
        
        if (line.substr(5, 3) == "ORG") {
            lineCounter = stoi(line.substr(9, 3), nullptr, 16);
        }
        else {
            lineCounter++;
        }

    }

    return 0;
}