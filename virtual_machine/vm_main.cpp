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

    stack_status stk_status = SUCCESS;
    vm_status prog_status = VM_SUCCESS;

    stack_lifo_t stk1 = {};
    stk_status = STACK_INIT(&stk1, START_CAPACITY);
    stk_status = STACK_DUMP(&stk1);

    PRINT_STACK_STATUS(stk_status);
    PRINT_STATUS_VM(prog_status);

    size_t ip = 0;

    size_t file_len = (size_t)len_of_file(FILE_IN);
    size_t count = file_len / sizeof(vm_elem_t);

    vm_elem_t* code = (vm_elem_t*) calloc(count + 2 * sizeof(vm_elem_t), sizeof(vm_elem_t));

    prog_status = txt_to_mass(FILE_IN, code);   //перевод многострочного файла в единый массив команд
    
    while (prog_status != VM_END_SECS)
    {
        prog_status = VM_COMMAND(code, &ip, &stk1);

        stk_status = STACK_DUMP(&stk1);

        if (prog_status == VM_PIZDA)
        {
            printf(RED "HUINYA happened, program ends, ip = %zu\n" CRESET, ip);
            return VM_PIZDA;
        }
    }

    stk_status = STACK_DUMP(&stk1);
    stk_status = STACK_DESTROY(&stk1);

    PRINT_STACK_STATUS(stk_status);
    PRINT_STATUS_VM(prog_status);
    printf(GRN "End of program\n" CRESET);
    return 0;
}
