#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int checkString(char *str);                 // function to check what type of token a valid string is 
int getInt(int *dig, int size);          // function that converts an array of ints to one int in order of array 
int validity(char c);                   // checks to see if character is a token or not

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
    char c;                     // char variable that will be used to iterate through the file
    char c2;                    // will be used for certain cases when needing to keep track of previous char 
    char str[12];          // char array used to store strings in file
    int dig[6];                 // int array used to hold numbers in file
    int tok[100000];            // arr of token values
    char id[100000][11];        // arr of identifier strings
    int hash[100000];           // int arr to hold numbers
    int k = 0;                  // used to set int values from char digits
    int size = 0;               // used to keep track of the size of the string
    int count = 0;              // counts how many tokens there are
    int flag = 0;               // flag used for digit error cases
    int num = 0;                // int that keeps track of the amount of identifier tokens there are 
    int track = 0;          // used to track of identifiers when printing token list
    int case3 = 0;              // used to hold values of valid numbers in file
    int caseSize = 0;           // used to keep track of the amount of valid numbers
    
    for (int i = 0; i < 11; i++) { // initializing array that will hold each digit in an int
        dig[i] = 0;
    }

    for (int i = 0; i < 11; i++) { // initializing char array
        str[i] = '\0';
    }

    c = fgetc(file);
    if (file == NULL) {
        printf("Error Occurred while Opening the File");
    } else {
        while (c != EOF) { // prints input text
            printf("%c", c);
            c = fgetc(file);
        }
    }

    rewind(file); // resets what element to look at
    printf("\n\nLexeme Table:\n\nlexeme\t\ttokentype\n");
    c = fgetc(file);
    while (c != EOF) {
        if (isalpha(c)) {
            if (isspace(c) == 0) { // if statement that check to make sure c is not a space 
                while (isspace(c) == 0 && c != EOF && validity(c) == 0 && !isdigit(c)) { // loop that runs until certain char are met
                    if (size > 11) { // if statement if length of string is > 11 
                        while (isspace(c) == 0 && c != EOF && validity(c) == 0 && !isdigit(c)) {                    // same loop
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
                } for (int i = 0; i < 11; i++) { // resets the values of str
                    str[i] = '\0';
                }
                
                size = 0; // resets the size of the string
            } else { // backup else statement
                printf("\t%c\t2\n", c2);
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

                    while(c != EOF && c != ',' && c != ';' && isspace(c) == 0 && validity(c) != 1){
                        c = fgetc(file);
                    }

                    flag = 1;
                    break;
                }
                if (size > 4) { // checks when number exceeds 5 digits
                    printf("Error : Numbers cannot exceed 5 digits\ n");

                    while (c != EOF && validity(c) == 0 && isspace(c) == 0 && isdigit(c)) {
                        c = fgetc(file); // ignores the rest of the string
                    }

                    flag = 1;
                    break;
                }

                c = fgetc(file);
            }

            if (size <= 5 && flag == 0) {
                case3 = getInt(dig, size);              // sets number = case3
                printf("\t%d\t3\n", getInt(dig, size)); // prints case
                hash[caseSize] = case3; // sets int array at caseSize to integer num
                caseSize++; // increments the amount of ints in the array
                tok[count] = 3; // token placement
                count++;
            }

            size = 0;
            flag = 0;

            for(int i = 0; i<5; i++){ // resets the digits
                dig[i] = 0;
            }

        } else{
            if(isspace(c) != 0){ // ignores any space, new line, or tab char
                c = fgetc(file);
            } else if(c == '+'){ // case 4
                printf("\t+\t4\n");
                tok[count] = 4;
                count++;
                c = fgetc(file);
            } else if(c == '-'){ // case 5
                printf("\t%c\t5\n", c);
                tok[count] = 5;
                count++;
                c = fgetc(file);
            } else if(c == '*'){ // case 6
                printf("\t%c\t6\n", c);
                tok[count] = 6;
                count++;
                c = fgetc(file);
            } else if(c == '/'){ // handles comments and case 7
                c2 = c;
                c = fgetc(file);

                if(c == '/') { //check if it is a comment
                    while(c != '\n') { // ignores every character until a new line is reached
                        c = fgetc(file);
                    }

                    c = fgetc(file);
                } else if(c == '*') { // case for comments that start with /*
                    c = fgetc(file);

                    while(c2 != '*' && c != '/') { // ignores every character until */ is reached
                        c2 = c;
                        c = fgetc(file);
                    }

                    c = fgetc(file);
                } else { // case 7
                    printf("\t/\t7\n");
                    tok[count] = 7;
                    count++;
                    c = fgetc(file);
                }

            } else if(c == '=') { // case 9
                printf("\t%c\t9\n", c);
                tok[count] = 9;
                count++;
                c = fgetc(file);

            } else if(c == '<') { // accounts for case 10-12
                c = fgetc(file);
                if(c == '>') { // case 10
                    printf("\t<>\t10\n");
                    tok[count] = 10;
                    count++;
                    c = fgetc(file);
                } else if(c == '=') { // case 12
                    printf("\t<=\t12\n");
                    tok[count] = 12;
                    count++;
                    c = fgetc(file);
                } else { // case 11
                    printf("\t<\t11\n");
                    tok[count] = 11;
                    count++;
                }
            } else if(c == '>') { // accounts for case 13 & 14
                c = fgetc(file);
                if(c == '=') { // case 14
                    printf("\t>=\t14\n");
                    tok[count] = 14;
                    count++;
                    c = fgetc(file);
                } else { // case 13
                    printf("\t>\t13\n");
                    tok[count] = 13;
                    count++;
                }

            } else if(c == '(') { // case 15
                printf("\t%c\t15\n", c);
                tok[count] = 15;
                count++;
                c = fgetc(file);

            } else if(c == ')') { // case 16
                printf("\t%c\t16\n", c);
                tok[count] = 16;
                count++;
                c = fgetc(file);

            } else if(c == ',') { // case 17
                printf("\t%c\t17\n", c);
                tok[count] = 17;
                count++;
                c = fgetc(file);

            } else if(c == ';') { // case 18
                printf("\t%c\t18\n", c);
                tok[count] = 18;
                count++;
                c = fgetc(file);

            } else if(c == '.') { // case 19
                printf("\t%c\t19\n", c);
                tok[count] = 19;
                count++;
                c = fgetc(file);

            } else if(c == ':') { // checks for case 20 if not then error
                c = fgetc(file);
                if(c == '=') { // case 20
                    printf("\t:%c\t20\n", c);
                    tok[count] = 20;
                    count++;
                    c = fgetc(file);
                } else { // error(when ':' is alone
                    printf("Error : Invalid Symbol\n");
                }

            } else { // prints an error any time a character not accepted is reached
                printf("Error : Invalid Symbol\n");
                c = fgetc(file);
            }
        }
    }

    caseSize = 0; // resets the caseSize in order to track and print
    printf("\nToken List:\n");

    for(int i = 0; i<count; i++) { // loop that will go through arrays and print
        printf("%d ", tok[i]); // prints the value of the tokens stored previously
        if(tok[i] == 2) { // will print the string of any identifier
            for(int j = 0; j<strlen(id[track]); j++) {
                printf("%c", id[track][j]);
            }
            printf(" ");
            track++;
        } else if(tok[i] == 3) { // will print any valid number stored
            printf("%d ", hash[caseSize]);
            caseSize++;
        }
    }

    fclose(file);
    return 0;
}

int checkString(char * str) {
    // if else statements to find if valid string is a token or identifier and prints it
    if(strcmp(str, "skip") == 0) {
        printf("\tskip\t1\n");
        return 1;
    } else if(strcmp(str, "ifel") == 0) {
        printf("\tifel\t8\n");
        return 8;
    } else if(strcmp(str, "begin") == 0) {
        printf("\tbegin\t21\n");
        return 21;
    } else if(strcmp(str, "end") == 0) {
        printf("\tend\t22\n");
        return 22;
    } else if(strcmp(str, "if") == 0) {
        printf("\tif\t23\n");
        return 23;
    } else if(strcmp(str, "then") == 0) {
        printf("\tthen\t24\n");
        return 24;
    } else if(strcmp(str, "while") == 0) {
        printf("\twhile\t25\n");
        return 25;
    } else if(strcmp(str, "do") == 0) {
        printf("\tdo\t26\n");
        return 26;
    } else if(strcmp(str, "call") == 0) {
        printf("\tcall\t27\n");
        return 27;
    } else if(strcmp(str, "const") == 0) {
        printf("\tconst\t28\n");
        return 28;
    } else if(strcmp(str, "var") == 0) {
        printf("\tvar\t29\n");
        return 29;
    } else if(strcmp(str, "procedure") == 0) {
        printf(" procedure 30\n");
        return 30;
    } else if(strcmp(str, "write") == 0) {
        printf("\twrite\t31\n");
        return 31;
    } else if(strcmp(str, "read") == 0) {
        printf("\tread\t32\n");
        return 32;
    } else if(strcmp(str, "else") == 0) {
        printf("\telse\t33\n");
        return 33;
    } else { // case for when string is an identifier
        printf("\t"); // formatting
        for(int i = 0; i<strlen(str); i++) { // loop that prints out the string
            printf("%c", str[i]);
        }
        printf("\t2\n"); // formatting

        return 2;
    }
}

int getInt(int * dig, int size) {
    int k = 0;
    for(int i = 0; i < size; i++) { // loops through array of ints to make them one 
        int k = 10 * k + dig[i];
    }
    return k;
}

int validity(char c) {
    if(c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '(' || c == ')' || c == ',' || c == ';' || c == '.' || c == ':'){ // makes sure character is not any valid token
        return 1;
    } else{
        return 0;
    }
}