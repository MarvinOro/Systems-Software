// Marvin Orozco
// 9/15/2023
// Professor: Montagne
// Secion: M/W 7:30 - 8:45
#include <stdio.h>
#include <stdlib.h>

int ARRAY_SIZE = 512;
// base function to find the base of a certain L levels below the top of the stack
int base(int BP, int L, int *pas);
// helper print function that prints to the console
void print(int I, int L, int M, int PC, int BP, int SP, int size, int ar, int initialLoc, int *AR, int *pas);

int main(int argc, char *argv[]) {

    FILE *file;
    char *fileName;

    if (argc < 2) {
        printf("Filename missing!\n");
        exit(1);
    } else {
        // assign file name from passed argument
        fileName = argv[1];
    }

    file = fopen(fileName, "r");
    int IR[3]; // IR[0] represents IR.OP, IR[1] = IR.L, IR[2] = IR.M
    int pas[ARRAY_SIZE];
    int BP;         // base pointer
    int SP;         // stack pointer
    int PC = 0;     // program counter
    int HALT = 1;   // flag to know when to halt the program
    int size = 0;   // size of the stack
    int initialLoc; // will store where the BP is
    /*
     * MIGHT NOT NEED TO USE THE 2 VARIABLES BELOW
     */
    int AR[ARRAY_SIZE]; // array that holds the size of every AR
    int ar = 0;         // holds the amount of ARs inside of the stack - 1
    int input;          // holds the value of an int in a file
    int pos = 0;        // used for keeping track of positioning

    for (int i = 0; i < ARRAY_SIZE; i++) {
        pas[i] = 0;
    }

    while (!feof(file)) {
        fscanf(file, "%d", &input);
        pas[pos] = input;
        pos++;
    }

    fclose(file);

    BP = pos; // intitializing BP to the index of the last integer in the pas[] array
    SP = BP - 1; // initializes SP
    initialLoc = BP;
    printf("\t\tPC\tBP\tSP\tstack\n");
    printf("Initial values:\t%d\t%d\t%d\n\n", PC, BP, SP);

    while (HALT != 0) { // loop that continues until SYS 0 3 is reached
        IR[0] = pas[PC]; // sets the OP, L, and M respectively
        IR[1] = pas[PC + 1];
        IR[2] = pas[PC + 2];
        PC = PC + 3; // increments PC by 3 to properly keep track
        switch (IR[0]) { // switch argument that looks at the OP and goes to the corresponding case case 1: // LIT
            SP++;                  //
            pas[SP] = IR[2];
            size++;
            print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
            break;
        case 2: // OPR
            switch (IR[2]) {
                case 0: // RTN
                    SP = BP - 1;
                    BP = pas[SP + 2];
                    PC = pas[SP + 3];
                    ar--;
                    printf("RTN ");
                    print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
                    break;
                // for the operators, SP-1 'operator' SP, and decrement SP
                case 1: // ADD
                    pas[SP - 1] = pas[SP - 1] + pas[SP];
                    SP--;
                    printf("ADD ");
                    print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
                    break;
                case 2: // SUB
                    pas[SP - 1] = pas[SP - 1] - pas[SP];
                    SP--;
                    printf("SUB ");
                    print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
                    break;
                case 3: // MUL
                    pas[SP - 1] = pas[SP - 1] * pas[SP];
                    SP--;
                    printf("MUL ");
                    print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
                    break;
                case 4: // DIV
                    pas[SP - 1] = pas[SP - 1] / pas[SP];
                    SP--;
                    printf("DIV ");
                    print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
                    break;
                case 5: // EQL
                    pas[SP - 1] = pas[SP - 1] == pas[SP];
                    SP--;
                    printf("EQL ");
                    print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
                    break;
                case 6: // NQL
                    pas[SP - 1] = pas[SP - 1] != pas[SP];
                    SP--;
                    printf("NQL ");
                    print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
                    break;
                case 7: // LSS
                    pas[SP - 1] = pas[SP - 1] < pas[SP];
                    SP--;
                    printf("LSS ");
                    print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
                    break;
                case 8: // LEQ
                    pas[SP - 1] = pas[SP - 1] <= pas[SP];
                    SP--;
                    printf("LEQ ");
                    print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
                    break;
                case 9: // GTR
                    pas[SP - 1] = pas[SP - 1] > pas[SP];
                    SP--;
                    printf("GTR ");
                    print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
                    break;
                case 10: // GEQ
                    pas[SP - 1] = pas[SP - 1] >= pas[SP];
                    SP--;
                    printf("GEQ ");
                    print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
                    break;
            }
            break;

        case 3: // LOD
            SP++;
            pas[SP] = pas[base(BP, IR[1], pas) + IR[2]];
            print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
            break;
        case 4: // STO
            pas[base(BP, IR[1], pas) + IR[2]] = pas[SP];
            SP--;
            print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
            break;
        case 5:                                 // CAL
            pas[SP + 1] = base(BP, IR[1], pas); // initializing the values for the new AR
            pas[SP+2] = BP;
            pas[SP + 3] = PC;
            BP = SP + 1; // the new BP will be the value on top of the current stack
            PC = IR[2]; // PC = M
            ar++;           // increments the amount of AR's
            print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
            AR[ar - 1] = BP; // AR array that holds the position for where we want '|' break;
        case 6: // INC
            size++;
            SP = SP + IR[2]; // sets the size SP M positions higher
            print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
            break;
        case 7:         // JMP
            PC = IR[2]; // sets the PC to M
            print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
            break;
        case 8: // JPC
            if (pas[SP] == 0)
                PC = IR[2];
            SP--;
            print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
            break;
        case 9: // SYS
            switch (IR[2]) {
                case 1: // prints the value to the screen and pops the stack
                    printf("Output result is: %d\n", pas[SP]);
                    SP--; // decrement SP
                    print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
                    break;
                case 2: // pushes a user inputted value into the stack
                    SP++; // increment SP
                    printf("Please Enter an Integer: ");
                    scanf("%d", &pas[SP]);
                    print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
                    break;
                case 3:       // case for stopping the program
                    HALT = 0; // sets the flag = 0 to end the program
                    print(IR[0], IR[1], IR[2], PC, BP, SP, size, ar, initialLoc, AR, pas);
                    break;
            }
            break;
        }
    }
    return 0;
}

// universal print functions that will
void print(int I, int L, int M, int PC, int BP, int SP, int size, int ar, int initialLoc, int *AR, int *pas)
{
    int arTemp = ar; // temporarily holds the value of the ar for manipulation
    int j = 0;
    switch (I) { // translates the OP code and prints it
        case 1: printf("LIT "); break;
        case 2: break;
        case 3: printf("LOD "); break;
        case 4: printf("STO "); break;
        case 5: printf("CAL "); break;
        case 6: printf("INC "); break;
        case 7: printf("JMP "); break;
        case 8: printf("JPC "); break;
        case 9: printf("SYS "); break;
    }

    if (size == 0) { // case for the initial values print
        printf("%d\t%d\t%d\t%d\t%d\n", L, M, PC, BP, SP);
        return;
    }

    printf("%d\t%d\t%d\t%d\t%d\t", L, M, PC, BP, SP);

    if (arTemp != 0) { // checks to see if there are ARs in the stack
        for (int i = initialLoc; i < SP + 1; i++) {
            if (i == AR[j]) { // will print the '|' at the desired location
                printf("| ");
                j++;
            }

            printf("%d ", pas[i]);
        }
    } else { // if there are no ARs then will just print the current stack
        for (int i = BP; i < SP + 1; i++) {
            printf("%d ", pas[i]);
        }
    }

    printf("\n");
}

// given function that will find the BP for L levels down
int base(int BP, int L, int *pas) {
    int arb = BP;
    while (L > 0) {
        arb = pas[arb];
        L--;
    }
    return arb;
}