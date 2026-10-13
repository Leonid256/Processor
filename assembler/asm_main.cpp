#include "asm.hpp"

//----------------------------------------------------------------------------
int main()
{
    FILE* file_asm  = NULL;
    FILE* file_bite = NULL;

    asm_status asm_stat = ASM_SUCCESS;

    asm_stat = open_files(file_asm, file_bite, __LINE__);

    //vm_elem_t* buffer = (vm_elem_t*)calloc(BUFFER_SIZE, sizeof(vm_elem_t));
    char command[BUFFER_SIZE] = "";
    int value = 0, line = 0, count = 0;

    print_asm_stat(asm_stat, __LINE__);

    while ((count = fscanf(file_asm, " %s %d", command, &value)) > 0)
    {
        // vm_command bite_comand = "HLT";

        for (size_t i = 0; i < sizeof(COMMANDS) / sizeof(char*); i++)
        {
            if (strcmp(command, COMMANDS[i]) == 0)
            {
                if ((count == 2 && strcmp(ARGS[i], "true")) || (count == 1 && strcmp(ARGS[i], "false")))
                {
                    printf(RED "Error in %d line in input file(code.asm) in %s:%d :: " 
                        "(count = %d | ARGS[%zu] = %s)\n" CRESET, 
                        line, __FILE__, __LINE__, count, i, ARGS[i]);
                    asm_stat = ASM_PIZDA;
                    break;
                }
                else if (count == 2)
                {
                    fprintf(file_bite, "%zu %d\n", i, value);
                    break;
                }
                else if (count == 1)
                {
                    fprintf(file_bite, "%zu\n", i);
                    break;
                }
            }
        }

        if (asm_stat == ASM_PIZDA) break;

        line++;
    }

    fclose(file_asm);
    fclose(file_bite);

    print_asm_stat(asm_stat, __LINE__);

}

