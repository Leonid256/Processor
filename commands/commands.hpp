//Автоматически сгенерированный файл. Не править руками!
#ifndef COMMANDS_HPP
#define COMMANDS_HPP

enum vm_command
{
    HLT   = 0,
    ADD   = 1,
    SUB   = 2,
    DIV   = 3,
    OUT   = 4,
    MUL   = 5,
    PUSH  = 6,
    PUSHR = 7,
    POPR  = 8,
    JMP   = 9,
    JA    = 10,
    JAE   = 11,
    JB    = 12,
    JBE   = 13,
    JE    = 14,
    JNE   = 15,
    PUSHM = 16,
    POPM  = 17,
};

const char* const COMMANDS[] = {"HLT", "ADD", "SUB", "DIV", "OUT", "MUL", "PUSH", "PUSHR", "POPR", "JMP", "JA", "JAE", "JB", "JBE", "JE", "JNE", "PUSHM", "POPM", };

const int len_of_commands_array = 18;

const char* const ARGS[] = {"false", "false", "false", "false", "false", "false", "true", "true", "true", "true", "true", "true", "true", "true", "true", "true", "true", "true", };

#endif
