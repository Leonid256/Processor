#include "asm.hpp"

//----------------------------------------------------------------------------
int main()
{
    FILE* file_asm  = NULL;
    FILE* file_bite = NULL;

    asm_status asm_stat = ASM_SUCCESS;

    asm_stat = open_files(file_asm, file_bite, __LINE__);

    //char command[BUFFER_SIZE] = "";
    //char value_str[BUFFER_SIZE] = "";
    //int value = 0, line = 1, index = 0;
    int labels[LABELS_SIZE] = {-1};

    //int readed = 0;

    print_asm_stat(asm_stat, __LINE__);

    size_t len = (size_t)len_of_file(FILE_ASM);
    char* buffer_0 = (char*) calloc(len + 2 * sizeof(vm_elem_t), sizeof(vm_elem_t));
    char* buffer = buffer_0;

    asm_to_buffer(buffer_0, file_asm, len);

    asm_stat = asm_first_go(buffer, labels);
    // while (sscanf(buffer, " %s%n", command, &readed) > 0)
    // {
    //     buffer += readed;

    //     if (command[0] == ':')                                  //обработка меток
    //     {
    //         sscanf(command, " :%d%n", &value, &readed);
    //         labels[value] = index;

    //         //printf("label [:%d] = %d\n", value, index);

    //         buffer += readed;
    //         index++;
    //         continue;
    //     }

    //     int command_id = 0;
    //     for (int i = 0; i < len_of_commands_array; i++)
    //     {
    //         if (strcmp(command, COMMANDS[i]) == 0)
    //         {
    //             command_id = i;
    //             break;
    //         }
    //     }

    //     if (command_id > MUL)
    //     {
    //         if (sscanf(buffer, " %d%n", &value, &readed))
    //         {
    //             //fprintf(file_bite, "%d %d\n", command_id, value);
    //             buffer += readed;
    //         }
    //         else
    //         {
                
    //             if (sscanf(buffer, " [%[^]] ]%n", value_str, &readed) != 1)
    //             {
    //                 //if (sscanf(buffer, " :%d%n", value, &readed) != 1)              //проверка на jmp
    //                 //{
    //                     if (sscanf(buffer, " %s%n", value_str, &readed) != 1)
    //                     {
    //                         printf(RED "Error in %d line in input file(code.asm) in %s:%d\n" CRESET, 
    //                                     line, __FILE__, __LINE__);
    //                         asm_stat = ASM_PIZDA;
    //                         break;
    //                     }
    //                 //}
    //                 // else
    //                 // {
    //                 //     value = labels[value];
    //                 //     fprintf(file_bite, "%d %d\n", command_id, value);

    //                 //     buffer += readed;
    //                 //     continue;
    //                 // }
    //             }
    //             buffer += readed;

    //             for (int i = 0; i < 4; i++)
    //             {
    //                 if (strcmp(value_str, registers_array[i]) == 0)
    //                 {
    //                     value = i;
    //                     break;
    //                 }
    //             }

    //             //fprintf(file_bite, "%d %d\n", command_id, value);
    //         }

    //         index += 2;
    //     }
    //     else
    //     {
    //         //fprintf(file_bite, "%d\n", command_id);

    //         index += 1;
    //     }

    //     if (asm_stat == ASM_PIZDA) break;

    //     line++;
    // }

    //##########################################################################
    //second passage//##########################################################
    //##########################################################################
        
    buffer = buffer_0;
    //readed = 0;

    asm_stat = asm_second_go(buffer, labels, file_bite);
    // while (sscanf(buffer, " %s%n", command, &readed) > 0)
    // {
    //     buffer += readed;

    //     int command_id = 0;
    //     for (int i = 0; i < len_of_commands_array; i++)
    //     {
    //         if (strcmp(command, COMMANDS[i]) == 0)
    //         {
    //             command_id = i;
    //             break;
    //         }
    //     }

    //     if (command[0] == ':')                                  //обработка меток
    //     {
    //         sscanf(command, " :%d%n", &value, &readed);

    //         buffer += readed;
    //         index++;
    //         continue;
    //     }

    //     if (command_id > MUL)
    //     {
    //         if (sscanf(buffer, " %d%n", &value, &readed))
    //         {
    //             fprintf(file_bite, "%d %d\n", command_id, value);
    //             buffer += readed;
    //         }
    //         else
    //         {
                
    //             if (sscanf(buffer, " [%[^]] ]%n", value_str, &readed) != 1)
    //             {
    //                 if (sscanf(buffer, " :%d%n", &value, &readed) != 1)              //проверка на jmp
    //                 {
    //                     if (sscanf(buffer, " %s%n", value_str, &readed) != 1)
    //                     {
    //                         printf(RED "Error in %d line in input file(code.asm) in %s:%d\n" CRESET, 
    //                                     line, __FILE__, __LINE__);
    //                         asm_stat = ASM_PIZDA;
    //                         break;
    //                     }
    //                 }
    //                 else
    //                 {
    //                     value = labels[value];
    //                     fprintf(file_bite, "%d %d\n", command_id, value);

    //                     buffer += readed;
    //                     continue;
    //                 }
    //             }
    //             buffer += readed;

    //             for (int i = 0; i < 4; i++)
    //             {
    //                 if (strcmp(value_str, registers_array[i]) == 0)
    //                 {
    //                     value = i;
    //                     break;
    //                 }
    //             }

    //             fprintf(file_bite, "%d %d\n", command_id, value);
    //         }

    //         index += 2;
    //     }
    //     else
    //     {
    //         fprintf(file_bite, "%d\n", command_id);

    //         index += 1;
    //     }

    //     if (asm_stat == ASM_PIZDA) break;

    //     line++;
    // }

    fclose(file_asm);
    fclose(file_bite);

    free(buffer_0);

    print_asm_stat(asm_stat, __LINE__);

}

