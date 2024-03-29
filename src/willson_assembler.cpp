#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>

/**
 * Project: ManoAssembler
 * Author:  Caleb Willson
 * Date:    3/17/23
 * 
 * Takes an assembly file given by command line argument and assembles a
 * .bin file containing binary code for the ManoMachine and a .sym file
 * containing the symbol table.
*/

using namespace std;

int main(int argc, char** argv) {
    // initial system variables
    map<string, unsigned short> symbolTable;
    string inFileName;
    string outFileSym;
    string outFileBin;

    map<string, unsigned short> instMap = {
        {"AND", 0x0000}, {"ADD", 0x1000}, {"LDA", 0x2000}, {"STA", 0x3000},
        {"BUN", 0x4000}, {"BSA", 0x5000}, {"ISZ", 0x6000}, {"CLA", 0x7800},
        {"CLE", 0x7400}, {"CMA", 0x7200}, {"CME", 0x7100}, {"CIR", 0x7080}, 
        {"CIL", 0x7040}, {"INC", 0x7020}, {"SPA", 0x7010}, {"SNA", 0x7008},
        {"SZA", 0x7004}, {"SZE", 0x7002}, {"HLT", 0x7001}, {"INP", 0xF800},
        {"OUT", 0xF400}, {"SKI", 0xF200}, {"SKO", 0xF100}, {"ION", 0xF080}, 
        {"IOF", 0xF040}, {"ORG", 0xFFFF}, {"HEX", 0xFFFF}, {"DEC", 0xFFFF},
        {"END", 0xFFFF}
    };

    int lineCounter = 0x000;
    int realLineCounter = 1;
    string line;

    // start execution timer
    auto start = chrono::high_resolution_clock::now();

    // get input file name
    if (argc < 2) {
        cerr << "Usage: ./asm [path to file]";
        return 1;
    }

    inFileName = argv[1];
    outFileSym = inFileName.substr(0, inFileName.rfind(".")) + ".sym";
    outFileBin = inFileName.substr(0, inFileName.rfind(".")) + ".bin";

    ifstream inFile(inFileName);
    if (!inFile) {
        cerr << "Error opening input file." << endl;
        return 1;
    }

    // first pass to load the symbol table
    ofstream outSym(outFileSym);
    if (!outSym) {
        cerr << "Error opening output symbol file" << endl;
        return 1;
    }

    while(getline(inFile, line)) {
        string label = line.substr(0, 3);
        if (label != "   ") {
            if (symbolTable.find(label) == symbolTable.end()) {
                symbolTable.insert(pair<string, int>(label, lineCounter));
                outSym << label << ": " << hex << uppercase << lineCounter << endl;
            }
            else {
                cerr << "Label \"" << label << "\" defined twice at line " << realLineCounter << endl;
                return 1;
            }
        }
        
        if (line.substr(5, 3) == "ORG") {
            lineCounter = stoi(line.substr(9, 3), nullptr, 16);
        }
        else {
            lineCounter++;
        }
        realLineCounter++;
    }
    outSym.close();

    // reset the input file
    inFile.close();
    inFile.open(inFileName);

    // second pass
    ofstream outBin(outFileBin);
    if (!outBin) {
        cerr << "Error opening output bin file" << endl;
        return 1;
    }

    lineCounter = 0;
    realLineCounter = 1;
    outBin << hex << uppercase;
    while(getline(inFile, line)) {
        string inst = line.substr(5, 3);

        // check if the instruction matches a known instruction
        auto instIter = instMap.find(inst);
        if (instIter != instMap.end()) {
            unsigned short opCode = instIter->second;

            // get the operand from the symbol table
            if (opCode < 0x7000) {
                if (line.length() < 12) {
                    cerr << "Invalid or missing operand at line " << realLineCounter << endl;
                    return 1;
                }
                string operand = line.substr(9, 3);

                // find the label's corresponding memory location
                auto symIter = symbolTable.find(operand);
                if (symIter != symbolTable.end()) {
                    opCode += symIter->second;
                }
                else {
                    cerr << "Undeclared label \"" << operand << "\" at line " << realLineCounter << endl;
                    return 1;
                }

                // check for indirect memory addressing
                if (line.length() >= 14 && line.at(13) == 'I') {
                    opCode += 0x8000;
                }
            }

            // special cases for pseudocodes
            if (instIter->first == "ORG") {
                lineCounter = stoi(line.substr(9, 3), nullptr, 16);
                realLineCounter++;
                continue;
            }
            else if (instIter->first == "HEX") {
                opCode = stoi(line.substr(9, 4), nullptr, 16);
            }
            else if (instIter->first == "DEC") {
                opCode = stoi(line.substr(9, 4));
            }
            else if (instIter->first == "END") {
                break;
            }

            // print to the bin file
            outBin << lineCounter << ": " << opCode << endl;
        }
        else {
            cerr << "Unknown instruction \"" << inst << "\" at line " << realLineCounter << endl;
            return 1;
        }

        lineCounter++;
        realLineCounter++;
    }

    inFile.close();
    outBin.close();

    // stop execution timer and print results
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    cout << "Assembled \"" << inFileName << "\" using WillsonAssembler v1." << endl;
    cout << "Execution time: " << duration.count() << " microseconds" << endl;
    
    return 0;
}