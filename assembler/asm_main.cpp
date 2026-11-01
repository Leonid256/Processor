#include "asm.hpp"

//----------------------------------------------------------------------------
int main()
{
    FILE* file_asm  = NULL;
    FILE* file_bite = NULL;

    asm_status asm_stat = ASM_SUCCESS;

    asm_stat = OPEN_FILES(file_asm, file_bite);

    int labels[LABELS_SIZE] = {-1};

    PRINT_ASM_STAT(asm_stat);

    size_t len = (size_t)len_of_file(FILE_ASM);
    char* buffer_0 = (char*) calloc(len + 2 * sizeof(vm_elem_t), sizeof(vm_elem_t));
    char* buffer = buffer_0;

    asm_to_buffer(buffer_0, file_asm, len);

    asm_stat = asm_first_go(buffer, labels);
    
    //second passage//
    buffer = buffer_0;
    asm_stat = asm_second_go(buffer, labels, file_bite);
    

    fclose(file_asm);
    fclose(file_bite);

    free(buffer_0);

    PRINT_ASM_STAT(asm_stat);

}

