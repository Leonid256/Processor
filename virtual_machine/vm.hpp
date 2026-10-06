#ifndef VM_HPP
#define VM_HPP

//----------------------------------------------------------------------------
typedef int vm_elem_t;

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "colours.hpp"

//----------------------------------------------------------------------------
const char* FILE_IN = "code.txt";

//----------------------------------------------------------------------------
enum vm_status
{
    VM_SUCCESS = 0,
    VM_PIZDA   = 52
};

//----------------------------------------------------------------------------
size_t len_of_file(const char* name);
vm_status txt_to_mass(const char* name, vm_elem_t* code);
vm_status vm_push(stack_lifo_t* stk, vm_elem_t elem, int line);
vm_status vm_add(stack_lifo_t* stk, int line);
vm_status vm_sub(stack_lifo_t* stk, int line);
vm_status vm_div(stack_lifo_t* stk, int line);
vm_status vm_mul(stack_lifo_t* stk, int line);

//----------------------------------------------------------------------------
#endif