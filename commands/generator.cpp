#include <stdio.h>

const char* COMMANDS_OUT = "commands.hpp";

int main() 
{
    FILE *file = fopen(COMMANDS_OUT, "w");
    if (!file) return 1;

    fprintf(file, "//Автоматически сгенерированный файл. Не править руками!\n");
    fprintf(file, "#ifndef COMMANDS_HPP\n#define COMMANDS_HPP\n\n");

    //enum vm_command
    fprintf(file, "enum vm_command\n{\n");

    int comm_value = 0;
    #define X(name, args)                                           \
    {                                                               \
        fprintf(file, "    %-5s = %d,\n", #name, comm_value);       \
        comm_value++;                                               \
    }
    #include "../src/commands.txt"
    #undef X

    fprintf(file, "};\n\n");


    //массив COMMANDS
    fprintf(file, "const char* const COMMANDS[] = {");

    int len_comm = 0;
    #define X(name, args)                   \
    {                                       \
        fprintf(file, "\"%s\", ", #name);   \
        len_comm++;                         \
    }
    #include "../src/commands.txt"
    #undef X

    fprintf(file, "};\n\n");
    fprintf(file, "const int len_of_commands_array = %d;\n\n", len_comm);


    //массив наличие/отсутствие аргументов
    fprintf(file, "const bool ARGS[] = {");

    #define X(name, args) fprintf(file, "%s, ", #args);
    #include "../src/commands.txt"
    #undef X

    fprintf(file, "};\n\n");


    //enum argument_type
    int arg_type_num = 1;
    fprintf(file, "enum argument_type\n");
    fprintf(file, "{\n");
    fprintf(file, "    ARG_DIGIT      = %d,\n", arg_type_num++);
    fprintf(file, "    ARG_REGISTER   = %d,\n", arg_type_num++);
    fprintf(file, "    ARG_REG_MEMORY = %d,\n", arg_type_num++);
    fprintf(file, "};\n\n");

    fprintf(file, "#endif\n");

    fclose(file);
    printf("Файл %s успешно создан!\n", COMMANDS_OUT);
}
