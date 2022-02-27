// Author: Charles Wyatt z5194905
// Date created: 9/08/2020

// Mips Simulator
    // This code takes a single file as input. 
    // This file should have hex numbers corresponding
    // to mips commands. This file then decodes the input into instructions, 
    // executes these instructions and returns the registers that have changed.

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_INSTRUCTION_CODES 1000

// don't have immediate values
#define ADD_CODE 32
#define SUB_CODE 34
#define AND_CODE 36
#define OR_CODE 37
#define SLT_CODE 42
#define MUL_FRONT_CODE 28
#define MUL_BACK_CODE 2

// do have immediate values
#define BEQ_CODE 4
#define BNE_CODE 5
#define ADDI_CODE 8
#define SLTI_CODE 10
#define ANDI_CODE 12
#define ORI_CODE 13
#define LUI_CODE 15

#define SYSCALL_CODE 12

// checks validity of all instructions - quits program if any invalid instruction
void valid_instruction(uint32_t code, int counter, char *filename);

// decodes hexademical numbers into instructions and prints them to stdout
void decode_instruction(uint32_t code);

// executes all instructions
int execute_instruction(uint32_t code, int registers[], int *counter, int num_codes);


int main(int argc, char *argv[]) {

    if(argc <= 1) {
        printf("Failed: Not enough arguments\n");
        return 1;
    }

    FILE *f = fopen(argv[1], "r");

    if(f == NULL) {
        perror("Failed to open file: ");
        printf("%s\n", argv[1]);
        return 1;
    }

    int registers[32] = {0};

    uint32_t codes[MAX_INSTRUCTION_CODES];
    int num_codes = 0;
    while(num_codes < MAX_INSTRUCTION_CODES && fscanf(f, "%x", &codes[num_codes]) == 1) {
        num_codes++;
    }

    // checks the validity of all codes
    // if there is an invalid code, the program quits.
    for(int j = 0; j < num_codes; j++) {
        valid_instruction(codes[j], j, argv[1]);
    }

    // prints out codes as instructions to stdout
    printf("Program\n");
    for(int j = 0; j < num_codes; j++) {
        printf("%3d: ", j);
        decode_instruction(codes[j]);
    }

    // prints the output
    printf("Output\n");
    for(int j = 0; j < num_codes; j++) {

        if(execute_instruction(codes[j], registers, &j, num_codes) == 1) {
            if(registers[0] != 0) { // if register zero is changed, set back to 0
                registers[0] = 0;
            }
            break; // breaks if an execution halt
        }

        if(registers[0] != 0) { // if register zero is changed, set back to 0
            registers[0] = 0;
        }
    }

    // prints the registers the changed after execution
    printf("Registers After Execution\n");
    for(int j = 0; j < 32; j++) {
        if(registers[j] != 0) {
            printf("$%-2d = %d\n", j, registers[j]);
        }
    }


    return 0;
}

// checks validity of all instructions - quits program if any invalid instruction
void valid_instruction(uint32_t code, int counter, char *filename) {
    // masks for the first 6 bits and the last 6 bits
    uint32_t front_code = code >> 26;
    uint32_t back_code = (code & 63);

    counter = counter + 1; // since trying to find the line of error of file, not index

    if (code == SYSCALL_CODE) {
    } else if ( front_code == 0 || 
            ( (front_code == MUL_FRONT_CODE) & (back_code == MUL_BACK_CODE)) 
            ) 
    { 
        if (front_code != 0) {
            // mul
        } else if (back_code == ADD_CODE) {
            // add
        } else if (back_code == SUB_CODE) {
            // sub
        } else if (back_code == AND_CODE) {
            // and
        } else if (back_code == OR_CODE) {
            // or
        } else if (back_code == SLT_CODE) {
            // slt
        } else {
            printf("%s:%d: invalid instruction code: %08X\n", filename, counter, code);
            exit(0);
        }

    } else { // these codes have immediate values
        
        // finding registers
        uint32_t code_shift = code;
        uint32_t reg_mask = 31;
        code_shift >>= 21;
        int s_reg = (code_shift & reg_mask);

        if ( front_code == BEQ_CODE) {
            // beq
        } else if ( front_code == BNE_CODE) {
            // bne
        } else if ( front_code == ADDI_CODE) {
            // addi
        } else if ( front_code == SLTI_CODE) {
            // stli
        } else if ( (front_code == LUI_CODE) && (s_reg == 0) ) {
            // lui
        } else if ( front_code == ANDI_CODE) {
            // andi
        } else if ( front_code == ORI_CODE) {
            // ori
        } else {
            printf("%s:%d: invalid instruction code: %08X\n", filename, counter, code);
            exit(0);
        }
    }

    return;
}

// decodes hexademical numbers into instructions and prints them to stdout
void decode_instruction(uint32_t code) {
    // masks for the first 6 bits and the last 6 bits
    uint32_t front_code = code >> 26;
    uint32_t back_code = (code & 63);

    if (code == SYSCALL_CODE) {
        printf("%-5s", "syscall\n");
    } else if (front_code == 0 || // no front code
            ( (front_code == MUL_FRONT_CODE) & (back_code == MUL_BACK_CODE)) // mul operation
            ) 
    { // no immediate values

        // allocating registers (no immediate values)
        uint32_t code_shift = code;
        uint32_t reg_mask = 31;

        code_shift >>= 11;
        int d_reg = (code_shift & reg_mask);

        code_shift >>= 5;
        int t_reg = (code_shift & reg_mask);

        code_shift >>= 5;
        int s_reg = (code_shift & reg_mask);

        if (front_code != 0) {
            // mul
            printf("%-5s", "mul");
        } else if (back_code == ADD_CODE) {
            // add
            printf("%-5s", "add");
        } else if (back_code == SUB_CODE) {
            // sub
            printf("%-5s", "sub");
        } else if (back_code == AND_CODE) {
            // and
            printf("%-5s", "and");
        } else if (back_code == OR_CODE) {
            // or
            printf("%-5s", "or");
        } else if (back_code == SLT_CODE) {
            // slt
            printf("%-5s", "slt");
        } else {
            printf("invalid instruction code: %08X\n", code);
            exit(0);
        }
        printf("$%d, $%d, $%d\n", d_reg, s_reg, t_reg);


    } else { // these codes have immediate values
        
        // finding registers
        uint32_t code_shift = code;
        uint32_t reg_mask = 31;
        code_shift >>= 16;
        int t_reg = (code_shift & reg_mask);

        code_shift >>= 5;
        int s_reg = (code_shift & reg_mask);

        int32_t imm_mask = 0xFFFF;
        int16_t imm = (code & imm_mask);

        
        if (front_code == BEQ_CODE) {
            // beq
            printf("%-5s", "beq");
            printf("$%d, $%d, %d\n", s_reg, t_reg, imm);

        } else if (front_code == BNE_CODE) {
            // bne
            printf("%-5s", "bne");
            printf("$%d, $%d, %d\n", s_reg, t_reg, imm);

        } else if (front_code == ADDI_CODE) {
            // addi
            printf("%-5s", "addi");

            printf("$%d, $%d, %d\n", t_reg, s_reg, imm);
        } else if (front_code == SLTI_CODE) {
            // stli
            printf("%-5s", "stli");

            printf("$%d, $%d, %d\n", t_reg, s_reg, imm);
        } else if ( (front_code == LUI_CODE) && (s_reg == 0) ) {
            // lui
            printf("%-5s", "lui");

            printf("$%d, %d\n", t_reg, imm);
        } else if (front_code == ANDI_CODE) {
            // andi
            printf("%-5s", "andi");

            printf("$%d, $%d, %d\n", t_reg, s_reg, imm);
        } else if (front_code == ORI_CODE) {
            // ori
            printf("%-5s", "ori");

            printf("$%d, $%d, %d\n", t_reg, s_reg, imm);
        } else {
            printf("invalid instruction code: %08X\n", code);
            exit(0);
        }
    }

    return;
}


// executes all instructions
int execute_instruction(uint32_t code, int registers[], int *counter, int num_codes) {
    // masks for the first and last 6 bits of the code
    uint32_t front_code = code >> 26;
    uint32_t back_code = (code & 63);
    
    if (code == SYSCALL_CODE) {
        if (registers[2] == 1) { // $v0 = 1
            printf("%d", registers[4]); // prints $a0
        } else if (registers[2] == 10) { // $v0 = 10
            return 1;
        } else if (registers[2] == 11){ // $v0 = 11
            // syscall 11 should print the lowest 8 bits
            printf("%c", registers[4]); // prints $a0
        } else {
            printf("Unknown system call: %d\n", registers[2]); // unknown syscall 
            return 1;
        }
    } else if (front_code == 0 || 
            ( (front_code == MUL_FRONT_CODE) & (back_code == MUL_BACK_CODE)) 
            ) 
    { // these codes do not have immediate values
            
        // gets the registers
        uint32_t code_shift = code;
        uint32_t reg_mask = 31;

        code_shift >>= 11;
        int d = (code_shift & reg_mask);

        code_shift >>= 5;
        int t = (code_shift & reg_mask);

        code_shift >>= 5;
        int s = (code_shift & reg_mask);    


        if (front_code != 0) {
            // mul
            registers[d] = registers[s] * registers[t];
        } else if (back_code == ADD_CODE) {
            // add
            registers[d] = registers[s] + registers[t];
        } else if (back_code == SUB_CODE) {
            // sub
            registers[d] = registers[s] - registers[t];
        } else if (back_code == AND_CODE) {
            // and
            registers[d] = registers[s] & registers[t];
        } else if (back_code == OR_CODE) {
            // or
            registers[d] = registers[s] | registers[t];
        } else if (back_code == SLT_CODE) {
            // slt
            if (registers[s] < registers[t]) {
                registers[d] = 1;
            } else {
                registers[d] = 0;
            }
        } else {
            printf("invalid instruction code: %08X\n", code);
            return 1;
        }

    } else { // these codes do have immediate values

        // gets the registers
        uint32_t code_shift = code;
        uint32_t reg_mask = 31;
        code_shift >>= 16;
        int t = (code_shift & reg_mask);

        code_shift >>= 5;
        int s = (code_shift & reg_mask);

        int32_t imm_mask = 0xFFFF;
        int16_t i = (code & imm_mask);

        
        if (front_code == BEQ_CODE) {
            // beq
            if (registers[s] == registers[t]) {
                *counter = *counter + i - 1; // - 1 since counter will go through loop again
                if (*counter < 0 || *counter >= num_codes) {
                    return 1;
                }
            }

        } else if (front_code == BNE_CODE) {
            // bne
            if (registers[s] != registers[t]) {
                *counter = *counter + i - 1; // - 1 since counter will go through loop again
                if (*counter < 0 || *counter >= num_codes) {
                    return 1;
                }
            }

        } else if (front_code == ADDI_CODE) {
            // addi
            registers[t] = registers[s] + i;
        } else if (front_code == SLTI_CODE) {
            // stli
            if (registers[s] < i) {
                registers[t] = 1;
            } else {
                registers[t] = 0;
            }
        } else if ( (front_code == LUI_CODE) && (s == 0) ) {
            // lui
            uint32_t result = i;
            result <<= 16;
            registers[t] = result;
        } else if (front_code == ANDI_CODE) {
            // andi
            registers[t] = (registers[s] & i);
        } else if (front_code == ORI_CODE) {
            // ori
            registers[t] = (registers[s] | i);
        } else {
            printf("invalid instruction code: %08X\n", code);
            return 1;
        }
    }
    return 0;
}
