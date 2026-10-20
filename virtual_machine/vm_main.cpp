#include "stack.hpp"
#include "vm.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

//----------------------------------------------------------------------------
int main()
{
    if (file != NULL)
    {
        printf(RED "ERROR: Attempt to open opened file\n" CRESET);
        return STK_DUMP_ERR;
    }
    else
    {
        file_init_default(STACK_DUMP_FILE,  "w");
        if (file == NULL)
        {
            printf(RED "ERROR: the file is not opened\n" CRESET);
            return STK_DUMP_ERR;
        }
    }

    func_arr command_func_arr[len_of_commands_array] = 
    {
        vm_hlt, vm_add, vm_sub, vm_div, vm_out, vm_mul, vm_in, 
        vm_push, vm_pushr, vm_popr, 
        vm_jmp, vm_ja, vm_jae, vm_jb, vm_jbe, vm_je, vm_jne,
        vm_pushm, vm_popm,
    };


    stack_status stk_status = SUCCESS;
    stack_lifo_t stk = {};

    size_t file_len = len_of_file(FILE_IN);
    size_t count = file_len;

    processor proc = {};
    proc.prog_status = VM_SUCCESS;
    proc.bite_code = (vm_elem_t*) calloc(count + 2, sizeof(vm_elem_t));
    proc.ip = 0;
    proc.data_stk = &stk;
    proc.ram = (vm_elem_t*) calloc(RAM_CAPACITY, sizeof(vm_elem_t));

    stk_status = STACK_INIT(proc.data_stk, START_CAPACITY);
    stk_status = STACK_DUMP(proc.data_stk);

    PRINT_STACK_STATUS(stk_status);
    PRINT_STATUS_VM(&proc);

    proc.prog_status = txt_to_mass(FILE_IN, &proc);   //перевод многострочного файла в единый массив команд
    
    while ((proc.prog_status) != VM_END_SECS)
    {
        (proc.prog_status) = VM_COMMAND(&proc, command_func_arr);

        stk_status = STACK_DUMP(proc.data_stk);

        if ((proc.prog_status) == VM_PIZDA)
        {
            printf(RED "HUINYA happened, program ends, ip = %zu\n" CRESET, proc.ip);
            return VM_PIZDA;
        }
    }

    //proc.prog_status = PRINT_VM_RAM(&proc);

    stk_status = STACK_DUMP(proc.data_stk);
    stk_status = STACK_DESTROY(proc.data_stk);

    free(proc.bite_code);

    PRINT_STACK_STATUS(stk_status);
    PRINT_STATUS_VM(&proc);
    printf(BLU "End of program\n" CRESET);

    return 0;
}
