#ifndef DISASM_CPP
#define DISASM_CPP

#include "disasm.hpp"

FILE* file_exit = NULL;
//----------------------------------------------------------------------------
size_t len_of_file(const char* name)
{
    struct stat file_info;
    stat(name, &file_info);

    if (file_info.st_size <= 0)
        return 0;

    return (size_t)file_info.st_size;
}

//----------------------------------------------------------------------------
disasm_status txt_to_mass(const char* name, processor* proc)
{
    FILE* file_txt = NULL;
    file_txt = fopen(name, "r");

    if (file_txt == NULL)
    {
        printf(RED "Error bite_code file opening\n" CRESET);
        return DISASM_PIZDA;
    }

    disasm_elem_t elem = 0;
    size_t i = 0;
    int q = 0;

    while ((q = fscanf(file_txt, DISASM_SPEC_TYPEDEF, &elem)) != EOF)
    {
        if (q == 1)
            (proc -> bite_code)[i++] = elem;
    }
    proc -> len_bite_code = i;

    fclose(file_txt);

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_hlt(processor* proc, int line)
{
    fprintf(file_exit, "HLT\n");

    (proc -> ip)++;
    (void)line;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_add(processor* proc, int line)
{
    fprintf(file_exit, "ADD\n");

    (proc -> ip)++;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_sub(processor* proc, int line)
{
    fprintf(file_exit, "SUB\n");

    (proc -> ip)++;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_div(processor* proc, int line)
{
    fprintf(file_exit, "DIV\n");

    (proc -> ip)++;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_out(processor* proc, int line)
{
    fprintf(file_exit, "OUT\n");

    (proc -> ip)++;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_mul(processor* proc, int line)
{
    fprintf(file_exit, "MUL\n");

    (proc -> ip)++;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_in(processor* proc, int line)
{
    (void)line;
    fprintf(file_exit, "IN\n");

    (proc -> ip)++;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_push(processor* proc, int line)
{
    fprintf(file_exit, "PUSH %d\n", (proc -> bite_code)[proc -> ip + 1]);

    (proc -> ip) += 2;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_pushr(processor* proc, int line)
{
    fprintf(file_exit, "PUSHR %s\n", registers_array[(proc -> bite_code)[proc -> ip + 1]]);

    (proc -> ip) += 2;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_popr(processor* proc, int line)
{
    fprintf(file_exit, "POPR %s\n", registers_array[(proc -> bite_code)[proc -> ip + 1]]);

    (proc -> ip) += 2;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_jmp(processor* proc, int line)
{
    fprintf(file_exit, "JMP %d\n", (proc -> bite_code)[proc -> ip + 1]);

    (proc -> ip) += 2;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_ja(processor* proc, int line)
{
    fprintf(file_exit, "JA %d\n", (proc -> bite_code)[proc -> ip + 1]);
    (proc -> ip) += 2;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_jae(processor* proc, int line)
{
    fprintf(file_exit, "JAE %d\n", (proc -> bite_code)[proc -> ip + 1]);
    (proc -> ip) += 2;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_jb(processor* proc, int line)
{
    fprintf(file_exit, "JB %d\n", (proc -> bite_code)[proc -> ip + 1]);
    (proc -> ip) += 2;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_jbe(processor* proc, int line)
{
    fprintf(file_exit, "JBE %d\n", (proc -> bite_code)[proc -> ip + 1]);
    (proc -> ip) += 2;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_je(processor* proc, int line)
{
    fprintf(file_exit, "JE %d\n", (proc -> bite_code)[proc -> ip + 1]);
    (proc -> ip) += 2;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_jne(processor* proc, int line)
{
    fprintf(file_exit, "JNE %d\n", (proc -> bite_code)[proc -> ip + 1]);
    (proc -> ip) += 2;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_call(processor* proc, int line)
{
    (void)line;
    fprintf(file_exit, "CALL %d\n", (proc -> bite_code)[proc -> ip + 1]);

    (proc -> ip) += 2;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_ret(processor* proc, int line)
{
    fprintf(file_exit, "RET\n");

    (proc -> ip)++;
    (void)line;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_pushm(processor* proc, int line)
{
    fprintf(file_exit, "PUSHM [%s]\n", registers_array[(proc -> bite_code)[proc -> ip + 1]]);
    (proc -> ip) += 2;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_popm(processor* proc, int line)
{
    fprintf(file_exit, "POPM [%s]\n", registers_array[(proc -> bite_code)[proc -> ip + 1]]);
    (proc -> ip) += 2;

    return DISASM_SUCCESS;
}

//----------------------------------------------------------------------------
disasm_status disasm_command(processor* proc, func_arr* arr, int line)
{
    disasm_status disasm_stat = DISASM_SUCCESS;
    if (proc -> ip >= proc -> len_bite_code)
        return DISASM_END_SECS;
    disasm_elem_t command = (proc -> bite_code)[proc -> ip];

    disasm_stat = arr[command](proc, line);

    return disasm_stat;
}

//----------------------------------------------------------------------------
void print_status_disasm(processor* proc, const char* FILE_1, int line)
{
    if ((proc -> prog_status) == DISASM_SUCCESS || (proc -> prog_status) == DISASM_END_SECS)
        printf(GRN "DISASM status in %s:%d: %u\n" CRESET, FILE_1, line, (proc -> prog_status));
    else
        printf(YEL "DISASM status in %s:%d: %u\n" CRESET, FILE_1, line, (proc -> prog_status));
}

#endif