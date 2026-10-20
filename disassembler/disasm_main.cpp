#include "disasm.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

//----------------------------------------------------------------------------
int main()
{
    file_exit = fopen(FILE_EXIT, "w");

    func_arr command_func_arr[len_of_commands_array] = 
    {
        disasm_hlt, disasm_add, disasm_sub, disasm_div, disasm_out, disasm_mul,
        disasm_push, disasm_pushr, disasm_popr, 
        disasm_jmp, disasm_ja, disasm_jae, disasm_jb, disasm_jbe, disasm_je, disasm_jne,
        disasm_pushm, disasm_popm,
    };

    size_t file_len = len_of_file(FILE_IN);
    size_t count = file_len;

    processor proc = {};
    proc.prog_status = DISASM_SUCCESS;
    proc.bite_code = (disasm_elem_t*) calloc(count + 2, sizeof(disasm_elem_t));
    proc.ip = 0;
    proc.ram = (disasm_elem_t*) calloc(RAM_CAPACITY, sizeof(disasm_elem_t));

    PRINT_STATUS_DISASM(&proc);

    proc.prog_status = txt_to_mass(FILE_IN, &proc);   //перевод многострочного файла в единый массив команд
    
    while ((proc.prog_status) != DISASM_END_SECS)
    {
        (proc.prog_status) = DISASM_COMMAND(&proc, command_func_arr);

        if ((proc.prog_status) == DISASM_PIZDA)
        {
            printf(RED "HUINYA happened, program ends, ip = %zu\n" CRESET, proc.ip);
            return DISASM_PIZDA;
        }
    }

    free(proc.bite_code);

    PRINT_STATUS_DISASM(&proc);
    printf(BLU "End of disasm program\n" CRESET);

    return 0;
}
