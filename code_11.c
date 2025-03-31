#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

void eval(int instr);
int fetch();


typedef enum {
    PSH, // Push value onto stack/into a register
    POP, // Pop value from stack
    ADD, // Add top two values on stack
    SUB, // Subtract top two values on stack
    MOV, // Move value from one register to another
    CMP, // Compare two values
    JZ, // Jump if zero
    HLT, // Halt execution
    JMP, // Jump to instruction
    NOP, // No operation (do nothing)
    OUT, // Output register value
    IN, // Input value into register
    JLZ, // Jump if less than zero
    JGZ, // Jump if greater than zero

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
    PSH, 0, 0,     // Push F0 = 0 onto stack
    PSH, 0, 1,     // Push F1 = 1 onto stack
    MOV, 0, R0, 0, // R0 = 0
    MOV, 0, R1, 1, // R1 = 1
    PSH, 0, 10,    // Push loop count (n = 10)
    MOV, 0, R2, 10,// R2 = 10 (loop counter)
    // LOOP:
    OUT, 1, R0,    // Print R0 (Fibonacci number)
    ADD, 1, R0, R1, R3, // R3 = R0 + R1
    MOV, 1, R0, R1, // R0 = R1
    MOV, 1, R1, R3, // R1 = R3
    SUB, 1, R2, 1, R2, // R2 = R2 - 1
    CMP, 1, R2, 0, // Compare R2 with 0
    JLZ, 1, R2, 18, // If R2 == 0, jump to EXIT
    JMP, 11,      // Jump to LOOP
    // EXIT:
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
int main(int agc, char** argv){
    while(running) {
        eval(fetch());
        ++ip;
    }
}

int fetch() {
    return program[ip];
}

void eval(int instr) {
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
                printf("Invalid PSH mode\n");
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
                printf("Invalid POP mode\n");
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
            } else {
                printf("Invalid SUB mode\n");
                eval(HLT);
            }
            break;
        }
        
        case CMP: {
            int mode = program[++ip];
            if (mode == 0){
                int a = stack[sp--];
                int b = stack[sp--];
                if (a == b) {
                    stack[++sp] = 0; // Set flag to 0 if equal
                } else {
                    stack[++sp] = 1; // Set flag to 1 if not equal
                }
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
            }
             else {
                printf("Invalid CMP mode\n");
            }
            break;

        }
        case JMP: {
            int addr = program[++ip];
            ip = addr;
            printf("JMP: INTO %d\n", ip);
            break;
        }
        case JZ: {
            int mode = program[++ip];
            int addr = program[++ip];
            if (mode == 0) {
                if (stack[sp] == 0) {
                    ip = addr;
                    printf("JZ: Jump to %d\n", ip);
                } else {
                    printf("JZ: No jump, stack top is not zero\n");
                }
            } else if (mode == 1) {
                if (registers[program[++ip]] == 0) {
                    ip = addr;
                    printf("JZ: Jump to %d\n", ip);
                } else {
                    printf("JZ: No jump, register is not zero\n");
                }
            } else {
                printf("Invalid JZ mode\n");
                eval(HLT);

            }
            break;
        }
        JLZ: {
            printf("JLZ\n");
            int mode = program[++ip];
            int addr = program[++ip];
            if (mode == 0) {
                if (stack[sp] < 0) {
                    ip = addr;
                    printf("JLZ: Jump to %d\n", ip);
                } else {
                    printf("JLZ: No jump, stack top is not less than zero\n");
                }
            } else if (mode == 1) {
                if (registers[program[++ip]] < 0) {
                    ip = addr;
                    printf("JLZ: Jump to %d\n", ip);
                } else {
                    printf("JLZ: No jump, register is not less than zero\n");
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
                printf("OUT: Output value %d from stack\n", stack[sp]);
            } else if (mode >= R0 && mode < NUM_REGS) { // Output register value
                printf("OUT: Output register R%d value %d\n", mode, registers[value]);
            } else {
                printf("Invalid OUT mode\n");
                eval(HLT);
            }
            break;
        }
        default:
            printf("Unknown instruction %d\n", instr);
            break;
    }
}