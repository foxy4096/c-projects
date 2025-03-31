// Just use gcc to compile, don't use my ms.exe to compile, 😭
// gcc code_11.c -o code_11.exe
// ./code_11.exe

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

void eval(int instr);
int fetch();


typedef enum {
    PSH, // 0:Push value onto stack/into a register
    POP, // 1:Pop value from stack
    ADD, // 2:Add top two values on stack
    SUB, // 3:Subtract top two values on stack
    MOV, // 4:Move value from one register to another
    CMP, // 5:Compare two values
    JZ, // 6:Jump if zero
    HLT, // 7:Halt execution
    JMP, // 8:Jump to instruction
    NOP, // 9:No operation (do nothing)
    OUT, // 10:Output register value
    IN, // 11:Input value into register
    JLZ, // 12:Jump if less than zero
    JGZ, // 13:Jump if greater than zero

} InstructionSet;

typedef struct {
    int zero_flag; // Zero flag
    int carry_flag; // Carry flag
    int sign_flag; // Sign flag
    int overflow_flag; // Overflow flag
    int parity_flag; // Parity flag
    int aux_carry_flag; // Auxiliary carry flag
} Flags;

Flags flags = {0};

typedef enum {
    R0, // Register 0
    R1, // Register 1
    R2, // Register 2
    R3, // Register 3
    R4, // Register 4
    R5, // Register 5
    R6, // Register 6
    PC, // Program Counter (Instruction Pointer)
    SP, // Stack Pointer
    NUM_REGS // Number of registers
} Registers;

int registers[NUM_REGS];

const int program[] = {
    // 0-2
    PSH, 0, 0,     // Push F0 = 0 onto stack
    // 3-5
    PSH, 0, 1,     // Push F1 = 1 onto stack
    // 6-9
    MOV, 0, R0, 0, // R0 = 0
    // 10-13
    MOV, 0, R1, 1, // R1 = 1
    // 14-16
    PSH, 0, 10,    // Push loop count (n = 10)
    // 17-20
    MOV, 0, R2, 10,// R2 = 10 (loop counter)
    // LOOP:
    // 21-23
    OUT, 1, R0,    // Print R0 (Fibonacci number)
    // 24-28
    ADD, 1, R0, R1, R3, // R3 = R0 + R1
    // 29-32
    MOV, 1, R0, R1, // R0 = R1
    // 33-36
    MOV, 1, R1, R3, // R1 = R3
    // 37-41
    SUB, 4, R2, 1, R2, // R2 = R2 - 1
    // 42-46
    CMP, 3, R2, 0, R4, // Compare R2 with 0 and set the R2 to 0 if equal and 1 if not equal
    // 47-50
    JLZ, 1, R2, 53, // If R2 == 0, jump to EXIT
    // 51 JMP PC
    // 51-52
    JMP, 21,      // Jump to LOOP
    // 53
    NOP,         // No operation (do nothing)
    // EXIT:
    // 54
    HLT
};

// The above program looks like assembly code, but it's actually just a C array of integers.



#define MEMORY_SIZE 256 // Size of memory for the virtual machine
int memory[MEMORY_SIZE]; // Memory for the virtual machine

#define sp (registers[SP])
#define ip (registers[PC])
#define MAX_STACK_SIZE 256 // Maximum stack size

bool running = true;
int stack[MAX_STACK_SIZE]; // Stack for the virtual machine
int main(int agc, char** argv) {
    while (running) {
        eval(fetch());
            ++ip;
    }
}

int fetch() {
    return program[ip];
}

void eval(int instr) {
    printf("Instruction: %d\n",instr);
    switch(instr) {
        case HLT: {
            printf("HLT\n");
            running = false;
            exit(0);
            break;
        }

        case PSH: {
            int mode = program[++ip];
            int value = program[++ip];
            if(sp >= MAX_STACK_SIZE - 1) {
                printf("Stack overflow\n");
                break;
            }
            if (mode == 0) {
                stack[++sp] = value;
                printf("PSH: Pushed value %d onto stack\n", value);
            } else if (mode == 1) {
                stack[++sp] = registers[value];
                printf("PSH: Pushed register R%d value %d onto stack\n", value, registers[value]);
            } else {
                printf("Invalid PSH mode %d\n", mode);
                eval(HLT);
            }
            break;
        }
        case POP: {
            if(sp < 0) {
                printf("Stack underflow\n");
                eval(HLT);
            }
            int mode = program[++ip];
            if (mode == 0) {
                printf("POP: %d\n", stack[sp--]);
            } else if (mode == 1 && program[++ip] >= R0 && program[ip] < NUM_REGS) {
                int dest = program[ip];
                registers[dest] = stack[sp--];
                printf("POP: %d INTO R%d\n", stack[sp], dest);
            } else {
                printf("Invalid POP mode, %d\n", mode);
                eval(HLT);
            }
            break;
        }
        case ADD: {
            int mode = program[++ip];
            if (mode == 0) {
                int a = stack[sp--];
                int b = stack[sp--];
                stack[++sp] = a+b;
                printf("ADD: %d + %d = %d\n", a, b, stack[sp]);
            } else if(mode == 1){
                int a = registers[program[++ip]];
                int b = registers[program[++ip]];
                int dest = program[++ip];
                registers[dest] = a+b;
                printf("ADD: %d + %d = %d\n", a, b, registers[dest]);
            } else if(mode == 2){
                int a = registers[program[++ip]];
                int b = stack[sp--];
                int dest = program[++ip];
                registers[dest] = a+b;
                printf("ADD: %d + %d = %d\n", a, b, registers[dest]);
            } else if(mode == 3){
                int a = stack[sp--];
                int b = registers[program[++ip]];
                int dest = program[++ip];
                registers[dest] = a+b;
                printf("ADD: %d + %d = %d\n", a, b, registers[dest]);
            } else {
                printf("Invalid ADD mode\n");
                eval(HLT);
            }
            break;
        }
        case SUB: {
            int mode = program[++ip];
            if (mode == 0) {
                int a = stack[sp--];
                int b = stack[sp--];
                stack[++sp] = a-b;
                printf("SUB: %d - %d = %d\n", a, b, stack[sp]);
            } else if (mode == 1) {
                int a = registers[program[++ip]];
                int b = registers[program[++ip]];
                int dest = program[++ip];
                registers[dest] = a-b;
                printf("SUB: %d - %d = %d\n", a, b, registers[dest]);
            } else if(mode == 2){
                int a = registers[program[++ip]];
                int b = stack[sp--];
                int dest = program[++ip];
                registers[dest] = a-b;
                printf("SUB: %d - %d = %d\n", a, b, registers[dest]);
            } else if(mode == 3){
                int a = stack[sp--];
                int b = registers[program[++ip]];
                int dest = program[++ip];
                registers[dest] = a-b;
                printf("SUB: %d - %d = %d\n", a, b, registers[dest]);
            } else if(mode == 4){
                int a = registers[program[++ip]];
                int b = program[++ip];
                int dest = program[++ip];
                registers[dest] = a-b;
                printf("SUB: %d - %d = %d\n", a, b, registers[dest]);
            }
            else {
                printf("Invalid SUB mode\n");
                eval(HLT);
            }
            break;
        }
        
        case CMP: {
            int mode = program[++ip];
            printf("CMP: %d\n", mode);
            if (mode == 0){
                int a = stack[sp--];
                int b = stack[sp--];
                if (a == b) {
                    stack[++sp] = 0; // Set flag to 0 if equal
                } else {
                    stack[++sp] = 1; // Set flag to 1 if not equal
                }
                printf("CMP:S-S %d == %d\n", a, b);
            } else if (mode == 1) {
                // Register comparison
                int a = registers[program[++ip]];
                int b = registers[program[++ip]];
                int dest = program[++ip];
                registers[dest] = 0; // Clear destination register
                if (a == b) {
                    registers[dest] = 0; // Set flag to 0 if equal
                } else {
                    registers[dest] = 1; // Set flag to 1 if not equal
                }
                printf("CMP:R-R %d == %d\n", a, b);
            }else if (mode == 2) {
                // Reg-Stack comparison
                int a = stack[sp--];
                int b = registers[program[++ip]];
                int dest = program[++ip];
                registers[dest] = 0; // Clear destination register
                if (a == b) {
                    registers[dest] = 0; // Set flag to 0 if equal
                } else {
                    registers[dest] = 1; // Set flag to 1 if not equal
                }
                printf("CMP:S-R %d == %d\n", a, b);
            }
            else if (mode == 3) {
                // Register-Constant comparison
                int a = registers[program[++ip]];
                int b = program[++ip];
                int dest = program[++ip];
                registers[dest] = 0; // Clear destination register
                if (a == b) {
                    registers[dest] = 0; // Set flag to 0 if equal
                } else if (a < b) {
                    registers[dest] = -1; // Set flag to -1 if less than
                } else if (a > b) {
                    registers[dest] = 1; // Set flag to 1 if greater than
                }
                else {
                    registers[dest] = 1; // Set flag to 1 if not equal
                }
                printf("PC: %d\n", ip);
                printf("CMP:R-C %d == %d\n", a, b);
            }                
             else {
                printf("Invalid CMP mode\n");
            }
            break;

        }
        case JMP: {
            int addr = program[++ip]; // Read address
            printf(">>>>JMP: Requesting Jump to %d (Before execution, IP=%d)\n", addr, ip);
            ip = addr-1;  // Try direct jump
            printf(">>>>JMP: Jumped to %d (After execution, IP=%d)\n", addr, ip);
            return;
            break;
        }
        case JZ: {
            int mode = program[++ip];
            if (mode == 0) {
                if (stack[sp] == 0) {
                    int addr = program[++ip];
                    ip = addr;
                    printf("JZ: Jump to %d\n", ip);
                } else {
                    printf("JZ: No jump, stack top is not zero\n");
                }
            } else if (mode == 1) {
                int reg = program[++ip];
                if (registers[reg] == 0) {
                    int addr = program[++ip];
                    ip = addr;
                    printf("JZ: Jump to %d\n", ip);
                } else {
                    ip++;
                    printf("JZ: No jump, register is not zero, %d\n", registers[reg]);
                }
            } else {
                printf("Invalid JZ mode\n");
                eval(HLT);

            }
            break;
        }
        case JLZ: {
            int mode = program[++ip];
            if (mode == 0) {
                int addr = program[++ip];
                if (stack[sp] < 0) {
                    ip = addr-1;
                    printf("JLZ: Jump to %d\n", ip);
                } else {
                    printf("JLZ: No jump, stack top is not less than zero\n");
                }
            } else if (mode == 1) {
                int reg = program[++ip];
                printf("JLZ: %d, R%d\n", registers[reg], reg);
                if (registers[reg] < 0) {
                    int addr = program[++ip];
                    ip = addr-1;
                    printf("JLZ: Jump to %d\n", ip);
                } else {
                    printf("JLZ: No jump, register is not less than zero, %d\n", registers[program[ip]]);
                }
            } else {
                printf("Invalid JLZ mode\n");
                eval(HLT);
            }
            break;
        }
        case JGZ: {
            int mode = program[++ip];
            int addr = program[++ip];
            if (mode == 0) {
                if (stack[sp] > 0) {
                    ip = addr;
                    printf("JGZ: Jump to %d\n", ip);
                } else {
                    printf("JGZ: No jump, stack top is not greater than zero\n");
                }
            } else if (mode == 1) {
                if (registers[program[++ip]] > 0) {
                    ip = addr;
                    printf("JGZ: Jump to %d\n", ip);
                } else {
                    printf("JGZ: No jump, register is not greater than zero\n");
                }
            } else {
                printf("Invalid JGZ mode\n");
                eval(HLT);
            }
            break;
        }
        case MOV: {
            int mode = program[++ip];
            int dest = program[++ip];
            int source = program[++ip];
            // Mode 0: Move value from constant to register
            // Mode 1: Move value from register to register
            // Mode 2: Move value from stack to register
            // Mode 3: Move value from register to stack
            // Mode 4: Move value from stack to stack [No, it does not makes sense]
            if (mode == 0) { // Move constant to register
                registers[dest] = source;
                printf("MOV: Moved constant %d to register R%d\n", source, dest);
            } else if (mode == 1) { // Move value from register to register
                registers[dest] = registers[source];
                printf("MOV: Moved value from register R%d to R%d\n", source, dest);
            } else if (mode == 2) { // Move value from stack to register
                registers[dest] = stack[sp--];
                printf("MOV: Moved value from stack to register R%d\n", dest);
            }
            else if (mode == 3) { // Move value from register to stack
                stack[++sp] = registers[source];
                printf("MOV: Moved value from register R%d to stack\n", source);
            } else {
                printf("Invalid MOV mode\n");
                eval(HLT);
            }
            break;
        }
        case OUT: {
            int mode = program[++ip];
            int value = program[++ip];
            if (mode == 0) { // Output value from stack
                printf("\033[1;32mOUT: Output value [%d] from stack\033[0m\n", stack[sp]);
            } else if (mode == 1) { // Output register value
                printf("\033[1;32mOUT: Output register R%d value [%d]\033[0m\n", mode, registers[value]);
            } else {
                printf("\033[1;31mInvalid OUT mode, %d\n\033[0m", mode);
                eval(HLT);
            }
            break;
        }
        case NOP: {
            printf("NOP: No operation\n");
            break;
        }
        default:
            printf("Unknown instruction %d\n", instr);
            break;
    }
}

/* 
    Just kill me already, otherwise I will keep writing this garbage code.
    I am not a programmer, I am a human being, and I have feelings too.
    I am not a machine, I am not a robot, I am not a computer.
    I am a person, and I have a life to live.
    I am not a slave to code, I am not a prisoner of my own mind.
    WHAT THE FUCK
    Anyway
*/