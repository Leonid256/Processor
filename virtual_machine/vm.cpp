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
vm_status txt_to_mass(const char* name, processor* proc)
{
    FILE* file_txt = NULL;
    file_txt = fopen(name, "r");

    if (file_txt == NULL)
    {
        printf(RED "Error bite_code file opening\n" CRESET);
        return VM_PIZDA;
    }

    vm_elem_t elem = 0;
    size_t i = 0;
    int q = 0;

    while ((q = fscanf(file_txt, VM_SPEC_TYPEDEF, &elem)) != EOF)
    {
        if (q == 1)
            (proc -> bite_code)[i++] = elem;
    }

    fclose(file_txt);

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------

vm_status vm_push(processor* proc, vm_elem_t elem, int line)
{
    vm_status vm_stat = VM_SUCCESS;

    if (stack_push((proc -> data_stk), elem, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_push:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_add(processor* proc, int line)
{
    vm_elem_t elem_1 = 0, elem_2 = 0;
    vm_status vm_stat = VM_SUCCESS;

    if (stack_pop((proc -> data_stk), &elem_1, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (stack_push((proc -> data_stk), elem_1 + elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_add:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_sub(processor* proc, int line)
{
    vm_elem_t elem_1 = 0, elem_2 = 0;
    vm_status vm_stat = VM_SUCCESS;

    if (stack_pop((proc -> data_stk), &elem_1, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (stack_push((proc -> data_stk), elem_1 - elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_sub:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_div(processor* proc, int line)
{
    vm_elem_t elem_1 = 0, elem_2 = 0;
    vm_status vm_stat = VM_SUCCESS;

    if (stack_pop((proc -> data_stk), &elem_1, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;

    if (elem_2 == 0)
    {
        printf(RED "Error: attempt to divide by zero\n" CRESET);
        return VM_PIZDA;
    }

    if (stack_push((proc -> data_stk), elem_1 / elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_div:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_mul(processor* proc, int line)
{
    vm_elem_t elem_1 = 0, elem_2 = 0;
    vm_status vm_stat = VM_SUCCESS;

    if (stack_pop((proc -> data_stk), &elem_1, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;

    if (stack_push((proc -> data_stk), elem_1 * elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_mul:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_command(processor* proc, int line)
{
    vm_status vm_stat = VM_SUCCESS;
    vm_elem_t elem_1 = 0, elem_2 = 0;

    vm_elem_t command = (proc -> bite_code)[proc -> ip];

    switch (command)
    {
        case (PUSH):
            if (stack_push((proc -> data_stk), (proc -> bite_code)[proc -> ip + 1], line) != SUCCESS)
                vm_stat = VM_PIZDA;
            (proc -> ip) += 2;
            break;
        case (ADD):
            if (stack_pop((proc -> data_stk), &elem_1, line) != SUCCESS) vm_stat = VM_PIZDA;
            if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS) vm_stat = VM_PIZDA;
            if (stack_push((proc -> data_stk), elem_1 + elem_2, line) != SUCCESS) vm_stat = VM_PIZDA;
            (proc -> ip)++;
            break;
        case (SUB):
            if (stack_pop((proc -> data_stk), &elem_1, line) != SUCCESS) vm_stat = VM_PIZDA;
            if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS) vm_stat = VM_PIZDA;
            if (stack_push((proc -> data_stk), elem_2 - elem_1, line) != SUCCESS) vm_stat = VM_PIZDA;
            (proc -> ip)++;
            break;
        case (DIV):
            if (stack_pop((proc -> data_stk), &elem_1, line) != SUCCESS) vm_stat = VM_PIZDA;
            if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS) vm_stat = VM_PIZDA;
            if (elem_2 == 0)
            {
                printf(RED "Error: attempt to divide by zero\n" CRESET);
                vm_stat = VM_PIZDA;
                break;
            }
            if (stack_push((proc -> data_stk), elem_2 / elem_1, line) != SUCCESS) vm_stat = VM_PIZDA;
            (proc -> ip)++;
            break;
        case (OUT):
            if (stack_pop((proc -> data_stk), &elem_1, line) != SUCCESS) vm_stat = VM_PIZDA;
            else printf(BLU "Result: " VM_SPEC_TYPEDEF CRESET "\n", elem_1);
            (proc -> ip)++;
            break;
        case (MUL):
            if (stack_pop((proc -> data_stk), &elem_1, line) != SUCCESS) vm_stat = VM_PIZDA;
            if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS) vm_stat = VM_PIZDA;
            if (stack_push((proc -> data_stk), elem_1 * elem_2, line) != SUCCESS) vm_stat = VM_PIZDA;
            (proc -> ip)++;
            break;
        case (HLT):
            return VM_END_SECS;
            break;
        default:
            printf(RED "Error: syntax error, comand = <%d> not exist\n" CRESET, command);
            (proc -> ip)++;
            vm_stat = VM_PIZDA;
            break;

    }

    return vm_stat;
}

//----------------------------------------------------------------------------
void print_status_vm(processor* proc, const char* FILE, int line)
{
    if ((proc -> prog_status) == VM_SUCCESS || (proc -> prog_status) == VM_END_SECS)
        printf(GRN "VM status in %s:%d: %u\n" CRESET, FILE, line, (proc -> prog_status));
    else
        printf(YEL "VM status in %s:%d: %u\n" CRESET, FILE, line, (proc -> prog_status));
}

#endif