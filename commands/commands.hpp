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
    IN    = 6,
    PUSH  = 7,
    PUSHR = 8,
    POPR  = 9,
    JMP   = 10,
    JA    = 11,
    JAE   = 12,
    JB    = 13,
    JBE   = 14,
    JE    = 15,
    JNE   = 16,
    PUSHM = 17,
    POPM  = 18,
};

const char* const COMMANDS[] = {"HLT", "ADD", "SUB", "DIV", "OUT", "MUL", "IN", "PUSH", "PUSHR", "POPR", "JMP", "JA", "JAE", "JB", "JBE", "JE", "JNE", "PUSHM", "POPM", };

const int len_of_commands_array = 19;

const bool ARGS[] = {false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, };

#endif
