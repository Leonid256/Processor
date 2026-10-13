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

    #define X(name, value, args) fprintf(file, "    %-5s = %d,\n", #name, value);
    #include "../src/commands.txt"
    #undef X

    fprintf(file, "};\n\n");


    //массив COMMANDS
    fprintf(file, "const char* const COMMANDS[] = {");

    #define X(name, value, args) fprintf(file, "\"%s\", ", #name);
    #include "../src/commands.txt"
    #undef X

    fprintf(file, "};\n\n");


    //массив наличие/отсутствие аргументов
    fprintf(file, "const char* const ARGS[] = {");

    #define X(name, value, args) fprintf(file, "\"%s\", ", #args);
    #include "../src/commands.txt"
    #undef X

    fprintf(file, "};\n\n");


    fprintf(file, "#endif\n");

    fclose(file);
    printf("Файл %s успешно создан!\n", COMMANDS_OUT);
}
