#ifndef VM_HPP
#define VM_HPP

#include "stack.hpp"

//----------------------------------------------------------------------------
typedef int vm_elem_t;

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "colours.hpp"

//----------------------------------------------------------------------------
const char* const FILE_IN = "code.txt";

//----------------------------------------------------------------------------
enum vm_status
{
    VM_SUCCESS   = 0,
    VM_PIZDA     = 52,
    VM_END_SECS  = 1
};

//----------------------------------------------------------------------------
ssize_t len_of_file(const char* name);
vm_status txt_to_mass(const char* name, vm_elem_t* code);
vm_status vm_push(stack_lifo_t* stk, vm_elem_t elem, int line);
vm_status vm_add(stack_lifo_t* stk, int line);
vm_status vm_sub(stack_lifo_t* stk, int line);
vm_status vm_div(stack_lifo_t* stk, int line);
vm_status vm_mul(stack_lifo_t* stk, int line);
vm_status vm_comand(vm_elem_t* code, size_t* ip_ptr, stack_lifo_t* stk, int line);
void print_status_vm(vm_status prog_status, int line);

//----------------------------------------------------------------------------
#define VM_SPEC_TYPEDEF "%d"

#define VM_COMAND(code, ip_ptr, stk) vm_comand(code, ip_ptr, stk, __LINE__)
#define PRINT_STATUS_VM(prog_status) print_status_vm(prog_status, __LINE__)

//----------------------------------------------------------------------------
#endif