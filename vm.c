/*
Assignment :
vm.c - Implement a P - machine virtual machine
Authors : Rohaan Mansoor; Nathan Miriello
Language : C ( only )
To Compile :
gcc -O2 -Wall -std=c11 -o vm vm.c
To Execute (on Eustis):
./ vm input.txt
where:
input.txt is the name of the file containing PM /0 instructions ;
each line has three integers ( OP L M )
Notes :
- Implements the PM /0 virtual machine described in the homework
instructions .
- No dynamic memory allocation or pointer arithmetic .
- Does not implement any VM instruction using a separate function .
- Runs on Eustis .
Class : COP 3402 - Systems Software - Fall 2025
Instructor : Dr . Jie Lin
Due Date : Friday , September 12 th , 2025
*/
#include <stdio.h>
#include <stdlib.h>

int base(int pas[], int bp, int l) {
    int arbase = bp; // activation record base
    while (l > 0) {
        arbase = pas[arbase]; // static link (SL) is always at offset 1
        l--;
    }
    return arbase;
}

void print(int op, int l, int m, int pas[], int pc, int bp, int sp) {
    char opcode[4];

    // Decode opcode string
    switch (op) {
        case 1:
            sprintf(opcode, "LIT");
            break;
        case 2:
            switch (m) {
                case 0:
                    sprintf(opcode, "RTN");
                    break;
                case 1:
                    sprintf(opcode, "ADD");
                    break;
                case 2:
                    sprintf(opcode, "SUB");
                    break;
                case 3:
                    sprintf(opcode, "MUL");
                    break;
                case 4:
                    sprintf(opcode, "DIV");
                    break;
                case 5:
                    sprintf(opcode, "EQL");
                    break;
                case 6:
                    sprintf(opcode, "NEQ");
                    break;
                case 7:
                    sprintf(opcode, "LSS");
                    break;
                case 8:
                    sprintf(opcode, "LEQ");
                    break;
                case 9:
                    sprintf(opcode, "GTR");
                    break;
                case 10:
                    sprintf(opcode, "GEQ");
                    break;
            }
            break;
        case 3:
            sprintf(opcode, "LOD");
            break;
        case 4:
            sprintf(opcode, "STO");
            break;
        case 5:
            sprintf(opcode, "CAL");
            break;
        case 6:
            sprintf(opcode, "INC");
            break;
        case 7:
            sprintf(opcode, "JMP");
            break;
        case 8:
            sprintf(opcode, "JPC");
            break;
        case 9:
            sprintf(opcode, "SYS");
            break;
    }

    printf("%3s %5d %7d %4d %5d %5d", opcode, l, m, pc, bp, sp);

    // Print stack
    printf("   ");
    for (int i=sp; i >= bp; i--) {
        printf(" %d", pas[i]);
    }

    printf("\n");
}

int main(int argc, char* argv[]) {
    // Read input file
    FILE *input = fopen(argv[1], "r");
    if (!input) {
        printf("Unable to open file!\n");
        return 1;
    }

    // Initialize PAS and registers
    int pas[500] = {0};
    int pc = 499; // program counter
    int bp = pc - (argc-1);
    int sp = pc - argc;

    // Print header
    printf("%9c %7c %5s %4s %4s %7s\n", 'L', 'M', "PC", "BP", "SP", "stack");
    printf("Initial values: %8d %4d %4d\n", pc, bp, sp);

    int instruct = pc;
    int l = pc-1;
    int m = pc-2;
    pc = bp;

    // Load instructions into PAS
    while (fscanf(input, "%d %d %d", &instruct, &l, &m) == 3) {
        pas[pc] = instruct;
        pas[pc-1] = l;
        pas[pc-2] = m;
        pc -= 3;
    }
    pc = bp;

    int halt = 0;
    while (!halt) {
        // Fetch Step
        int op = pas[pc];
        l = pas[pc-1];
        m = pas[pc-2];
        pc -= 3;
        // Decode Step
        switch (op) {
            // LIT (Literal Push)
            case 1:
                sp--;
                pas[sp] = pas[m];
                break;
            // OPR (Operation Code)
            case 2:
                switch (m) {
                    // RTN (Return)
                    case 0:
                        sp = bp + 1;
                        bp = pas[m];
                        pc = pas[sp-3];
                        break;
                    // ADD
                    case 1:
                        pas[sp+1] = pas[sp+1] + pas[sp];
                        sp++;
                        break;
                    // SUB
                    case 2:
                        pas[sp+1] = pas[sp+1] - pas[sp];
                        sp++;
                        break;
                    // MUL
                    case 3:
                        pas[sp+1] = pas[sp+1] * pas[sp];
                        sp++;
                        break;
                    // DIV
                    case 4:
                        pas[sp+1] = pas[sp+1] / pas[sp];
                        sp++;
                        break;
                    // EQL
                    case 5:
                        pas[sp+1] = (pas[sp+1] == pas[sp]) ? 1 : 0;
                        sp++;
                        break;
                    // NEQ
                    case 6:
                        pas[sp+1] = (pas[sp+1] != pas[sp]) ? 1 : 0;
                        sp++;
                        break;
                    // LSS
                    case 7:
                        pas[sp+1] = (pas[sp+1] < pas[sp]) ? 1 : 0;
                        sp++;
                        break;
                    // LEQ
                    case 8:
                        pas[sp+1] = (pas[sp+1] <= pas[sp]) ? 1 : 0;
                        sp++;
                        break;
                    // GTR
                    case 9:
                        pas[sp+1] = (pas[sp+1] > pas[sp]) ? 1 : 0;
                        sp++;
                        break;
                    // GEQ
                    case 10:
                        pas[sp+1] = (pas[sp+1] >= pas[sp]) ? 1 : 0;
                        sp++;
                        break;
                }
                break;
                // LOD (Load)
                case 3:
                    sp--;
                    pas[sp] = pas[base(pas, bp, pas[pc-1]) - pas[pc-2]];
                    break;
                // STO (Store)
                case 4:
                    pas[base(pas, bp, pas[pc-1]) - pas[pc-2]] = pas[sp];
                    sp++;
                    break;
                // CAL (Call Procedure)
                case 5:
                    pas[sp-1] = base(pas, bp, pas[pc-1]);
                    pas[sp-2] = bp;
                    pas[sp-3] = pc;
                    bp = sp - 1;
                    pc = pas[pc-2];
                    break;
                // INC (Increment SP)
                case 6:
                    sp -= pas[pc-2];
                    break;
                // JMP (Jump)
                case 7:
                    pc = pas[pc-2];
                    break;
                // JPC (Jump Conditional)
                case 8:
                    if (pas[sp] == 0)
                        pc = pas[pc-2];
                    sp++;
                    break;
                // SYS (Output, Input, Halt)
                case 9:
                    switch (pas[pc-2]) {
                        // Output
                        case 1:
                            printf("Output result is: %d\n", pas[sp]);
                            sp++;
                            break;
                        // Input
                        case 2:
                            sp--;
                            printf("Please Enter an Integer: ");
                            scanf("%d", &pas[sp]);
                            break;
                        // Halt
                        case 3:
                            halt = 1;
                            break;
                    }
                    break;
        }
        print(op, l, m, pas, pc, bp, sp);
    }
    return 0;
}
