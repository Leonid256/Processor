#ifndef ASM_CPP
#define ASM_CPP

#include "asm.hpp"

//----------------------------------------------------------------------------
asm_status open_files(FILE* &file_asm, FILE* &file_bite, int line)
{
    file_asm  = fopen(FILE_ASM, "r");
    file_bite = fopen(FILE_BITE, "w");
    if (file_asm == NULL)
    {
        printf(RED "Error opening assembler file in %s:%d\n" CRESET, __FILE__, line);
        return ASM_PIZDA;
    }
    else if (file_bite == NULL)
    {
        printf(RED "Error opening bitecode file in %s:%d\n" CRESET, __FILE__, line);
        return ASM_PIZDA;
    }

    return ASM_SUCCESS;
}

//----------------------------------------------------------------------------
void print_asm_stat(asm_status asm_stat, int line)
{
    if (asm_stat == ASM_SUCCESS)
    {
        printf(GRN "Assembler status in %s:%d: ASM_SUCCESS\n" CRESET, __FILE__, line);
    }
    else if (asm_stat == ASM_PIZDA)
    {
        printf(RED "Assembler status in %s:%d: ASM_PIZDA\n" CRESET, __FILE__, line);
    }
}

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
size_t asm_to_buffer(char* buffer, FILE* file, size_t len)
{
    len = (size_t)fread(buffer, sizeof(vm_elem_t), (size_t)len, file);

    return len;
}

//----------------------------------------------------------------------------
asm_status asm_first_go(char* buffer, int labels[])
{
    char command[BUFFER_SIZE] = "";
    char value_str[BUFFER_SIZE] = "";
    int value = 0, line = 1, index = 0;

    int readed = 0;

    while (sscanf(buffer, " %s%n", command, &readed) > 0)
    {
        buffer += readed;

        if (command[0] == ':')                                  //обработка меток
        {
            sscanf(command, " :%d%n", &value, &readed);
            labels[value] = index;

            index++;
            continue;
        }

        int command_id = 0;
        for (int i = 0; i < len_of_commands_array; i++)
        {
            if (strcmp(command, COMMANDS[i]) == 0)
            {
                command_id = i;
                break;
            }
        }

        if (ARGS[command_id])
        {
            if (sscanf(buffer, " %d%n", &value, &readed) == 1) buffer += readed;
            else
            {
                if (sscanf(buffer, " [%[^]]]%n", value_str, &readed) != 1)
                {
                    if (sscanf(buffer, " %s%n", value_str, &readed) != 1)
                    {
                        printf(RED "Error in %d line in input file(code.asm) in %s:%d\n" CRESET, 
                                    line, __FILE__, __LINE__);
                        return ASM_PIZDA;
                    }
                }

                buffer += readed;
            }

            index += 2;
        }
        else index += 1;

        line++;
    }

    return ASM_SUCCESS;
}

//----------------------------------------------------------------------------
asm_status asm_second_go(char* buffer, int labels[], FILE* file_bite)
{
    char command[BUFFER_SIZE] = "";
    char value_str[BUFFER_SIZE] = "";
    int value = 0, line = 1;

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

        if (command[0] == ':')                                  //обработка меток
        {
            sscanf(command, " :%d%n", &value, &readed);

            continue;
        }

        if (ARGS[command_id])
        {
            if (sscanf(buffer, " %d%n", &value, &readed) == 1)
            {
                fprintf(file_bite, "%d %d\n", command_id, value);
                buffer += readed;
            }
            else
            {
                
                if (sscanf(buffer, " [%[^]]]%n", value_str, &readed) != 1)
                {
                    if (sscanf(buffer, " :%d%n", &value, &readed) != 1)              //проверка на jmp
                    {
                        if (sscanf(buffer, " %s%n", value_str, &readed) != 1)
                        {
                            printf(RED "Error in %d line in input file(code.asm) in %s:%d\n" CRESET, 
                                        line, __FILE__, __LINE__);
                            return ASM_PIZDA;
                        }
                    }
                    else
                    {
                        value = labels[value];
                        fprintf(file_bite, "%d %d\n", command_id, value);

                        buffer += readed;
                        continue;
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

        line++;
    }

    return ASM_SUCCESS;
}
//----------------------------------------------------------------------------
#endif