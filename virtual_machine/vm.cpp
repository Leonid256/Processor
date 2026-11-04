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
    file_txt = fopen(name, "rb");

    if (file_txt == NULL)
    {
        printf(RED "Error bite_code file opening\n" CRESET);
        return VM_PIZDA;
    }

    uint8_t elem = 0;
    size_t i = 0;
    while (fread(&elem, sizeof(elem), 1, file_txt) == 1)
    {
        proc -> bite_code[i++] = elem;
        proc -> bite_code_size = i;
    }

    fclose(file_txt);

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_hlt(processor* proc, int line)
{
    (proc -> ip)++;
    (void)line;

    return VM_EOP;
}

//----------------------------------------------------------------------------
vm_status vm_add(processor* proc, int line)
{
    vm_elem_t elem_1 = 0, elem_2 = 0;
    vm_status vm_stat = VM_SUCCESS;

    if (stack_pop((proc -> data_stk), &elem_1, line) != SUCCESS)
        return VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        return VM_PIZDA;
    
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
        return VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        return VM_PIZDA;
    
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
        return VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        return VM_PIZDA;

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
        return VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        return VM_PIZDA;

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
    if (scanf("%d", &value) != 1)
        return VM_PIZDA;

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
    bool is_number   = (proc -> bite_code)[proc -> ip] & ARG_NUMBER;
    bool is_register = (proc -> bite_code)[proc -> ip] & ARG_REGISTER;
    bool is_memory   = (proc -> bite_code)[proc -> ip] & ARG_MEMORY;

    if (is_number)
    {
        vm_elem_t value = 0;
        memcpy(&value, proc -> bite_code + proc -> ip + 1, sizeof(int));

        if (stack_push((proc -> data_stk), value, line) != SUCCESS)
        {
            printf(RED "Error in vm_push:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
            return VM_PIZDA;
        }

        (proc -> ip) += 1 + sizeof(vm_elem_t);
    }
    else if (is_register)
    {
        if ((proc -> bite_code)[proc -> ip + 1] > REGISTERS_CAPACITY) return VM_PIZDA;
        if (stack_push((proc -> data_stk), (proc -> registers)[(proc -> bite_code)[proc -> ip + 1]], line) != SUCCESS)
        {
            printf(RED "Error in vm_push:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
            return VM_PIZDA;
        }   

        (proc -> ip) += 2;
    }
    else if (is_memory)
    {
        if ((proc -> bite_code)[proc -> ip + 1] > REGISTERS_CAPACITY) return VM_PIZDA;
        int index = (proc -> registers)[(proc -> bite_code)[proc -> ip + 1]];

        if (index > RAM_CAPACITY) return VM_PIZDA;
        if (stack_push((proc -> data_stk), (proc -> ram)[index], line) != SUCCESS)
        {
            printf(RED "Error in vm_push:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
            return VM_PIZDA;
        }

        (proc -> ip) += 2;
    }
    else
    {
        printf(RED "Error in vm_push(wrong argument):(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_pop(processor* proc, int line)
{
    bool is_register = (proc -> bite_code)[proc -> ip] & ARG_REGISTER;
    bool is_memory   = (proc -> bite_code)[proc -> ip] & ARG_MEMORY;

    if (is_register)
    {
        if ((proc -> bite_code)[proc -> ip + 1] > REGISTERS_CAPACITY) return VM_PIZDA;
        if (stack_pop((proc -> data_stk), &(proc -> registers)[(proc -> bite_code)[proc -> ip + 1]], line) != SUCCESS)
        {
            printf(RED "Error in vm_pop:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
            return VM_PIZDA;
        }

        (proc -> ip) += 2;
    }
    else if (is_memory)
    {
        if ((proc -> bite_code)[proc -> ip + 1] > REGISTERS_CAPACITY) return VM_PIZDA;
        int index = (proc -> registers)[(proc -> bite_code)[proc -> ip + 1]];

        if (index > RAM_CAPACITY) return VM_PIZDA;
        if (stack_pop((proc -> data_stk), &(proc -> ram)[index], line) != SUCCESS)
        {
            printf(RED "Error in vm_pop:(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
            return VM_PIZDA;
        }

        (proc -> ip) += 2;
    }
    else
    {
        printf(RED "Error in vm_pop(wrong argument):(%s)%s:%d\n" CRESET, __func__, __FILE__, line);
        return VM_PIZDA;
    }

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_jmp(processor* proc, int line)
{
    (void) line;

    vm_elem_t address = 0;
    memcpy(&address, proc -> bite_code + proc -> ip + 1, sizeof(address));
    if (address < 0 || (size_t)address >= proc -> bite_code_size)
    {
        printf(RED "Error in vm_jmp:(%s)%s:%d\n" CRESET, __func__, __FILE__, __LINE__);
        return VM_PIZDA;
    }

    if (address < 0)
    {
        printf(RED "Error in vm_jmp:(%s)%s:%d\n" CRESET, __func__, __FILE__, __LINE__);
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
        return VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        return VM_PIZDA;

    if (elem_1 < elem_2)
    {
        vm_stat = vm_jmp(proc, line);
    }
    else
    {
        (proc -> ip) += 5;
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
        return VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        return VM_PIZDA;

    if (elem_1 <= elem_2)
    {
        vm_stat = vm_jmp(proc, line);
    }
    else
    {
        (proc -> ip) += 5;
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
        return VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        return VM_PIZDA;

    if (elem_1 > elem_2)
    {
        vm_stat = vm_jmp(proc, line);
    }
    else
    {
        (proc -> ip) += 5;
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
        return VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        return VM_PIZDA;

    if (elem_1 >= elem_2)
    {
        vm_stat = vm_jmp(proc, line);
    }
    else
    {
        (proc -> ip) += 5;
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
        return VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        return VM_PIZDA;

    if (elem_1 == elem_2)
    {
        vm_stat = vm_jmp(proc, line);
    }
    else
    {
        (proc -> ip) += 5;
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
        return VM_PIZDA;
    if (stack_pop((proc -> data_stk), &elem_2, line) != SUCCESS)
        return VM_PIZDA;

    if (elem_1 != elem_2)
    {
        vm_stat = vm_jmp(proc, line);
    }
    else
    {
        (proc -> ip) += 5;
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

    if (stack_push((proc -> ret_stk), (int)(proc -> ip + 5), __LINE__) != SUCCESS)
        return VM_PIZDA;

    vm_elem_t address = 0;
    memcpy(&address, proc -> bite_code + proc -> ip + 1, sizeof(address));
    if (address < 0 || (size_t)address >= proc -> bite_code_size)
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
    vm_elem_t address = 0;
    if (stack_pop((proc -> ret_stk), &address, __LINE__) != SUCCESS)
        return VM_PIZDA;

    if (address < 0 || (size_t)address >= proc -> bite_code_size)
    {
        printf(RED "Error in vm_ret:(%s)%s:%d\n" CRESET, __func__, __FILE__, __LINE__);
        return VM_PIZDA;
    }

    (proc -> ip) = (size_t)address;

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status vm_command(processor* proc, func_arr* arr, int line)
{
    vm_status vm_stat = VM_SUCCESS;
    uint8_t command = (proc -> bite_code)[proc -> ip] & CMD_MASK;

    vm_stat = arr[command](proc, line);

    return vm_stat;
}

//----------------------------------------------------------------------------
void print_status_vm(processor* proc, const char* FILE_1, int line)
{
    if ((proc -> prog_status) == VM_SUCCESS || (proc -> prog_status) == VM_EOP)
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

//----------------------------------------------------------------------------
vm_status proc_init(processor* proc, uint8_t* bite_code_arr, int* ram_arr, stack_lifo_t* stk, stack_lifo_t* ret)
{
    // stack_lifo_t stk = {};
    // stack_lifo_t ret = {};

    proc -> prog_status = VM_SUCCESS;
    if (bite_code_arr) proc -> bite_code = bite_code_arr;
    proc -> bite_code_size = 0;

    proc -> ip = 0;

    proc -> data_stk = stk;
    proc -> ret_stk  = ret;
    if (ram_arr) proc -> ram = ram_arr;

    STACK_INIT(proc -> data_stk, START_CAPACITY);
    STACK_DUMP(proc -> data_stk);
    STACK_INIT(proc -> ret_stk, START_CAPACITY);

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status proc_execute(processor* proc)
{
    func_arr command_func_arr[len_of_commands_array] = 
    {
        vm_hlt, vm_add, vm_sub, vm_div, vm_out, vm_mul, vm_in, 
        vm_push, vm_pop, 
        vm_jmp, vm_ja, vm_jae, vm_jb, vm_jbe, vm_je, vm_jne,
        vm_call, vm_ret,
    };

    while ((proc -> prog_status) != VM_EOP)
    {
        (proc -> prog_status) = VM_COMMAND(proc, command_func_arr);

        STACK_DUMP(proc -> data_stk);

        if ((proc -> prog_status) == VM_PIZDA)
        {
            printf(RED "HUINYA happened, program ends, ip = %zu\n" CRESET, proc -> ip);
            return VM_PIZDA;
        }
    }

    STACK_DUMP(proc -> data_stk);

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
vm_status proc_destroy(processor* proc)
{
    STACK_DESTROY(proc -> data_stk);
    STACK_DESTROY(proc -> ret_stk);

    free(proc -> bite_code);
    free(proc -> ram);

    return VM_SUCCESS;
}

//----------------------------------------------------------------------------
#endif