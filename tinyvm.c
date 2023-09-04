//CDA Project
// Author: Diego La Rosa
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv) {
    typedef struct {
        int op;
        int addr;
    } InstructionRegister;//Instruction Register
    InstructionRegister IR; 
    int PC = 0; //Program Counter
    int MAR = 0; //Memory Address Register
    int DM[11] = { 0,0,0,0,0,0,0,0,0,0,1 }; //Data Memory.
                                        // Index 0 and 1 stores input values. 
                                         //Index 2 stores math computation made from inputs. 
                                         //Index 10 stores integer 1, to decrement counter input accordingly if multiplication is necessary
    int MDR = 0; //Memory Data Register
    int IM[128] = { 0 }; //Instruction Memory
    int A = 0; //Accumulator
    int END = 0; // Culminate the program. 

    // Reading the file from the terminal
    char* fname = argv[1];
    FILE* file = fopen(fname, "r");

    if (file == 0)
    {
        printf("Could not open file\n");
    }
    else {
        // Read integers from the file until the end is reached
        int num;
        int i = 0;
        while (fscanf(file, "%d", &num) != EOF) {
            IM[i] = num; // Store the integers to the Instruction Memory
            i++;
        }
        fclose(file);
        //Making sure that the elf file is not empty
        if (i == 0) {
            printf("elf file is empty");
            return -1;
        }
    }
    char vmstate = 'y';
    //Status messages before starting execution
    printf("Would you like to see the state of the VM when executing the program?\n Please type y for yes or n for no, otherwise it will be displayed by default\n");
    scanf("%s", &vmstate);
    if(vmstate != 'n'){
    printf("Initial values\nPC = %d | A = %i | DM = [", PC, A);
    for (int i = 0; i < 9; i++) {
        printf("%i, ", DM[i]);
    }
    printf("%i]\n", DM[9]);
    }

    // Starting the Fetch execute cycle
    while (END != 1) {
        // Fetch
        IR.op = IM[PC]; // IR.op contains the instruction to execute.
        IR.addr = IM[PC + 1];// IR.addr contains the address where the instruction will be executed.
        PC = PC + 2; // Increment program counter to the next instruction

        // Execute
        switch (IR.op) {
            case(1):// LOAD
                 if(vmstate != 'n') printf("LOAD <%i>\n", IR.addr);
                MAR = IR.addr; 
                MDR = DM[MAR];
                A = MDR; // Assign to the Accumulator the value of the Data Memory at the index address of Instruction Register
                break;
            case(2): // ADD
                if(vmstate != 'n') printf("ADD <%i>\n", IR.addr);
                MAR = IR.addr;
                MDR = DM[MAR];
                A = A + MDR; //Add to the Accumulator the value in Data memory pointed at the address of Instruction Register
                break;
            case(3): // STORE
                if(vmstate != 'n') printf("STORE <%i>\n", IR.addr);
                MAR = IR.addr;
                MDR = A;
                DM[MAR] = MDR; // Store in Data Memory in the index pointed in Instruction Register address the Accumulator
                break;
            case(4): // SUBSTRACT
                 if(vmstate != 'n') printf("SUB <%i>\n", IR.addr);
                MAR = IR.addr;
                MDR = DM[MAR];
                A = A - MDR; //Substract to the Accumulator the value in Data memory pointed at the address of Instruction Register
                break;
            case(5): // INPUT A VALUE
                printf("IN <%i>\nInput a value:", IR.addr);
                scanf("%d", &A);  // Read input from the keyboard, make sure that it is valid, and store it in the Accumulator
                break;  
            case(6): // OUTPUT THE VALUE
                 if(vmstate != 'n'){
                    printf("OUT <%i>\n", IR.addr);
                    printf("The value in the accumulator is: %d\n", A);
                    }  //Print the value of the accumulator to the screen
                break;
            case(7): // HALT
                 if(vmstate != 'n') printf("HALT\n");
                END = 1; // Culminate the program after this execution
                break;
            case(8): // JMP
                 if(vmstate != 'n') printf("JMP <%i>\n", IR.addr);
                PC = IR.addr; // Jump back to a previous address pointed by the Instruction Register
                break;
            case(9): // SKIPZ
                 if(vmstate != 'n') printf("SKIPZ\n");
                if (A == 0) PC = PC + 2; // Skip the next instruction if condition is true
                break;
            case(10): // SKIPG
                if(vmstate != 'n') printf("SKIPG\n");
                if (A >  0) PC = PC + 2; // Skip the next instruction if condition is true
                break;
            case(11):// SKIPL
                 if(vmstate != 'n') printf("SKIPL\n");
                if (A < 0) PC = PC + 2; // Skip the next instruction if condition is true
                break;
        }
    //Status messages after the execute
    if(vmstate != 'n'){
    printf("PC = %d | A = %i | DM = [", PC, A);
    for (int i = 0; i < 9; i++) {
        printf("%i, ", DM[i]);
    }
    printf("%i]\n ", DM[9]);
    }
    }

    // Final message
    printf("The result is: %d\n", A); 
    printf("End of program");
    return 0;
}


