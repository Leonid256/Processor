#ifndef VM_CPP
#define VM_CPP

//#include "stack.cpp"
#include "vm.hpp"

//----------------------------------------------------------------------------
ssize_t len_of_file(const char* name)
{
    struct stat file_info;
    stat(name, &file_info);

    if (file_info.st_size <= 0)
        return 0;

    return file_info.st_size;
}

//----------------------------------------------------------------------------
vm_status txt_to_mass(const char* name, vm_elem_t* code)
{
    FILE* file_txt = NULL;
    file_txt = fopen(name, "r");

    if (file_txt == NULL)
    {
        printf(RED "Error code file opening\n" CRESET);
        return VM_PIZDA;
    }

    vm_elem_t elem = 0;
    size_t i = 0;
    int q = 0;

    while ((q = fscanf(file_txt, VM_SPEC_TYPEDEF, &elem)) != EOF)
    {
        if (q == 1)
            code[i++] = elem;
    }

    fclose(file_txt);

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------

vm_status vm_push(stack_lifo_t* stk, vm_elem_t elem, int line)
{
    vm_status vm_stat = VM_SUCCESS;

    if (stack_push(stk, elem, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_push:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_add(stack_lifo_t* stk, int line)
{
    vm_elem_t elem_1 = 0, elem_2 = 0;
    vm_status vm_stat = VM_SUCCESS;

    if (stack_pop(stk, &elem_1, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    if (stack_pop(stk, &elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (stack_push(stk, elem_1 + elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_add:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_sub(stack_lifo_t* stk, int line)
{
    vm_elem_t elem_1 = 0, elem_2 = 0;
    vm_status vm_stat = VM_SUCCESS;

    if (stack_pop(stk, &elem_1, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    if (stack_pop(stk, &elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (stack_push(stk, elem_1 - elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_sub:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_div(stack_lifo_t* stk, int line)
{
    vm_elem_t elem_1 = 0, elem_2 = 0;
    vm_status vm_stat = VM_SUCCESS;

    if (stack_pop(stk, &elem_1, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    if (stack_pop(stk, &elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;

    if (elem_2 == 0)
    {
        printf(RED "Error: attempt to divide by zero\n" CRESET);
        return VM_PIZDA;
    }

    if (stack_push(stk, elem_1 / elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_div:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_mul(stack_lifo_t* stk, int line)
{
    vm_elem_t elem_1 = 0, elem_2 = 0;
    vm_status vm_stat = VM_SUCCESS;

    if (stack_pop(stk, &elem_1, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    if (stack_pop(stk, &elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;

    if (stack_push(stk, elem_1 * elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_mul:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_comand(vm_elem_t* code, size_t* ip_ptr, stack_lifo_t* stk, int line)
{
    vm_status vm_stat = VM_SUCCESS;
    vm_elem_t elem_1 = 0, elem_2 = 0;

    vm_elem_t comand = code[*ip_ptr];

    switch (comand)
    {
        case (1):
            if (stack_push(stk, code[*ip_ptr + 1], line) != SUCCESS)
                vm_stat = VM_PIZDA;
            (*ip_ptr) += 2;
            break;
        case (2):
            if (stack_pop(stk, &elem_1, line) != SUCCESS) vm_stat = VM_PIZDA;
            if (stack_pop(stk, &elem_2, line) != SUCCESS) vm_stat = VM_PIZDA;
            if (stack_push(stk, elem_1 + elem_2, line) != SUCCESS) vm_stat = VM_PIZDA;
            (*ip_ptr)++;
            break;
        case (3):
            if (stack_pop(stk, &elem_1, line) != SUCCESS) vm_stat = VM_PIZDA;
            if (stack_pop(stk, &elem_2, line) != SUCCESS) vm_stat = VM_PIZDA;
            if (stack_push(stk, elem_2 - elem_1, line) != SUCCESS) vm_stat = VM_PIZDA;
            (*ip_ptr)++;
            break;
        case (4):
            if (stack_pop(stk, &elem_1, line) != SUCCESS) vm_stat = VM_PIZDA;
            if (stack_pop(stk, &elem_2, line) != SUCCESS) vm_stat = VM_PIZDA;
            if (elem_2 == 0)
            {
                printf(RED "Error: attempt to divide by zero\n" CRESET);
                vm_stat = VM_PIZDA;
                break;
            }
            if (stack_push(stk, elem_2 / elem_1, line) != SUCCESS) vm_stat = VM_PIZDA;
            (*ip_ptr)++;
            break;
        case (5):
            if (stack_pop(stk, &elem_1, line) != SUCCESS) vm_stat = VM_PIZDA;
            else printf(BLU "Result: " VM_SPEC_TYPEDEF CRESET "\n", elem_1);
            (*ip_ptr)++;
            break;
        case (6):
            if (stack_pop(stk, &elem_1, line) != SUCCESS) vm_stat = VM_PIZDA;
            if (stack_pop(stk, &elem_2, line) != SUCCESS) vm_stat = VM_PIZDA;
            if (stack_push(stk, elem_1 * elem_2, line) != SUCCESS) vm_stat = VM_PIZDA;
            (*ip_ptr)++;
            break;
        case (0):
            return VM_END_SECS;
            break;
        default:
            printf(RED "Error: syntax error, comand = <%d> not exist\n" CRESET, comand);
            (*ip_ptr)++;
            vm_stat = VM_PIZDA;
            break;

    }

    return vm_stat;
}

//----------------------------------------------------------------------------
void print_status_vm(vm_status prog_status, int line)
{
    if (prog_status == VM_SUCCESS || prog_status == VM_END_SECS)
        printf(GRN "VM status in %s:%d: %u\n" CRESET, __FILE__, line, prog_status);
    else
        printf(YEL "VM status in %s:%d: %u\n" CRESET, __FILE__, line, prog_status);
}

#endif