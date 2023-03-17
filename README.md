# ManoAssembler

## Description
Assembles assembly code into machine code for use on the Mano Microprocessor created in Project 1 for EGCP 3210: Computer Architecture. The microprocessor can be simulated using the `ManoMachine.cdl` file and CedarLogic version 2.0.

## Compilation
Using the g++ compliler, navigate to the top repository directory and run:
```
...\ManoAssembler> g++ src/main.cpp -o asm.exe
```

## Usage
The assembler can be used from the command line by running:
```
...\ManoAssembler> ./asm [path to file]
```
In the case of this repository, compiling `tests/testFile1.asm` in the tests folder would look like:
```
...\ManoAssembler> ./asm tests/testFile1.asm
```
The output .bin and .sym files will be saved with the same name and stored in the same location as the input file.