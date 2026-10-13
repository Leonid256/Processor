#ifndef ASM_HPP
#define ASM_HPP

//----------------------------------------------------------------------------
typedef int vm_elem_t;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "colours.hpp"
#include "../commands/commands.hpp" //вставка enum с командами и COMMANDS

//----------------------------------------------------------------------------
const char* const FILE_ASM  = "../src/code.asm";
const char* const FILE_BITE = "../src/code.txt";
const vm_elem_t BUFFER_SIZE = 10 * sizeof(vm_elem_t);


//----------------------------------------------------------------------------

enum asm_status
{
    ASM_SUCCESS   = 0,
    ASM_PIZDA     = 6767
};

//----------------------------------------------------------------------------
asm_status open_files(FILE* &file_asm, FILE* &file_bite, int line);
void print_asm_stat(asm_status asm_stat, int line);

//----------------------------------------------------------------------------
#endif