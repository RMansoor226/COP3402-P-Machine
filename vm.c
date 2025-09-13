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

#define STACK_SIZE 500
#define MAX_AR 100

int base(int pas[], int bp, int l) {
    int arbase = bp; // activation record base
    while (l > 0) {
        arbase = pas[arbase]; // static link (SL) is always at offset 1
        l--;
    }
    return arbase;
}

void print(int op, int l, int m, int pas[], int pc, int bp, int sp, int instructionIndex) {
    char opcode[5] = "    ";

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
    }

    printf("%3s %5d %8d %5d %4d %4d   ", opcode, l, m, pc, bp, sp);

    int currBP = bp;
    int nextBP;
    int curr = sp;

    while (curr <= instructionIndex) {
        if (curr == currBP) {
            if (curr != sp)
                printf("| ");
            nextBP = pas[currBP];
        }
        printf(" %d", pas[curr]);
        curr++;

        if (curr == nextBP && nextBP != 0) {
            currBP = nextBP;
        }
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

    // Initalize PAS and instruction argument variables
    int pas[STACK_SIZE] = {0};
    int op, l, m;
    int instructionIndex = STACK_SIZE - 1;
    int instructionCount = 0;

    // Load instructions into PAS
    while (fscanf(input, "%d %d %d", &op, &l, &m) == 3) {
        pas[instructionIndex] = op;
        pas[instructionIndex-1] = l;
        pas[instructionIndex-2] = m;
        instructionIndex -= 3;
        instructionCount++;
    }
    fclose(input);

    int pc = STACK_SIZE-1; // program counter
    int sp = STACK_SIZE - (instructionCount * 3);
    int bp = sp - 1;

    // Print header
    printf("%9c %7c %5s %4s %4s %7s\n", 'L', 'M', "PC", "BP", "SP", "stack");
    printf("Initial values: %8d %4d %4d\n", pc, bp, sp);

    int halt = 0;
    while (!halt) {
        // Fetch Step
        op = pas[pc];
        l = pas[pc-1];
        m = pas[pc-2];
        pc -= 3;

        // Decode Step
        switch (op) {
            // LIT (Literal Push)
            case 1:
                sp--;
                pas[sp] = m;
                break;
            // OPR (Operation Code)
            case 2:
                switch (m) {
                    // RTN (Return)
                    case 0:
                        sp = bp + 1;
                        bp = pas[sp-2];
                        pc = pas[sp-3];
                        break;
                    // ADD
                    case 1:
                        pas[sp] += pas[sp+1];
                        sp++;
                        break;
                    // SUB
                    case 2:
                        pas[sp] -= pas[sp+1];
                        sp++;
                        break;
                    // MUL
                    case 3:
                        pas[sp] *= pas[sp+1];
                        sp++;
                        break;
                    // DIV
                    case 4:
                        pas[sp] /= pas[sp+1];
                        sp++;
                        break;
                    // EQL
                    case 5:
                        pas[sp] = (pas[sp] == pas[sp+1]) ? 1 : 0;
                        sp++;
                        break;
                    // NEQ
                    case 6:
                        pas[sp] = (pas[sp] != pas[sp+1]) ? 1 : 0;
                        sp++;
                        break;
                    // LSS
                    case 7:
                        pas[sp] = (pas[sp] < pas[sp+1]) ? 1 : 0;
                        sp++;
                        break;
                    // LEQ
                    case 8:
                        pas[sp] = (pas[sp] <= pas[sp+1]) ? 1 : 0;
                        sp++;
                        break;
                    // GTR
                    case 9:
                        pas[sp] = (pas[sp] > pas[sp+1]) ? 1 : 0;
                        sp++;
                        break;
                    // GEQ
                    case 10:
                        pas[sp] = (pas[sp] >= pas[sp+1]) ? 1 : 0;
                        sp++;
                        break;
                }
                break;
                // LOD (Load)
                case 3:
                    sp--;
                    pas[sp] = pas[base(pas, bp, l) + m];
                    break;
                // STO (Store)
                case 4:
                    pas[base(pas, bp, l) + m] = pas[sp];
                    sp++;
                    break;
                // CAL (Call Procedure)
                case 5:
                    pas[sp-1] = base(pas, bp, l);
                    pas[sp-2] = bp;
                    pas[sp-3] = pc;
                    bp = sp - 1;
                    pc = (STACK_SIZE - 1) - m;
                    break;
                // INC (Increment SP)
                case 6:
                    sp -= m;
                    break;
                // JMP (Jump)
                case 7:
                    pc = (STACK_SIZE - 1) - m;
                    break;
                // JPC (Jump Conditional)
                case 8:
                    if (pas[sp] == 0)
                        pc = (STACK_SIZE - 1) - m;
                    sp++;
                    break;
                // SYS (Output, Input, Halt)
                case 9:
                    switch (m) {
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
        print(op, l, m, pas, pc, bp, sp, instructionIndex);
    }
    return 0;
}
