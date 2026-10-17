#ifndef VM_HPP
#define VM_HPP

#include "stack.hpp"

//----------------------------------------------------------------------------
typedef int vm_elem_t;

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "colours.hpp"
#include "../commands/commands.hpp" //вставка enum с командами

//----------------------------------------------------------------------------
const char* const FILE_IN       = "../src/code.txt";
const int REGISTERS_CAPACITY    = 4;
const int RAM_CAPACITY          = 100;

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
    vm_elem_t* bite_code;
    size_t ip;
    vm_elem_t registers[REGISTERS_CAPACITY];
    stack_lifo_t* data_stk;
    vm_elem_t* ram;
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
vm_status vm_push(processor* proc, int line);
vm_status vm_pushr(processor* proc, int line);
vm_status vm_popr(processor* proc, int line);

vm_status vm_command(processor* proc, func_arr* arr, int line);
void print_status_vm(processor* proc, const char* FILE, int line);

//----------------------------------------------------------------------------
#define VM_SPEC_TYPEDEF "%d"

#define VM_COMMAND(proc, arr) vm_command(proc, arr, __LINE__)
#define PRINT_STATUS_VM(proc) print_status_vm(proc, __FILE__, __LINE__)

//----------------------------------------------------------------------------
#endif