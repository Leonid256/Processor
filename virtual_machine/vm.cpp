#include "stack.cpp"
#include "vm.hpp"

//----------------------------------------------------------------------------
size_t len_of_file(const char* name)
{
    struct stat file_info;
    stat(name, &file_info);

    return file_info.st_size;
}

//----------------------------------------------------------------------------
vm_status txt_to_mass(const char* name, vm_elem_t* code)
{
    FILE* file = NULL;
    file = fopen(name, "r");

    if (file == NULL)
    {
        printf(RED "Error code file opening\n" CRESET);
        return VM_PIZDA;
    }

    vm_elem_t elem = 0;
    size_t i = 0;

    while ((elem = fgetc(file)) != EOF)
    {
        code[i++] = elem;
    }

    fclose(file);

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
