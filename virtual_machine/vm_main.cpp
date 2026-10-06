#include "stack.cpp"
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
    stack_lifo_t stk1 = {};
    stk_status = stack_init_default(&stk1, START_CAPACITY);
    stk_status = stack_dump_default(&stk1);

    vm_status prog_status = VM_SUCCESS;
    size_t ip = 0;

    size_t file_len = len_of_file(FILE_IN);
    size_t count = file_len / sizeof(vm_elem_t);

    vm_elem_t* code = (vm_elem_t*) calloc(count, sizeof(vm_elem_t));

    prog_status = txt_to_mass(FILE_IN, code);                    //перевод многострочного файла в единый массив команд
    

}
