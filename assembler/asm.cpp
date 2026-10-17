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
#endif