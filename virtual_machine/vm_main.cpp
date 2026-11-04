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
            printf(RED "ERROR: the dump file is not opened\n" CRESET);
            return STK_DUMP_ERR;
        }
    }

    stack_status stk_status = SUCCESS;
    stack_lifo_t stk = {};
    stack_lifo_t ret = {};

    size_t file_len = len_of_file(FILE_IN);
    size_t count = file_len;

    processor proc = {};
    uint8_t* bite_code_arr = (uint8_t*) calloc(count + 2, sizeof(uint8_t));
    int* ram_arr = (int*) calloc(RAM_CAPACITY, sizeof(int));

    proc_init(&proc, bite_code_arr, ram_arr, &stk, &ret);

    PRINT_STACK_STATUS(stk_status);
    PRINT_STATUS_VM(&proc);

    proc.prog_status = txt_to_mass(FILE_IN, &proc);   //перевод многострочного файла в единый массив команд
    
    proc_execute(&proc);

    proc_destroy(&proc);

    PRINT_STACK_STATUS(stk_status);
    PRINT_STATUS_VM(&proc);
    printf(BLU "End of program\n" CRESET);

    return 0;
}
