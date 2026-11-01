#ifndef ASM_HPP
#define ASM_HPP

//----------------------------------------------------------------------------
typedef int vm_elem_t;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "colours.hpp"
#include "../commands/commands.hpp"             //вставка enum с командами и COMMANDS
#include "../virtual_machine/const.hpp"         //файл с константами

//----------------------------------------------------------------------------
const char* const FILE_ASM      = "../src/code.asm";
const char* const FILE_BITE     = "../src/code.bin";
const vm_elem_t BUFFER_SIZE     = 10 * sizeof(vm_elem_t);

//----------------------------------------------------------------------------

enum asm_status
{
    ASM_SUCCESS   = 0,
    ASM_PIZDA     = 6767
};

const char* const registers_array[REGISTERS_CAPACITY] = {"ax", "bx", "cx", "dx", "sr"};

struct assembler
{
    char* buffer;
    uint8_t command_id = 0;
    FILE* file_bite;
};

//----------------------------------------------------------------------------
asm_status open_files(FILE* &file_asm, FILE* &file_bite, const char* file, int line);
void print_asm_stat(asm_status asm_stat, const char* file, int line);
ssize_t len_of_file(const char* name);
size_t asm_to_buffer(char* buffer, FILE* file, size_t len);
asm_status asm_first_go(char* buffer, size_t labels[]);
asm_status asm_second_go(char* buffer, size_t labels[], FILE* file_bite);
asm_status asm_push_pop(assembler* ebasm);
bool registers_id(char* value_str, uint8_t* result);
void print_bite_file(FILE* file);

//----------------------------------------------------------------------------
#define OPEN_FILES(file_asm, file_bite) open_files(file_asm, file_bite, __FILE__, __LINE__)
#define PRINT_ASM_STAT(asm_stat) print_asm_stat(asm_stat, __FILE__, __LINE__)

//----------------------------------------------------------------------------
#endif