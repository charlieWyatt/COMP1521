#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_INSTRUCTION_CODES 1000

#define ADD_CODE 32
#define SUB_CODE 34
#define AND_CODE 36
#define OR_CODE 37
#define SLT_CODE 42

#define MUL_FRONT_CODE 28
#define MUL_BACK_CODE 2

#define BEQ_CODE 4
#define BNE_CODE 5
#define ADDI_CODE 8
#define SLTI_CODE 10
#define ANDI_CODE 12
#define ORI_CODE 13
#define LUI_CODE 15

#define SYSCALL_CODE 12



void execute_instruction(uint32_t code, int *registers, int *counter);
void decode_instruction(uint32_t code);


int main(int argc, char *argv[]) {

    if(argc <= 1) {
        printf("Failed: Not enough arguments\n");
        return 1;
    }

    FILE *f = fopen(argv[1], "r");

    if(f == NULL) {
        perror("Failed to open file: ");
        return 1;
    }

    int registers[32] = {0};

    uint32_t codes[MAX_INSTRUCTION_CODES];
    int num_codes = 0;
    while(num_codes < MAX_INSTRUCTION_CODES && fscanf(f, "%x", &codes[num_codes]) == 1) {
        num_codes++;
    }

    printf("Program\n");
    for(int j = 0; j < num_codes; j++) {
        printf("%3d: ", j);
        decode_instruction(codes[j]);
    }

    printf("Output\n");
    for(int j = 0; j < num_codes; j++) {

        execute_instruction(codes[j], registers, &j);
        // execute codes

        if(registers[0] != 0) {
            registers[0] = 0;
        }
    }

    printf("Registers After Execution\n");
    for(int j = 0; j < num_codes; j++) {
        if(registers[j] != 0) {
            printf("$%d  = %d\n", j, registers[j]);
        }
    }


    // different instructions are in the codes[] array, then branch statements are just moving between these indices
    // so in the array are a number of hex codes
    // go through each hex code and change it to the mips instruction via decode_instruction
    // done enough for a pass

    // to do the rest, go through and then run each of the mips instructions in c

    // make an iterator called curr_instr

    // also have an array of registers (make sure that register 0 is ALWAYS 0)


    return 0;
}

// // instead of printing here, change the value of code to the output of this
// void decode_instruction(uint32_t code) {
//     uint32_t front_mask = 63;
//     front_mask <<= 26;

//     uint32_t back_mask = 1;

//     if( (front_mask & code) == 0) {

//         // checks if mult
//         front_mask = 1;
//         front_mask <<= 30;
//         if( (front_mask & code) != 0) {
//             // mul
//             printf("%-5s", "mul");
//         } else {
//             // either add, sub, and, or, slt, syscall
//             if( (code & back_mask) == 1) {
//                 // or
//                 printf("%-5s", "or");
//             } else {
//                 back_mask <<= 1;
//                 if( (code & back_mask) != 0) {
//                     // either sub / slt
//                     back_mask <<= 2;
//                     if( (code & back_mask) != 0) {
//                         // slt
//                         printf("%-5s", "slt");
//                     } else {
//                         // sub
//                         printf("%-5s", "sub");
//                     }
//                 } else{
//                     // either and / add / syscall
//                     back_mask <<= 1;
//                     if( (code & back_mask) != 0) {
//                         // and / syscall
//                         back_mask <<= 1;
//                         if((back_mask & code) != 0) {
//                             // syscall
//                             printf("%-5s", "syscall\n");
//                             return;
//                         } else {
//                             printf("%-5s", "and");
//                         }
//                     } else{
//                         // add
//                         printf("%-5s", "add");
//                     }
//                 }
//             }
//         }

//         uint32_t code_shift = code;
//         uint32_t reg_mask = 31;

//         code_shift >>= 11;
//         int d_reg = (code_shift & reg_mask);

//         code_shift >>= 5;
//         int t_reg = (code_shift & reg_mask);

//         code_shift >>= 5;
//         int s_reg = (code_shift & reg_mask);        
        
//         printf("$%d, $%d, $%d\n", d_reg, s_reg, t_reg);
//     } else {
//         front_mask = 1;
//         front_mask <<= 29;
//         if( (front_mask & code) == 0) {
//             // either bne / beq
//             front_mask >>= 3;
//             if((front_mask & code) == 0) {
//                 // beq
//                 printf("%-5s", "beq");
//             } else {
//                 // bne
//                 printf("%-5s", "bne");
//             }
//             // these commands need registers printed in different order

//             uint32_t code_shift = code;
//             uint32_t reg_mask = 31;
//             code_shift >>= 16;
//             int t_reg = (code_shift & reg_mask);

//             code_shift >>= 5;
//             int s_reg = (code_shift & reg_mask);

//             int32_t imm_mask = 0xFFFF;
//             int16_t imm = (code & imm_mask);

//             printf("$%d, $%d, %d\n", s_reg, t_reg, imm);

//             return;

//         } else {
//             front_mask >>= 1;
//             if((front_mask & code) == 0) {
//                 // either addi / slti
//                 front_mask >>= 1;
//                 if((front_mask & code) == 0) {
//                     // addi
//                     printf("%-5s", "addi");
//                 } else {
//                     // slti
//                     printf("%-5s", "slti");
//                 }
//             } else {
//                 front_mask >>= 1;
//                 if((front_mask & code) != 0) {
//                     // lui
//                     printf("%-5s", "lui");

//                     // this needs different printing

//                     uint32_t code_shift = code;
//                     uint32_t reg_mask = 31;
//                     code_shift >>= 16;
                    
//                     int t_reg = (code_shift & reg_mask);

//                     int32_t imm_mask = 0xFFFF;
//                     int16_t imm = (code & imm_mask);

//                     printf("$%d, %d\n", t_reg, imm);

//                     return;
//                 } else {
//                     // either andi / ori /syscall
//                     front_mask >>= 1;
//                     if((front_mask & code) == 0) {
//                         // andi
//                         printf("%-5s", "andi");
//                     } else {
//                         // ori
//                         printf("%-5s", "ori");
//                     }
//                 }
//             } 
//         }

//         // note below code does not work for registers beq, bne or lui

//         // printing registers
//         uint32_t code_shift = code;
//         uint32_t reg_mask = 31;
//         code_shift >>= 16;
//         int t_reg = (code_shift & reg_mask);

//         code_shift >>= 5;
//         int s_reg = (code_shift & reg_mask);

//         int32_t imm_mask = 0xFFFF;
//         int16_t imm = (code & imm_mask);

//         printf("$%d, $%d, %d\n", t_reg, s_reg, imm);

//     }
// }



void decode_instruction(uint32_t code) {
    uint32_t front_code = code >> 26;
    uint32_t back_code = (code & 63);

    if( code == SYSCALL_CODE) {
        printf("%-5s", "syscall\n");
    } else if( front_code == 0 || 
        ( (front_code == MUL_FRONT_CODE) & (back_code == MUL_BACK_CODE)) 
        ) {
        if(front_code != 0) {
            // mul
            printf("%-5s", "mul");
        } else if(back_code == ADD_CODE) {
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
            return;
        }

        uint32_t code_shift = code;
        uint32_t reg_mask = 31;

        code_shift >>= 11;
        int d_reg = (code_shift & reg_mask);

        code_shift >>= 5;
        int t_reg = (code_shift & reg_mask);

        code_shift >>= 5;
        int s_reg = (code_shift & reg_mask);        
        
        printf("$%d, $%d, $%d\n", d_reg, s_reg, t_reg);
    } else {

        uint32_t code_shift = code;
        uint32_t reg_mask = 31;
        code_shift >>= 16;
        int t_reg = (code_shift & reg_mask);

        code_shift >>= 5;
        int s_reg = (code_shift & reg_mask);

        int32_t imm_mask = 0xFFFF;
        int16_t imm = (code & imm_mask);

        
        if ( front_code == BEQ_CODE) {
            // beq
            printf("%-5s", "beq");
            printf("$%d, $%d, %d\n", s_reg, t_reg, imm);

        } else if ( front_code == BNE_CODE) {
            // bne
            printf("%-5s", "bne");
            printf("$%d, $%d, %d\n", s_reg, t_reg, imm);

        } else if ( front_code == ADDI_CODE) {
            // addi
            printf("%-5s", "addi");

            printf("$%d, $%d, %d\n", t_reg, s_reg, imm);
        } else if ( front_code == SLTI_CODE) {
            // stli
            printf("%-5s", "stli");

            printf("$%d, $%d, %d\n", t_reg, s_reg, imm);
        } else if ( (front_code == LUI_CODE) && (s_reg == 0) ) {
            // lui
            printf("%-5s", "lui");

            printf("$%d, %d\n", t_reg, imm);
        } else if( front_code == ANDI_CODE) {
            // andi
            printf("%-5s", "andi");

            printf("$%d, $%d, %d\n", t_reg, s_reg, imm);
        } else if( front_code == ORI_CODE) {
            // ori
            printf("%-5s", "ori");

            printf("$%d, $%d, %d\n", t_reg, s_reg, imm);
        } else {
            printf("invalid instruction code: %08X\n", code);
            return;
        }
    }
    return;
}




void execute_instruction(uint32_t code, int *registers, int *counter) {
    uint32_t front_code = code >> 26;
    uint32_t back_code = (code & 63);

    if( code == SYSCALL_CODE) {
        if(registers[2] == 1) { // $v0 = 1
            printf("%d", registers[4]);
        } else if(registers[2] == 10) { // $v0 = 10
            exit(0);
        } else if(registers[2] == 11){ // $v0 = 11
            printf("%c", registers[2]);
        } else {
            printf("Unknown system call: %d", registers[2]); // unknown syscall 
            exit(0);
        }
    } else if( front_code == 0 || 
        ( (front_code == MUL_FRONT_CODE) & (back_code == MUL_BACK_CODE)) 
        ) {
            
        // gets the registers
        uint32_t code_shift = code;
        uint32_t reg_mask = 31;

        code_shift >>= 11;
        int d = (code_shift & reg_mask);

        code_shift >>= 5;
        int t = (code_shift & reg_mask);

        code_shift >>= 5;
        int s = (code_shift & reg_mask);    


        if(front_code != 0) {
            // mul
            registers[d] = registers[s] * registers[t];
        } else if(back_code == ADD_CODE) {
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
            if(registers[s] < registers[t]) {
                registers[d] = 1;
            } else {
                registers[d] = 0;
            }
        } else {
            printf("invalid instruction code: %08X\n", code);
            return;
        }

    } else {

        uint32_t code_shift = code;
        uint32_t reg_mask = 31;
        code_shift >>= 16;
        int t = (code_shift & reg_mask);

        code_shift >>= 5;
        int s = (code_shift & reg_mask);

        int32_t imm_mask = 0xFFFF;
        int16_t i = (code & imm_mask);

        
        if ( front_code == BEQ_CODE) {
            // beq
            if(registers[s] == registers[t]) {
                counter = counter + i;
            }

        } else if ( front_code == BNE_CODE) {
            // bne
            if(registers[s] != registers[t]) {
                counter = counter + i;
            }

        } else if ( front_code == ADDI_CODE) {
            // addi
            registers[t] = registers[s] + i;
        } else if ( front_code == SLTI_CODE) {
            // stli
            if(registers[s] < i) {
                registers[t] = 1;
            } else {
                registers[t] = 0;
            }
        } else if ( (front_code == LUI_CODE) && (s == 0) ) {
            // lui
            uint32_t result = i;
            result <<= 16;
            registers[t] = result;
        } else if( front_code == ANDI_CODE) {
            // andi
            registers[t] = (registers[s] & i);
        } else if( front_code == ORI_CODE) {
            // ori
            registers[t] = (registers[s] | i);
        } else {
            printf("invalid instruction code: %08X\n", code);
            return;
        }
    }
    return;
}






// void execute_instruction(uint32_t code, int *registers, int *counter) {
//     uint32_t front_mask = 63;
//     front_mask <<= 26;

//     uint32_t back_mask = 1;

//     if( (front_mask & code) == 0) {

//         // gets the registers
//         uint32_t code_shift = code;
//         uint32_t reg_mask = 31;

//         code_shift >>= 11;
//         int d = (code_shift & reg_mask);

//         code_shift >>= 5;
//         int t = (code_shift & reg_mask);

//         code_shift >>= 5;
//         int s = (code_shift & reg_mask);    

//         // checks if mult
//         front_mask = 1;
//         front_mask <<= 30;
//         if( (front_mask & code) != 0) {
//             // mul
//             registers[d] = registers[s] * registers[t];
//         } else {
//             // either add, sub, and, or, slt, syscall
//             if( (code & back_mask) == 1) {
//                 // or
//                 registers[d] = registers[s] | registers[t];
//             } else {
//                 back_mask <<= 1;
//                 if( (code & back_mask) != 0) {
//                     // either sub / slt
//                     back_mask <<= 2;
//                     if( (code & back_mask) != 0) {
//                         // slt
//                         if(registers[s] < registers[t]) {
//                             registers[d] = 1;
//                         } else {
//                             registers[d] = 0;
//                         }
//                     } else {
//                         // sub
//                         registers[d] = registers[s] - registers[t];
//                     }
//                 } else{
//                     // either and / add / syscall
//                     back_mask <<= 1;
//                     if( (code & back_mask) != 0) {
//                         // and / syscall
//                         back_mask <<= 1;
//                         if((back_mask & code) != 0) {
//                             // syscall
//                             if(registers[2] == 1) { // $v0 = 1
//                                 printf("%d", registers[4]);
//                             } else if(registers[2] == 10) { // $v0 = 10
//                                 exit(0);
//                             } else if(registers[2] == 11){ // $v0 = 11
//                                 printf("%c", registers[2]);
//                             } else {
//                                 printf("Unknown system call: %d", registers[2]); // unknown syscall 
//                                 exit(0);
//                             }
//                         } else {
//                             // and
//                             registers[d] = registers[s] & registers[t];
//                         }
//                     } else{
//                         // add
//                         registers[d] = registers[s] + registers[t];
//                     }
//                 }
//             }
//         }

//     } else {

//         uint32_t code_shift = code;
//         uint32_t reg_mask = 31;
//         code_shift >>= 16;
//         int t = (code_shift & reg_mask);

//         code_shift >>= 5;
//         int s = (code_shift & reg_mask);

//         int32_t imm_mask = 0xFFFF;
//         int16_t i = (code & imm_mask);


//         front_mask = 1;
//         front_mask <<= 29;
//         if( (front_mask & code) == 0) {
//             // either bne / beq
//             front_mask >>= 3;
//             if((front_mask & code) == 0) {
//                 // beq
//                 if(registers[s] == registers[t]) {
//                     counter = counter + i;
//                 }
//             } else {
//                 // bne
//                 if(registers[s] != registers[t]) {
//                     counter = counter + i;
//                 }
//             }
//         } else {
//             front_mask >>= 1;
//             if((front_mask & code) == 0) {
//                 // either addi / slti
//                 front_mask >>= 1;
//                 if((front_mask & code) == 0) {
//                     // addi
//                     registers[t] = registers[s] + i;
//                 } else {
//                     // slti
//                     if(registers[s] < i) {
//                         registers[t] = 1;
//                     } else {
//                         registers[t] = 0;
//                     }
//                 }
//             } else {
//                 front_mask >>= 1;
//                 if((front_mask & code) != 0) {
//                     // lui
//                     uint32_t result = i;
//                     result <<= 16;
//                     registers[t] = result;
//                 } else {
//                     // either andi / ori /syscall
//                     front_mask >>= 1;
//                     if((front_mask & code) == 0) {
//                         // andi
//                         registers[t] = (registers[s] & i);
//                     } else {
//                         // ori
//                         registers[t] = (registers[s] | i);
//                     }
//                 }
//             } 
//         }
//     }
//     return;
// }