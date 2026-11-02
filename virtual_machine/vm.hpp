#ifndef VM_HPP
#define VM_HPP

#include "stack.hpp"

//----------------------------------------------------------------------------
typedef int vm_elem_t;

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "colours.hpp"
#include "../commands/commands.hpp" //вставка enum с командами
#include "const.hpp"                //файл с константами

//----------------------------------------------------------------------------
const char* const FILE_IN       = "../src/code.bin";

//----------------------------------------------------------------------------
enum vm_status
{
    VM_SUCCESS   = 0,
    VM_PIZDA     = 52,
    VM_END_SECS  = 1
};

struct processor
{
    vm_status prog_status;
    uint8_t* bite_code;
    size_t bite_code_size;

    size_t ip;
    bool is_number;
    bool is_register;
    bool is_memory;

    vm_elem_t registers[REGISTERS_CAPACITY];
    stack_lifo_t* data_stk;
    stack_lifo_t* ret_stk;
    int* ram;
};

typedef vm_status (*func_arr)(processor*, int);

//----------------------------------------------------------------------------
size_t len_of_file(const char* name);
vm_status txt_to_mass(const char* name, processor* proc);

vm_status vm_hlt(processor* proc, int line);
vm_status vm_add(processor* proc, int line);
vm_status vm_sub(processor* proc, int line);
vm_status vm_div(processor* proc, int line);
vm_status vm_out(processor* proc, int line);
vm_status vm_mul(processor* proc, int line);
vm_status vm_in(processor* proc, int line);

vm_status vm_push(processor* proc, int line);
// vm_status vm_pushr(processor* proc, int line);
vm_status vm_pop(processor* proc, int line);

vm_status vm_jmp(processor* pproc, int line);
vm_status vm_ja(processor* proc, int line);
vm_status vm_jae(processor* proc, int line);
vm_status vm_jb(processor* proc, int line);
vm_status vm_jbe(processor* proc, int line);
vm_status vm_je(processor* proc, int line);
vm_status vm_jne(processor* proc, int line);

vm_status vm_call(processor* proc, int line);
vm_status vm_ret(processor* proc, int line);

// vm_status vm_pushm(processor* pproc, int line);
// vm_status vm_popm(processor* pproc, int line);

vm_status vm_command(processor* proc, func_arr* arr, int line);
void print_status_vm(processor* proc, const char* FILE, int line);
vm_status print_vm_ram(processor* proc, int line);

//----------------------------------------------------------------------------
#define VM_SPEC_TYPEDEF "%d"
#define CMD_MASK 0b00011111

#define VM_COMMAND(proc, arr) vm_command(proc, arr, __LINE__)
#define PRINT_STATUS_VM(proc) print_status_vm(proc, __FILE__, __LINE__)
#define PRINT_VM_RAM(proc) print_vm_ram(proc, __LINE__)

//----------------------------------------------------------------------------
#endif