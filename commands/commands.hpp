//Автоматически сгенерированный файл. Не править руками!
#ifndef COMMANDS_HPP
#define COMMANDS_HPP

enum vm_command
{
    HLT   = 0,
    PUSH  = 1,
    ADD   = 2,
    SUB   = 3,
    DIV   = 4,
    OUT   = 5,
    MUL   = 6,
    PUSHR = 7,
    POPR  = 8,
    JMP   = 9,
};

const char* const COMMANDS[] = {"HLT", "PUSH", "ADD", "SUB", "DIV", "OUT", "MUL", "PUSHR", "POPR", "JMP", };

const char* const ARGS[] = {"false", "true", "false", "false", "false", "false", "false", "true", "true", "true", };

#endif
