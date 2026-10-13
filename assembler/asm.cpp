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
#endif