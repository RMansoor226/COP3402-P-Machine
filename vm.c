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

void print(int argc, char** argv, int pas[], int pc, int bp, int sp) {
    char l = 'L';
    char m = 'M';
    char PC[] = "PC";
    char BP[] = "BP";
    char SP[] = "SP";
    char stack[] = "stack";
    printf("%9c %7c %5s %4s %4s %7s\n", l, m, PC, BP, SP, stack);
    printf("Initial values: %8d %4d %4d\n", 499, 439, 440);
    for (int i=1; i < argc; i+=3) {
        if (atoi(argv[i]) == '1') {
            printf("JMP %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
        }
        if (atoi(argv[i]) == '2') {
            if (argv[i][2] == '0') {
                printf("RTN %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
            }
            if (argv[i][2] == '1') {
                printf("ADD %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
            }
            if (argv[i][2] == '2') {
                printf("SUB %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
            }
            if (argv[i][2] == '3') {
                printf("MUL %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
            }
            if (argv[i][2] == '4') {
                printf("DIV %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
            }
            if (argv[i][2] == '5') {
                printf("EQL %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
            }
            if (argv[i][2] == '6') {
                printf("NEQ %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
            }
            if (argv[i][2] == '7') {
                printf("LSS %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
            }
            if (argv[i][2] == '8') {
                printf("LEQ %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
            }
            if (argv[i][2] == '9') {
                printf("GTR %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
            }
            if (argv[i][2] == '10') {
                printf("GEQ %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
            }
        }
        if (atoi(argv[i]) == '3') {
            printf("LOD %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
        }
        if (atoi(argv[i]) == '4') {
            printf("STO %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
        }
        if (atoi(argv[i]) == '5') {
            printf("CAL %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
        }
        if (atoi(argv[i]) == '6') {
            printf("INC %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
        }
        if (atoi(argv[i]) == '7') {
            printf("JMP %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
        }
        if (atoi(argv[i]) == '8') {
            printf("JPC %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
        }
        if (atoi(argv[i]) == '9') {
            printf("SYS %5d %7d %4d %4d %4d", atoi(argv[i+1]), atoi(argv[i+2]), pc, bp, sp);
        }
    }
}

int main(int argc, char* argv[]) {
    int pas[500] = {0};
    int pc = 499; // program counter
    int bp = pc - (3 * argc);
    int sp = pc - (3 * argc) + 1;
    // Step 1: Read
    for (int i=1; i < argc; i+=3) {
        // Fetch Step
        pas[pc] = atoi(argv[i]); // IR.OP <-- PAS[PC]
        pas[pc-1] = atoi(argv[i+1]); // IR.L <-- PAS[PC-1]
        pas[pc-2] = atoi(argv[i+2]); // IR.M <-- PAS[PC-2]
        pc -= 3;
        // Decode Step
        // LIT (Literal Push)
        if (pc == 1) {
            sp++;
            pas[sp] = pas[pc-2];
        }
        // OPR (Operation Code)
        if (pc == 2) {
            // RTN (Return)
            if (pas[pc-2] == 0) {
                sp = bp + 1;
                bp = pas[sp-2];
                pc = pas[sp-3];
            }
            // ADD
            if (pas[pc-2] == 1) {
                pas[sp+1] = pas[sp+1] + pas[sp];
                sp++;
            }
            // SUB
            if (pas[pc-2] == 2) {
                pas[sp+1] = pas[sp+1] - pas[sp];
                sp++;
            }
            // MUL
            if (pas[pc-2] == 3) {
                pas[sp+1] = pas[sp+1] * pas[sp];
                sp++;
            }
            // DIV
            if (pas[pc-2] == 4) {
                pas[sp+1] = pas[sp+1] / pas[sp];
                sp++;
            }
            // EQL
            if (pas[pc-2] == 5) {
                pas[sp+1] = (pas[sp+1] == pas[sp]) ? 1 : 0;
                sp++;
            }
            // NEQ
            if (pas[pc-2] == 6) {
                pas[sp+1] = (pas[sp+1] != pas[sp]) ? 1 : 0;
                sp++;
            }
            // LSS
            if (pas[pc-2] == 7) {
                pas[sp+1] = (pas[sp+1] < pas[sp]) ? 1 : 0;
                sp++;
            }
            // LEQ
            if (pas[pc-2] == 8) {
                pas[sp+1] = (pas[sp+1] <= pas[sp]) ? 1 : 0;
                sp++;
            }
            // GTR
            if (pas[pc-2] == 9) {
                pas[sp+1] = (pas[sp+1] > pas[sp]) ? 1 : 0;
                sp++;
            }
            // GEQ
            if (pas[pc-2] == 10) {
                pas[sp+1] = (pas[sp+1] >= pas[sp]) ? 1 : 0;
                sp++;
            }
        }
        // LOD (Load)
        if (pc == 3) {
            sp--;
            pas[sp] = pas[base(pas, bp, pas[pc-1]) - pas[pc-2]];
        }
        // STO (Store)
        if (pc == 4) {
            pas[base(pas, bp, pas[pc-1]) - pas[pc-2]] = pas[sp];
            sp++;
        }
        // CAL (Call Procedure)
        // INC (Increment SP)
        // JMP (Jump)
        // JPC (Jump Conditional)
        // SYS (Output, Input, Halt)

    }
    print(argc, argv, pas, pc, bp, sp);
    return 0;
}
