#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

string toHex(int i) {
    stringstream ss;
    ss << hex << uppercase << i;
    return ss.str().substr(4, 4);
}

int main(int argc, char** argv) {
    map<string, int> symbolTable;
    string inFileName = "test.asm";

    map<string, int> instMap = {
        {"AND", 0x0000}, {"ADD", 0x1000}, {"LDA", 0x2000}, {"STA", 0x3000},
        {"BUN", 0x4000}, {"BSA", 0x5000}, {"ISZ", 0x6000}, {"CLA", 0x7800},
        {"CLE", 0x7400}, {"CMA", 0x7200}, {"CME", 0x7100}, {"CIR", 0x7080}, 
        {"CIL", 0x7040}, {"INC", 0x7020}, {"SPA", 0x7010}, {"SNA", 0x7008},
        {"SZA", 0x7004}, {"SZE", 0x7002}, {"HLT", 0x7001}, {"INP", 0xF800},
        {"OUT", 0xF400}, {"SKI", 0xF200}, {"SKO", 0xF100}, {"ION", 0xF080}, 
        {"IOF", 0xF040}
    };

    int lineCounter = 0x000;

    ifstream inFile(inFileName);
    if (!inFile) {
        cerr << "Error opening input file." << endl;
        return 1;
    }

    string line;
    getline(inFile, line);
    if (line.substr(5, 3) == "ORG") {
        lineCounter = stoi(line.substr(9, 3), nullptr, 16);
    }
    else {
        cerr << "Memory location not initialized" << endl;
        return 2;
    }

    ofstream outSym("test.sym");
    while(getline(inFile, line)) {
        string label = line.substr(0, 3);
        if (label != "   ") {
            symbolTable.insert(pair<string, int>(label, lineCounter));
            outSym << label << ": " << hex << uppercase << lineCounter << endl;
        }
        
        if (line.substr(5, 3) == "ORG") {
            lineCounter = stoi(line.substr(9, 3), nullptr, 16);
        }
        else {
            lineCounter++;
        }

    }
    outSym.close();

    inFile.close();
    inFile.open(inFileName);

    ofstream outBin("test.bin");
    lineCounter = 0;

    outBin << hex << uppercase;
    while(getline(inFile, line)) {
        string inst = line.substr(5, 3);
        if (inst == "ORG") {
            lineCounter = stoi(line.substr(9, 3), nullptr, 16);
            continue;
        }
        else if (inst == "HEX") {
            outBin << lineCounter << ": " << stoi(line.substr(9, 4), nullptr, 16) << endl;
        }
        else if (inst == "DEC") {
            outBin << lineCounter << ": " << (short)stoi(line.substr(9, 4)) << endl;
        }
        else if (inst == "END") {
            break;
        }

        auto instIter = instMap.find(inst);
        if (instIter != instMap.end()) {
            int opCode = instIter->second;

            if (opCode < 0x7000) {
                string operand = line.substr(9, 3);
                auto symIter = symbolTable.find(operand);
                if (symIter != symbolTable.end()) {
                    opCode += symIter->second;
                }

                if (line.length() == 14 && line.at(13) == 'I') {
                    opCode += 0x8000;
                }
            }

            outBin << lineCounter << ": " << opCode << endl;
        }

        lineCounter++;
    }

    inFile.close();
    outBin.close();

    return 0;
}