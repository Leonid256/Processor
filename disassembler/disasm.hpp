#ifndef DISASM_HPP
#define DISASM_HPP

//----------------------------------------------------------------------------
typedef int disasm_elem_t;

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "colours.hpp"
#include "../commands/commands.hpp" //вставка enum с командами

//----------------------------------------------------------------------------
const char* const FILE_IN       = "../src/code.txt";
const int REGISTERS_CAPACITY    = 5;
const int RAM_CAPACITY          = 100;
const char* const FILE_EXIT     = "../src/exit_code.asm";

extern FILE* file_exit;

//----------------------------------------------------------------------------
enum disasm_status
{
    DISASM_SUCCESS   = 0,
    DISASM_PIZDA     = 52,
    DISASM_END_SECS  = 1
};

struct processor
{
    disasm_status prog_status;
    disasm_elem_t* bite_code;
    size_t ip;
    disasm_elem_t registers[REGISTERS_CAPACITY];
    disasm_elem_t* ram;
};

const char* const registers_array[REGISTERS_CAPACITY] = {"sr", "ax", "bx", "cx", "dx"};

typedef disasm_status (*func_arr)(processor*, int);

//----------------------------------------------------------------------------
size_t len_of_file(const char* name);
disasm_status txt_to_mass(const char* name, processor* proc);

disasm_status disasm_hlt(processor* proc, int line);
disasm_status disasm_add(processor* proc, int line);
disasm_status disasm_sub(processor* proc, int line);
disasm_status disasm_div(processor* proc, int line);
disasm_status disasm_out(processor* proc, int line);
disasm_status disasm_mul(processor* proc, int line);
disasm_status disasm_push(processor* proc, int line);
disasm_status disasm_pushr(processor* proc, int line);
disasm_status disasm_popr(processor* proc, int line);

disasm_status disasm_jmp(processor* pproc, int line);
disasm_status disasm_ja(processor* proc, int line);
disasm_status disasm_jae(processor* proc, int line);
disasm_status disasm_jb(processor* proc, int line);
disasm_status disasm_jbe(processor* proc, int line);
disasm_status disasm_je(processor* proc, int line);
disasm_status disasm_jne(processor* proc, int line);

disasm_status disasm_pushm(processor* pproc, int line);
disasm_status disasm_popm(processor* pproc, int line);

disasm_status disasm_command(processor* proc, func_arr* arr, int line);
void print_status_disasm(processor* proc, const char* FILE, int line);

//----------------------------------------------------------------------------
#define DISASM_SPEC_TYPEDEF "%d"

#define DISASM_COMMAND(proc, arr) disasm_command(proc, arr, __LINE__)
#define PRINT_STATUS_DISASM(proc) print_status_disasm(proc, __FILE__, __LINE__)
#define PRINT_DISASM_RAM(proc) print_disasm_ram(proc, __LINE__)

//----------------------------------------------------------------------------
#endif