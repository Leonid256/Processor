#ifndef VM_CPP
#define VM_CPP

//#include "stack.cpp"
#include "vm.hpp"

//----------------------------------------------------------------------------
size_t len_of_file(const char* name)
{
    struct stat file_info;
    stat(name, &file_info);

    if (file_info.st_size <= 0)
        return 0;

    return (size_t)file_info.st_size;
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
vm_status vm_hlt(processor* proc, int line)
{
    (proc -> ip)++;
    (void)line;

    return VM_END_SECS;
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

    (proc -> ip)++;

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
    
    if (stack_push((proc -> data_stk), elem_2 - elem_1, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_sub:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    (proc -> ip)++;

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

    if (elem_1 == 0)
    {
        printf(RED "Error: attempt to divide by zero\n" CRESET);
        return VM_PIZDA;
    }

    if (stack_push((proc -> data_stk), elem_2 / elem_1, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_div:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    (proc -> ip)++;

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_out(processor* proc, int line)
{
    vm_elem_t elem = 0;

    if (stack_pop((proc -> data_stk), &elem, line) != SUCCESS)
    {
        printf(RED "Error in vm_out:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }
    else 
        printf(BLU "Result: " VM_SPEC_TYPEDEF CRESET "\n", elem);

    (proc -> ip)++;

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

    (proc -> ip)++;

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_in(processor* proc, int line)
{
    vm_status vm_stat = VM_SUCCESS;

    vm_elem_t value = 0;
    scanf("%d", &value);

    if (stack_push((proc -> data_stk), value, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_in:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    (proc -> ip)++;

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_push(processor* proc, int line)
{
    vm_status vm_stat = VM_SUCCESS;

    if (stack_push((proc -> data_stk), (proc -> bite_code)[proc -> ip + 1], line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_push:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    (proc -> ip) += 2;

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_pushr(processor* proc, int line)
{
    vm_status vm_stat = VM_SUCCESS;

    if (stack_push((proc -> data_stk), (proc -> registers)[(proc -> bite_code)[proc -> ip + 1]], line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_pushr:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    (proc -> ip) += 2;

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_popr(processor* proc, int line)
{
    vm_status vm_stat = VM_SUCCESS;

    if (stack_pop((proc -> data_stk), &(proc -> registers)[(proc -> bite_code)[proc -> ip + 1]], line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_popr:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    (proc -> ip) += 2;

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_jmp(processor* proc, int line)
{
    int address = (proc -> bite_code)[proc -> ip + 1];

    if (address < 0)
    {
        printf(RED "Error in vm_jmp:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    (proc -> ip) = (size_t)address;

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_ja(processor* proc, int line)
{
    vm_elem_t elem_1 = 0, elem_2 = 0;
    vm_status vm_stat = VM_SUCCESS;

    if (stack_pop((proc -> data_stk), &elem_1, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;

    if (elem_1 > elem_2)
    {
        vm_stat = vm_jmp(proc, line);
    }
    else
    {
        (proc -> ip) += 2;
    }

    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_ja:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_jae(processor* proc, int line)
{
    vm_elem_t elem_1 = 0, elem_2 = 0;
    vm_status vm_stat = VM_SUCCESS;

    if (stack_pop((proc -> data_stk), &elem_1, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;

    if (elem_1 >= elem_2)
    {
        vm_stat = vm_jmp(proc, line);
    }
    else
    {
        (proc -> ip) += 2;
    }

    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_jae:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_jb(processor* proc, int line)
{
    vm_elem_t elem_1 = 0, elem_2 = 0;
    vm_status vm_stat = VM_SUCCESS;

    if (stack_pop((proc -> data_stk), &elem_1, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;

    if (elem_1 < elem_2)
    {
        vm_stat = vm_jmp(proc, line);
    }
    else
    {
        (proc -> ip) += 2;
    }

    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_jb:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_jbe(processor* proc, int line)
{
    vm_elem_t elem_1 = 0, elem_2 = 0;
    vm_status vm_stat = VM_SUCCESS;

    if (stack_pop((proc -> data_stk), &elem_1, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;

    if (elem_1 <= elem_2)
    {
        vm_stat = vm_jmp(proc, line);
    }
    else
    {
        (proc -> ip) += 2;
    }

    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_jbe:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_je(processor* proc, int line)
{
    vm_elem_t elem_1 = 0, elem_2 = 0;
    vm_status vm_stat = VM_SUCCESS;

    if (stack_pop((proc -> data_stk), &elem_1, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;

    if (elem_1 == elem_2)
    {
        vm_stat = vm_jmp(proc, line);
    }
    else
    {
        (proc -> ip) += 2;
    }

    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_je:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_jne(processor* proc, int line)
{
    vm_elem_t elem_1 = 0, elem_2 = 0;
    vm_status vm_stat = VM_SUCCESS;

    if (stack_pop((proc -> data_stk), &elem_1, line) != SUCCESS)
        vm_stat = VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        vm_stat = VM_PIZDA;

    if (elem_1 != elem_2)
    {
        vm_stat = vm_jmp(proc, line);
    }
    else
    {
        (proc -> ip) += 2;
    }

    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_jne:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_call(processor* proc, int line)
{
    (void)line;
    vm_status vm_stat = VM_SUCCESS;

    if (stack_push((proc -> ret_stk), (int)(proc -> ip + 2), __LINE__) != SUCCESS)
        vm_stat = VM_PIZDA;

    int address = (proc -> bite_code)[proc -> ip + 1];

    if (vm_stat == VM_PIZDA || address < 0)
    {
        printf(RED "Error in vm_call:(%s)%s:%d\n" CRESET, __func__, __FILE__, __LINE__);
        return VM_PIZDA;
    }

    (proc -> ip) = (size_t)address;

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_ret(processor* proc, int line)
{
    (void)line;
    int address = 0;
    if (stack_pop((proc -> ret_stk), &address, __LINE__) != SUCCESS)
        return VM_PIZDA;

    if (address < 0 )
    {
        printf(RED "Error in vm_ret:(%s)%s:%d\n" CRESET, __func__, __FILE__, __LINE__);
        return VM_PIZDA;
    }

    (proc -> ip) = (size_t)address;

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_pushm(processor* proc, int line)
{
    vm_status vm_stat = VM_SUCCESS;

    int index = (proc -> registers)[(proc -> bite_code)[proc -> ip + 1]];
    if (stack_push((proc -> data_stk), (proc -> ram)[index], line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_pushm:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    (proc -> ip) += 2;

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_popm(processor* proc, int line)
{
    vm_status vm_stat = VM_SUCCESS;

    int index = (proc -> registers)[(proc -> bite_code)[proc -> ip + 1]];
    if (stack_pop((proc -> data_stk), &(proc -> ram)[index], line) != SUCCESS)
        vm_stat = VM_PIZDA;
    
    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in vm_popm:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    (proc -> ip) += 2;

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_command(processor* proc, func_arr* arr, int line)
{
    vm_status vm_stat = VM_SUCCESS;
    vm_elem_t command = (proc -> bite_code)[proc -> ip];

    vm_stat = arr[command](proc, line);

    return vm_stat;
}

//----------------------------------------------------------------------------
void print_status_vm(processor* proc, const char* FILE_1, int line)
{
    if ((proc -> prog_status) == VM_SUCCESS || (proc -> prog_status) == VM_END_SECS)
        printf(GRN "VM status in %s:%d: %u\n" CRESET, FILE_1, line, (proc -> prog_status));
    else
        printf(YEL "VM status in %s:%d: %u\n" CRESET, FILE_1, line, (proc -> prog_status));
}

//----------------------------------------------------------------------------
vm_status print_vm_ram(processor* proc, int line)
{
    vm_status vm_stat = VM_SUCCESS;

    printf("vvvvvvvvvvvvvvvvvv|RAM|vvvvvvvvvvvvvvvvvv\n");
    for (int i = 0; i < RAM_CAPACITY; i++)
    {
        if (i % 10 != 0)
        {
            if (proc -> ram[i] != 0)
                printf("%3d ", proc -> ram[i]);
            else
                printf(" .  ");
        }
        else
        {
            printf("\n");
            if (proc -> ram[i] != 0)
                printf("%3d ", proc -> ram[i]);
            else
                printf(" .  ");
        }
    }
    printf("\n\n");
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

    if (vm_stat == VM_PIZDA)
    {
        printf(RED "Error in print_vm_ram:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return vm_stat;
}

#endif