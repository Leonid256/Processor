#include "asm.hpp"

//----------------------------------------------------------------------------
int main()
{
    FILE* file_asm  = NULL;
    FILE* file_bite = NULL;

    asm_status asm_stat = ASM_SUCCESS;

    asm_stat = open_files(file_asm, file_bite, __LINE__);

    char command[BUFFER_SIZE] = "";
    int value = 0, line = 1;
    char value_str[BUFFER_SIZE] = "";

    print_asm_stat(asm_stat, __LINE__);

    size_t len = (size_t)len_of_file(FILE_ASM);
    char* buffer = (char*) calloc(len + 2 * sizeof(vm_elem_t), sizeof(vm_elem_t));

    asm_to_buffer(buffer, file_asm, len);

    int readed = 0;

    while (sscanf(buffer, " %s%n", command, &readed) > 0)
    {
        buffer += readed;

        int command_id = 0;
        for (int i = 0; i < len_of_commands_array; i++)
        {
            if (strcmp(command, COMMANDS[i]) == 0)
            {
                command_id = i;
                break;
            }
        }

        if (command_id > MUL)
        {
            if (sscanf(buffer, " %d%n", &value, &readed))
            {
                fprintf(file_bite, "%d %d\n", command_id, value);
                buffer += readed;
            }
            else
            {
                
                if (sscanf(buffer, " [%[^]] ]%n", value_str, &readed) != 1)
                {
                    if (sscanf(buffer, " %s%n", value_str, &readed) != 1)
                    {
                        printf(RED "Error in %d line in input file(code.asm) in %s:%d\n" CRESET, 
                                    line, __FILE__, __LINE__);
                        asm_stat = ASM_PIZDA;
                        break;
                    }
                }
                buffer += readed;

                for (int i = 0; i < 4; i++)
                {
                    if (strcmp(value_str, registers_array[i]) == 0)
                    {
                        value = i;
                        break;
                    }
                }

                fprintf(file_bite, "%d %d\n", command_id, value);
            }
        }
        else
        {
            fprintf(file_bite, "%d\n", command_id);
        }

        if (asm_stat == ASM_PIZDA) break;

        line++;
    }

    fclose(file_asm);
    fclose(file_bite);

    print_asm_stat(asm_stat, __LINE__);

}

