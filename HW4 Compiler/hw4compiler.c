#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYMBOL_TABLE_SIZE 500
#define MAX_INST 100

int ARRAY_SIZE = 512;

typedef struct {
    int kind;      // const = 1, var = 2, proc = 3
    char name[11]; // name up to 11 chars
    int val;       // number (ASCII value)
    int level;     // L level
    int addr;      // M address
    int mark;      // to indiciate unavailable or deleted
} symbol;

typedef struct {
    int op; // operation
    int r;  // register
    int l;  // lexicographical level
    int m;  // modifier
} instruction;

symbol s[MAX_SYMBOL_TABLE_SIZE]; // declaration of the symbol struct array with max size 500
instruction inst[MAX_INST];

int caseHW3 = 0;     // keeps track of id[][] in STATEMENT
int cnt = 0;         // keeps track of tok[] in STATEMENT
int numHW3 = 0;      // keeps track of hash in STATEMENT
int sizeHW3 = 0;     // keeps track of how big the inst[] will be
int symCount = 0;    // used to keep the size of the symbol_table
int tok[100000];     // arr of token values
char id[100000][11]; // arr of identifier strings
int hash[100000];    // int arr to hold numbers
int curLvl = 0;
int lvl = 0;
int dx = 3;
int checkString(char *str);      // function to check what type of token a valid string is
int getInt(int *dig, int size);  // function that converts an array of ints to one int in order of array
int validity(char c);            // checks to see if character is a token or not
int symbolTableCheck(char *str); // checks the symbol_table struct to see if valid symbol is being used
void PROGRAM();
void BLOCK();
void CONST_DECLARATION();
int VAR_DECLARATION();
void STATEMENT();
void CONDITION();
void EXPRESSION();
void TERM();
void FACTOR();
void emit(int op, int l, int m);

int main(int argc, char *argv[])
{
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

    char c;           // char variable that will be used to iterate through the file
    char c2;          // will be used for certain cases when needing to keep track of previous char
    char str[12];     // char array used to store strings in file
    int dig[6];       // int array used to hold numbers in file
    int k = 0;        // used to set int values from char digits
    int size = 0;     // used to keep track of the size of the string
    int count = 0;    // counts how many tokens there are
    int flag = 0;     // flag used for digit error cases
    int num = 0;      // int that keeps track of the amount of identifier tokens there are
    int track = 0;    // used to track of identifiers when printing token list
    int case3 = 0;    // used to hold values of valid numbers in file
    int caseSize = 0; // used to keep track of the amount of valid numbers

    for (int i = 0; i < 11; i++) { // initializing array that will hold each digit in an int
        dig[i] = 0;
    } for (int i = 0; i < 11; i++) { // initializing char array
        str[i] = '\0';
    }
    c = fgetc(file);

    /*if(file == NULL){
    printf("Error Occurred while Opening the File");
    }else{
    while(c!=EOF){ // prints input text
    printf("%c", c);
    c = fgetc(file);
    }
    }*/

    rewind(file); // resets what element to look at
    c = fgetc(file);

    while (c != EOF) {
        if (isalpha(c)) {
            if (isspace(c) == 0) { // if statement that check to make sure c is not a space
                while (isspace(c) == 0 && c != EOF && validity(c) == 0) { // loop that runs until certain char are met
                    if (size > 11) { // if statement if length of string is > 11
                        while (isspace(c) == 0 && c != EOF && validity(c) == 0) { // same loop
                            c = fgetc(file); // ignores the rest of the string
                        }
                        break;
                    }
                    
                    str[size] = c; // initializes string elements w previous string
                    c = fgetc(file);
                    size++;
                }

                if (size <= 11) {
                    tok[count] = checkString(str); // sets array w value of str's token

                    if (tok[count] == 2) { // enters statement when str is an identifier
                        for (int i = 0; i < strlen(str); i++) { // loop that copies str into id array
                            id[num][i] = str[i];
                        }

                        num++; // incrementing number of identifiers
                    }

                    count++; // incrementing the size of tokens
                } else { // error statement
                    printf("Error : Identifier names cannot exceed 11 characters\n");
                    exit(1);
                }

                for (int i = 0; i < 11; i++) { // resets the values of str
                    str[i] = '\0';
                }

                size = 0; // resets the size of the string
            } else { // backup else statement
                // printf("\t%c\t2\n", c2);
                tok[count] = 2;
                count++;
                id[num][0] = c2;
                num++;
                c = fgetc(file);
            }
        } else if (isdigit(c)) { // must account for if followed up by a character
            while (validity(c) == 0 && c != EOF && isspace(c) == 0 && isdigit(c)) { // same loops as above
                if (isdigit(c)) { // if c = integer, then:
                    k = c - '0';
                    dig[size] = k;
                    size++;
                } else { // error statement when running into a char
                    printf("Error : Identifiers cannot begin with a digit\n");
                    exit(1);
                    while (c != EOF && c != ',' && c != ';' && isspace(c) == 0 && validity(c) != 1) {
                        c = fgetc(file);
                    }
                    flag = 1;
                    break;
                }

                if (size > 4) { // checks when number exceeds 5 digits
                    printf("Error : Numbers cannot exceed 5 digits\ n");
                    exit(1);
                    while (c != EOF && validity(c) == 0 && isspace(c) == 0 && isdigit(c)) {
                        c = fgetc(file); // ignores the rest of the string
                    }
                    flag = 1;
                    break;
                }

                c = fgetc(file);
            }

            if (size <= 5 && flag == 0) {
                case3 = getInt(dig, size); // sets number = case3
                // printf("\t%d\t3\n", getInt(dig, size)); // prints case
                hash[caseSize] = case3; // sets int array at caseSize to integer num
                caseSize++;             // increments the amount of ints in the array
                tok[count] = 3;         // token placement
                count++;
            }

            size = 0;
            flag = 0;

            for (int i = 0; i < 5; i++) { // resets the digits
                dig[i] = 0;
            }
        } else {

            if (isspace(c) != 0) { // ignores any space, new line, or tab char
                c = fgetc(file);

            } else if (c == '+') { // case 4
                // printf("\t+\t4\n");
                tok[count] = 4;
                count++;
                c = fgetc(file);

            } else if (c == '-') { // case 5
                // printf("\t%c\t5\n", c);
                tok[count] = 5;
                count++;
                c = fgetc(file);

            } else if (c == '*') { // case 6
                // printf("\t%c\t6\n", c);
                tok[count] = 6;
                count++;
                c = fgetc(file);

            } else if (c == '/') { // handles comments and case 7
                c2 = c;
                c = fgetc(file);

                if (c == '/') { // check if it is a comment

                    while (c != '\n') { // ignores every character until a new line is reached
                        c = fgetc(file);
                    }

                    c = fgetc(file);
                } else if (c == '*') { // case for comments that start with /*
                    c = fgetc(file);

                    while (c2 != '*' && c != '/') { // ignores every character until */ is reached
                        c2 = c;
                        c = fgetc(file);
                    }

                    c = fgetc(file);
                } else { // case 7
                    tok[count] = 7;
                    count++;
                    c = fgetc(file);
                }

            } else if (c == '=') { // case 9
                tok[count] = 9;
                count++;
                c = fgetc(file);

            } else if (c == '<') { // accounts for case 10-12
                c = fgetc(file);

                if (c == '>') { // case 10
                    tok[count] = 10;
                    count++;
                    c = fgetc(file);

                } else if (c == '=') { // case 12
                    // printf("\t<=\t12\n");
                    tok[count] = 12;
                    count++;
                    c = fgetc(file);
                } else { // case 11
                    // printf("\t<\t11\n");
                    tok[count] = 11;
                    count++;
                }

            } else if (c == '>') { // accounts for case 13 & 14
                c = fgetc(file);

                if (c == '=') { // case 14
                    // printf("\t>=\t14\n");
                    tok[count] = 14;
                    count++;
                    c = fgetc(file);
                } else { // case 13
                    // printf("\t>\t13\n");
                    tok[count] = 13;
                    count++;
                }

            } else if (c == '(') { // case 15
                // printf("\t%c\t15\n", c);
                tok[count] = 15;
                count++;
                c = fgetc(file);

            } else if (c == ')') { // case 16
                // printf("\t%c\t16\n", c);
                tok[count] = 16;
                count++;
                c = fgetc(file);

            } else if (c == ',') { // case 17
                // printf("\t%c\t17\n", c);
                tok[count] = 17;
                count++;
                c = fgetc(file);

            } else if (c == ';') { // case 18
                // printf("\t%c\t18\n", c);
                tok[count] = 18;
                count++;
                c = fgetc(file);

            } else if (c == '.') { // case 19
                // printf("\t%c\t19\n", c);
                tok[count] = 19;
                count++;
                c = fgetc(file);

            } else if (c == ':') { // checks for case 20 if not then error
                c = fgetc(file);
                if (c == '=') { // case 20
                    // printf("\t:%c\t20\n", c);
                    tok[count] = 20;
                    count++;
                    c = fgetc(file);
                } else { // error(when ':' is alone
                    // printf("Error : Invalid Symbol\n");
                    exit(1);
                }

            } else { // prints an error any time a character not accepted is reached
                // printf("Error : Invalid Symbol\n");
                exit(1);
                c = fgetc(file);
            }
        }
    }

    emit(7, lvl, 0);
    sizeHW3++;
    PROGRAM();

    printf("No errors, program is syntactically correct\n");
    printf("OP\tL\tM\n");

    FILE *fptr;
    fptr = fopen("elf.txt", "w");

    for (int i = 0; i < sizeHW3; i++) {
        printf("%d\t%d\t%d\n", inst[i].op, inst[i].l, inst[i].m);
        fprintf(fptr, "%d\t%d\t%d\n", inst[i].op, inst[i].l, inst[i].m);
    }

    fclose(fptr);
    fclose(file);

    return 0;
}

int checkString(char *str) {

    // if else statements to find if valid string is a token or identifier and prints it
    if (strcmp(str, "odd") == 0) {
        return 1;
    } else if (strcmp(str, "ifel") == 0) {
        return 2;
    } else if (strcmp(str, "begin") == 0) {
        return 21;
    } else if (strcmp(str, "end") == 0) {
        return 22;
    } else if (strcmp(str, "if") == 0) {
        return 23;
    } else if (strcmp(str, "then") == 0) {
        return 24;
    } else if (strcmp(str, "while") == 0) {
        return 25;
    } else if (strcmp(str, "do") == 0) {
        return 26;
    } else if (strcmp(str, "call") == 0) {
        return 27;
    } else if (strcmp(str, "const") == 0) {
        return 28;
    } else if (strcmp(str, "var") == 0) {
        return 29;
    } else if (strcmp(str, "procedure") == 0) {
        return 30;
    } else if (strcmp(str, "write") == 0) {
        return 31;
    } else if (strcmp(str, "read") == 0) {
        return 32;
    } else if (strcmp(str, "else") == 0) {
        return 2;
    } else { // case for when string is an identifier
        return 2;
    }
}

int getInt(int *dig, int size) {

    int k = 0;

    for (int i = 0; i < size; i++) { // loops through array of ints to make them one int
        k = 10 * k + dig[i];
    }

    return k;
}

int validity(char c) {
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' ||
        c == '>' || c == '(' || c == ')' || c == ',' || c == ';' || c == '.' ||
        c == ':') { // makes sure character is not any valid token
        return 1;
    } else {
        return 0;
    }
}

void emit(int op, int l, int m) {

    if (sizeHW3 > 100) {
        printf("Error: code can only go up to 100 lines");
        exit(1);
    }

    inst[sizeHW3].op = op;
    inst[sizeHW3].m = m;
    inst[sizeHW3].l = l;
    inst[sizeHW3].r = 0;
}

int symbolTableCheck(char *str) {

    int symIdx = -1;

    for (int i = 0; i < symCount; i++) { // loops through the amount of stored symbols
        if (strcmp(s[i].name, str) == 0) {               // checks to see if identifier was declared
            symIdx = i; // sets symIdx to index that the desired string is in
        }
    }

    return symIdx;
}

void PROGRAM() {

    BLOCK();

    if (tok[cnt] != 19) {
        printf("Error: program must end with a period");
        exit(1);
    }

    emit(9, 0, 3);
    sizeHW3++;
}

void BLOCK() {

    int jmpIdx = 0;
    int numVars = 0;
    emit(7, curLvl, 0);

    do {
        CONST_DECLARATION();
        numVars += VAR_DECLARATION();
        while (tok[cnt] == 30) {

            cnt++;

            if (tok[cnt] == 2) {

                strcpy(s[symCount].name, id[caseHW3]);
                caseHW3++;

                s[symCount].kind = 3;
                s[symCount].addr = sizeHW3 * 3;
                s[symCount].mark = 1;
                s[symCount].level = curLvl;
                symCount++;
                cnt++;
            } else {
                printf("Error: procedure must be followed by identifier");
                exit(1);
            }

            if (tok[cnt] == 18) {
                cnt++;
            } else {
                printf("Error: procedure declaration must be followed by semicolon");
                exit(1);
            }

            curLvl++;
            BLOCK();
            curLvl--;

            if (tok[cnt] == 18) {
                cnt++;
            } else {
                printf("Error: procedures must be followed by semicolon");
                exit(1);
            }
        }
    } while (tok[cnt] == 28 || tok[cnt] == 29 || tok[cnt] == 30);

    inst[0].m = sizeHW3 * 3;
    emit(6, 0, 3 + numVars);
    sizeHW3++;
    STATEMENT();
    
    if (curLvl != 0) {
        emit(2, 0, 0);
        sizeHW3++;
    }
}

void CONST_DECLARATION() {

    if (tok[cnt] == 28) {
        do {
            cnt++;

            if (tok[cnt] != 2) {
                printf("Error: 'const' must be followed by identifier");
                exit(1);
            }

            if (symbolTableCheck(id[caseHW3]) != -1) {
                printf("Error: redeclaration of const");
                exit(1);
            }

            cnt++;
            if (tok[cnt] != 9) {
                printf("Error: const identifier must be followed by =");
                exit(1);
            }

            cnt++;
            if (tok[cnt] != 3) {
                printf("Error: const must be assigned an integer value");
                exit(1);
            }

            s[symCount].kind = 1;
            strcpy(s[symCount].name, id[caseHW3]);
            caseHW3++;

            s[symCount].val = hash[numHW3];
            numHW3++;

            s[symCount].addr = 0;
            s[symCount].level = curLvl;
            s[symCount].mark = 1;
            symCount++;
            cnt++;
        } while (tok[cnt] == 17);

        if (tok[cnt] != 18) {
            printf("Error: const declarations must be followed by ';'");
            exit(1);
        }
        cnt++;
    }
}

int VAR_DECLARATION() {

    int numVars = 0;

    if (tok[cnt] == 29) {
        do {
            numVars++;
            cnt++;

            if (tok[cnt] != 2) {
                printf("Error: 'var' declaration must be followed by an identifier");
                exit(1);
            }

            if (symbolTableCheck(id[caseHW3]) != -1) {
                printf("Error: redeclaration of identifier");
                exit(1);
            }

            s[symCount].kind = 2;
            strcpy(s[symCount].name, id[caseHW3]);
            caseHW3++;

            s[symCount].val = 0;
            s[symCount].addr = numVars + 2;
            s[symCount].level = curLvl;
            s[symCount].mark = 1;
            symCount++;
            cnt++;
        } while (tok[cnt] == 17);

        if (tok[cnt] != 18) {
            printf("Error: var declarations must be ende with a ';'");
            exit(1);
        }

        cnt++;
        return numVars;
    }
}

void STATEMENT() {

    int symIdx = 0;
    int jpcIdx = 0;
    int loopIdx = 0;

    if (tok[cnt] == 2) { // case for when tok[cnt] is an identifier
        symIdx = symbolTableCheck(id[caseHW3]);
        caseHW3++;

        if (symIdx == -1) {
            printf("Error: identifier not declared");
            exit(1);
        }

        if (s[symIdx].kind != 2) {
            printf("Error: only variables can be modified");
            exit(1);
        }

        cnt++;

        if (tok[cnt] != 20) { // when tok[cnt] is not ':='
            printf("Error: vars can only be modified with becomesym");
            exit(1);
        }

        cnt++;
        EXPRESSION();
        emit(4, curLvl, s[symIdx].addr);
        sizeHW3++;

        return;
    }

    if (tok[cnt] == 27) { // case for when tok[cnt] is callsym
        cnt++;

        if (tok[cnt] != 2) {
            printf("Error: callsym must be followed by indentsym");
            exit(1);
        } else {
            symIdx = symbolTableCheck(id[caseHW3]);
            caseHW3++;

            if (symIdx == -1) {
                printf("Error: identifier name not found");
                exit(1);
            }

            if (s[symIdx].kind == 3) {
                emit(5, curLvl - s[symIdx].level, s[symIdx].addr);
                sizeHW3++;
                curLvl++;
            } else {
                printf("Error: call can only use procedure identifiers");
                exit(1);
            }

            cnt++;
        }
    }

    if (tok[cnt] == 21) { // case for when tok[cnt] is beginsym
        do {
            cnt++;
            STATEMENT();
        } while (tok[cnt] == 18);

        if (tok[cnt] != 22) {
            printf("Error: begin must be followed by end");
            exit(1);
        }

        if (curLvl != 0) {
            curLvl--;
            sizeHW3++;
        }

        cnt++;
        return;
    }

    if (tok[cnt] == 23) { // case for when tok[cnt] is ifsym
        cnt++;
        CONDITION();
        jpcIdx = sizeHW3;
        emit(8, 0, (jpcIdx + 1) * 3);
        sizeHW3++;

        if (tok[cnt] != 24) {
            printf("Error: if must be followed by then");
            exit(1);
        }

        cnt++;
        STATEMENT();
        inst[jpcIdx].m = (sizeHW3 + 1) * 3;

        return;
    }

    if (tok[cnt] == 25) { // case for when tok[cnt] is whilesym
        cnt++;
        loopIdx = sizeHW3;
        CONDITION();

        if (tok[cnt] != 26) {
            printf("Error: while must be followed by do");
            exit(1);
        }

        cnt++;
        jpcIdx = sizeHW3;
        emit(8, 0, (jpcIdx + 1) * 3);

        sizeHW3++;
        STATEMENT();

        emit(7, 0, loopIdx);

        sizeHW3++;
        inst[jpcIdx].m = (sizeHW3 + 1) * 3;

        return;
    }

    if (tok[cnt] == 32) { // case for when tok[cnt] is readsym
        cnt++;

        if (tok[cnt] != 2) {
            printf("Error: read must be followed by an identifier");
            exit(1);
        }

        symIdx = symbolTableCheck(id[caseHW3]);
        caseHW3++;
        
        if (symIdx == -1) {
            printf("Error: identifier not found");
            exit(1);
        }

        if (s[symIdx].kind != 2) {
            printf("Error: only identifiers can be read");
            exit(1);
        }

        cnt++;
        emit(9, 0, 1);
        sizeHW3++;
        emit(4, curLvl - s[symIdx].level, s[symIdx].addr);
        sizeHW3++;
        
        return;
    }

    if (tok[cnt] == 31) { // case for when tok[cnt] is writesym
        cnt++;
        EXPRESSION();
        emit(9, 0, 2);
        sizeHW3++;

        return;
    }
}

void CONDITION() {

    if (tok[cnt] == 1) { // case for when tok[cnt] is oddsym
        cnt++;
        EXPRESSION();
        emit(2, 0, 11);
        sizeHW3++;
    } else { // otherwise check other condition statements
        EXPRESSION();

        if (tok[cnt] == 9) { // case for when tok[cnt] is eqlsym
            cnt++;
            EXPRESSION();
            emit(2, 0, 5);
            sizeHW3++;

        } else if (tok[cnt] == 10) { // case for when tok[cnt] is nqlsym
            cnt++;
            EXPRESSION();
            emit(2, 0, 6);
            sizeHW3++;

        } else if (tok[cnt] == 11) { // case for when tok[cnt] is lessym
            cnt++;
            EXPRESSION();
            emit(2, 0, 7);
            sizeHW3++;

        } else if (tok[cnt] == 12) { // case for when tok[cnt] is leqsym
            cnt++;
            EXPRESSION();
            emit(2, 0, 8);
            sizeHW3;

        } else if (tok[cnt] == 13) { // case for when tok[cnt] is gtrsym
            cnt++;
            EXPRESSION();
            emit(2, 0, 9);
            sizeHW3++;

        } else if (tok[cnt] == 14) { // case for when tok[cnt] is geqsym
            cnt++;
            EXPRESSION();
            emit(2, 0, 10);
            sizeHW3++;

        } else {
            printf("Error: condition must have condition symbol");
            exit(1);
        }
    }
}

void EXPRESSION() {

    if (tok[cnt] == 5) {
        cnt++;
        TERM();
        emit(2, 0, 12); // case for emitting NEG
        sizeHW3++;

        while (tok[cnt] == 4 || tok[cnt] == 5) {
            if (tok[cnt] == 4) { // when dealing with plussym
                cnt++;
                TERM();
                emit(2, 0, 1);
                sizeHW3++;
            } else { // when dealing with minussym
                cnt++;
                TERM();
                emit(2, 0, 2);
                sizeHW3++;
            }
        }
    } else { // otherwise dealing with positive numbers
        if (tok[cnt] == 4)
            cnt++;
        TERM();

        while (tok[cnt] == 4 || tok[cnt] == 5) { // loop to figure out whether add or subtract
            if (tok[cnt] == 4) { // case for plussym
                cnt++;
                TERM();
                emit(2, 0, 1);
                sizeHW3++;
            } else { // case for minussym
                cnt++;
                TERM();
                emit(2, 0, 2);
                sizeHW3++;
            }
        }
    }
}

void TERM() {

    FACTOR();

    while (tok[cnt] == 6 || tok[cnt] == 7) {
        if (tok[cnt] == 6) {
            cnt++;
            FACTOR();
            emit(2, 0, 3);
            sizeHW3++;
        } else if (tok[cnt] == 7) {
            cnt++;
            FACTOR();
            emit(2, 0, 4);
            sizeHW3++;
        }
    }
}

void FACTOR() {

    int symIdx = 0;

    if (tok[cnt] == 2) {
        symIdx = symbolTableCheck(id[caseHW3]);
        caseHW3++;

        if (symIdx == -1) {
            printf("Error: undeclared variable trying to be used");
            exit(1);
        }

        if (s[symIdx].kind == 1) {
            emit(1, 0, s[symIdx].val);
            sizeHW3++;
        } else {
            // printf("%d ", s[symIdx].level);
            emit(3, s[symIdx].level, s[symIdx].addr);
            sizeHW3++;
        }

        cnt++;

    } else if (tok[cnt] == 3) {
        emit(1, 0, hash[numHW3]);
        sizeHW3++;
        numHW3++;
        cnt++;

    } else if (tok[cnt] == 15) { // case for when we run into a left parenthesis
        cnt++;
        EXPRESSION();

        if (tok[cnt] != 16) { // case for when no rparenthesissym is found
            printf("Error: left parenthesis must be followed by right parenthesis");
            exit(1);
        }
        cnt++;
    } else {
        printf("Error: arithmetic equations must contain operands, parentheses, numbers, or symbols");
        exit(1);
    }
}