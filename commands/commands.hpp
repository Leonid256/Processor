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
    POP   = 8,
    JMP   = 9,
    JA    = 10,
    JAE   = 11,
    JB    = 12,
    JBE   = 13,
    JE    = 14,
    JNE   = 15,
    CALL  = 16,
    RET   = 17,
};

const char* const COMMANDS[] = {"HLT", "ADD", "SUB", "DIV", "OUT", "MUL", "IN", "PUSH", "POP", "JMP", "JA", "JAE", "JB", "JBE", "JE", "JNE", "CALL", "RET", };

const int len_of_commands_array = 18;

const bool ARGS[] = {false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, false, };

//битовые маски для битовой записи команды
#define ARG_NUMBER   0b00100000
#define ARG_REGISTER 0b01000000
#define ARG_MEMORY   0b10000000

#endif
