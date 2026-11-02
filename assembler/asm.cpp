#ifndef ASM_CPP
#define ASM_CPP

#include "asm.hpp"

//----------------------------------------------------------------------------
asm_status open_files(FILE* &file_asm, FILE* &file_bite, const char* file, int line)
{
    file_asm  = fopen(FILE_ASM, "r");
    file_bite = fopen(FILE_BITE, "wb");
    if (file_asm == NULL)
    {
        printf(RED "Error opening assembler file in %s:%d\n" CRESET, file, line);
        return ASM_PIZDA;
    }
    else if (file_bite == NULL)
    {
        printf(RED "Error opening bitecode file in %s:%d\n" CRESET, file, line);
        return ASM_PIZDA;
    }

    return ASM_SUCCESS;
}

//----------------------------------------------------------------------------
void print_asm_stat(asm_status asm_stat, const char* file, int line)
{
    if (asm_stat == ASM_SUCCESS)
    {
        printf(GRN "Assembler status in %s:%d: ASM_SUCCESS\n" CRESET, file, line);
    }
    else if (asm_stat == ASM_PIZDA)
    {
        printf(RED "Assembler status in %s:%d: ASM_PIZDA\n" CRESET, file, line);
    }
}

//----------------------------------------------------------------------------
ssize_t len_of_file(const char* name)
{
    struct stat file_info;
    if (stat(name, &file_info) != 0)
    return -1;

    if (file_info.st_size <= 0)
        return 0;

    return file_info.st_size;
}

//----------------------------------------------------------------------------
size_t asm_to_buffer(char* buffer, FILE* file, size_t len)
{
    size_t readed = fread(buffer, 1, len, file);
    buffer[readed] = '\0';

    return readed;
}

//----------------------------------------------------------------------------
asm_status asm_first_go(char* buffer, size_t labels[])
{
    char command[BUFFER_SIZE] = "";
    char value_str[BUFFER_SIZE] = "";
    int value = 0, line = 1;
    size_t index = 0;

    int readed = 0;

    while (sscanf(buffer, " %9s%n", command, &readed) > 0)
    {
        buffer += readed;

        if (command[0] == ':')                                  //обработка меток
        {
            if (sscanf(command, " :%d%n", &value, &readed) != 1)
                return ASM_PIZDA;
            if (value < 0 || value >= LABELS_SIZE)
                return ASM_PIZDA;
            labels[value] = index;

            continue;
        }

        bool found = false;
        uint8_t command_id = 0;
        for (int i = 0; i < len_of_commands_array; i++)
        {
            if (strcmp(command, COMMANDS[i]) == 0)
            {
                command_id = (uint8_t)i;
                found = true;
                break;
            }
        }
        if (!found)
        {
            printf("Unknown command: %s, line: %d\n", command, line);
            return ASM_PIZDA;
        }

        index++;
        if (ARGS[command_id])
        {
            if (sscanf(buffer, " %d%n", &value, &readed) == 1 || sscanf(buffer, " :%d%n", &value, &readed) == 1)
            {
                buffer += readed;
                index += 4;
            }
            else
            {
                if (sscanf(buffer, " [%9[^]]]%n", value_str, &readed) != 1)
                {
                    if (sscanf(buffer, " %9s%n", value_str, &readed) != 1)
                    {
                        printf("Wrong argument, command: %s, line: %d\n", command, line);
                        return ASM_PIZDA;
                    }
                }
                buffer += readed;
                index++;
            }
        }

        line++;
    }

    return ASM_SUCCESS;
}

//----------------------------------------------------------------------------
asm_status asm_second_go(char* buffer, size_t labels[], FILE* file_bite)
{
    char command[BUFFER_SIZE] = "";
    char value_str[BUFFER_SIZE] = "";
    int value = 0;
    uint8_t value_r = 0;
    size_t value_l = 0;
    int line = 1;

    int readed = 0;

    while (sscanf(buffer, " %9s%n", command, &readed) > 0)
    {
        buffer += readed;

        if (command[0] == ':')                                  //обработка меток
        {
            sscanf(command, " :%d%n", &value, &readed);

            continue;
        }

        bool found = false;
        uint8_t command_id = 0;                                    //определение номера команды
        for (int i = 0; i < len_of_commands_array; i++)
        {
            if (strcmp(command, COMMANDS[i]) == 0)
            {
                command_id = (uint8_t)i;
                found = true;
                break;
            }
        }
        if (!found)
            return ASM_PIZDA;


        assembler ebasm = {};
        ebasm.buffer = buffer;
        ebasm.command_id = command_id;
        ebasm.file_bite = file_bite;
        
        if (command_id == PUSH || command_id == POP)           //отделение PUSH и POP
        {
            if (asm_push_pop(&ebasm) != ASM_SUCCESS)
                return ASM_PIZDA;

            buffer = ebasm.buffer;

            continue;
        }


        if (ARGS[command_id])
        {
            if (sscanf(buffer, " %9d%n", &value, &readed) == 1)
            {
                buffer += readed;
                command_id |= ARG_NUMBER;
            }
            else
            {
                if (sscanf(buffer, " [%9[^]]]%n", value_str, &readed) != 1)
                {
                    if (sscanf(buffer, " :%9zu%n", &value_l, &readed) != 1)          //проверка на jmp и call
                    {
                        if (sscanf(buffer, " %9s%n", value_str, &readed) != 1)
                        {
                            printf("Wrong argument, command: %s, line: %d\n", command, line);
                            return ASM_PIZDA;
                        }
                        else
                        {
                            if (!registers_id(value_str, &value_r))
                                return ASM_PIZDA;
                            command_id |= ARG_REGISTER;
                        }
                    }
                    else
                    {
                        value = (int)labels[value_l];
                        command_id |= ARG_NUMBER;
                    }
                }
                else
                {
                    command_id |= ARG_MEMORY;

                    if (!registers_id(value_str, &value_r))
                        return ASM_PIZDA;
                }
                buffer += readed;
            }
        }

        if (fwrite(&command_id, sizeof(command_id), 1, file_bite) != 1)
            return ASM_PIZDA;

        if ((command_id & ARG_NUMBER) == ARG_NUMBER)
        {
            if (fwrite(&value, sizeof(value), 1, file_bite) != 1)
                return ASM_PIZDA;
        }
        else if ((command_id & ARG_MEMORY) == ARG_MEMORY || (command_id & ARG_REGISTER) == ARG_REGISTER)
        {
            if (fwrite(&value_r, sizeof(value_r), 1, file_bite) != 1)
                return ASM_PIZDA;
        }

        line++;
    }

    return ASM_SUCCESS;
}

//----------------------------------------------------------------------------
asm_status asm_push_pop(assembler* ebasm)
{
    char value_str[BUFFER_SIZE] = "";
    int value = 0;
    uint8_t value_r = 0;

    int readed = 0;

    if (sscanf(ebasm -> buffer, " %d%n", &value, &readed) == 1)          //аргумент - число
    {
        ebasm -> buffer += readed;

        ebasm -> command_id |= ARG_NUMBER;
    }
    else
    {
        if (sscanf(ebasm -> buffer, " [%9[^]]]%n", value_str, &readed) != 1)
        {
            if (sscanf(ebasm -> buffer, " %9s%n", value_str, &readed) != 1)
            {
                printf(RED "Error in line in input file(code.asm)\n" CRESET);
                return ASM_PIZDA;
            }
            else                    //аргумент - _x
            {
                if (!registers_id(value_str, &value_r))
                    return ASM_PIZDA;

                ebasm -> command_id |= ARG_REGISTER;
            }
        }
        else                        //аргумент - [_x] (memory)
        {
            if (!registers_id(value_str, &value_r))
                return ASM_PIZDA;

            ebasm -> command_id |= ARG_MEMORY;
        }
        ebasm -> buffer += readed;
    }

    if (fwrite(&ebasm -> command_id, sizeof(ebasm -> command_id), 1, ebasm -> file_bite) != 1)
        return ASM_PIZDA;

    if ((ebasm -> command_id & ARG_NUMBER) == ARG_NUMBER)
    {
        if (fwrite(&value, sizeof(value), 1, ebasm -> file_bite) != 1)
            return ASM_PIZDA;
    }
    else
    {
        if (fwrite(&value_r, sizeof(value_r), 1, ebasm -> file_bite) != 1)
            return ASM_PIZDA;
    }

    return ASM_SUCCESS;
}

//----------------------------------------------------------------------------
bool registers_id(char* value_str, uint8_t* result)
{
    for (uint8_t i = 0; i < (int)(sizeof(registers_array) / sizeof(registers_array[0])); i++)
    {
        if (strcmp(value_str, registers_array[i]) == 0)
        {
            *result = i;
            return true;
        }
    }

    printf(RED "Error in register in input asm-file\n" CRESET);
    return false;
}

//----------------------------------------------------------------------------
void print_bite_file(FILE* file)
{
    rewind(file);

    uint8_t byte = 0;
    size_t index = 0;

    while (fread(&byte, sizeof(byte), 1, file) == 1)
    {
        printf("%04zu: %02X\n", index, byte);
        index++;
    }
}

//----------------------------------------------------------------------------
#endif