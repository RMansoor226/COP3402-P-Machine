/*
Assignment :
vm . c - Implement a P - machine virtual machine
Authors : Rohaan Mansoor; Nathan Miriello
Language : C ( only )
To Compile :
gcc - O2 - Wall - std = c11 -o vm vm . c
To Execute ( on Eustis ) :
./ vm input . txt
where :
input . txt is the name of the file containing PM /0 instructions ;
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

int base(void);

void print(void) {
    char l = 'L';
    char m = 'M';
    char pc[] = "PC";
    char bp[] = "BP";
    char sp[] = "SP";
    char stack[] = "stack";
    printf("%9c %7c %5s %4s %4s %7s\n", l, m, pc, bp, sp, stack);
    printf("Initial values: %8d %4d %4d\n", 499, 439, 440);
}

int main(int argc, char** argv) {
    int pas[500] = {0};
    int pc = 499; // program counter
    int bp = pc - (3 * argc);
    int sp = pc - (3 * argc) + 1;
    // Step 1: Read
    for (int i=1; i < argc; i++) {
        // Fetch Step
        pas[pc] = (int) argv[i][0]; // IR.OP <-- PAS[PC]
        pas[pc-1] = (int) argv[i][1]; // IR.L <-- PAS[PC-1]
        pas[pc-2] = (int) argv[i][2]; // IR.M <-- PAS[PC-2]
        pc -= 3;
        // Decode Step
        // LIT (Literal Push)
        if (pc == 1) {
            sp++;
            pas[sp] = pas[pc-2];
        }
        // OPR (Operation Code)
        if (pc == 2) {

        }
        // LOD (Load)
        // STO (Store)

    }
    print();
    return 0;
}
