// This CLI app helps in quickly compling my shitty c code into executables

/* 
Code to complie this

cl.exe /Fe:ms.exe makeshit.c && del *.obj
*/

// DON'T USE THIS!

#include <stdio.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <code_index>\n", argv[0]);
        printf("%s clear\n", argv[0]);
        return 1;
    }

    // Compile the code with the given index (argv[1]
    const char *code_idx = argv[1];

    if (strncmp(argv[1], "clear", 5) == 0)
    {
        printf("Clearing all executables...\n");
        system("del *.obj");
        system("del *.exe");

        system("cls");
        return 0;
    }
    

    char compile_command[100];
    // The compile command to be executed
    sprintf(compile_command, "cl.exe code_%s.c /Fe:code_%s.exe && del *.obj", code_idx, code_idx);

    system(compile_command);
    printf("Code compiled successfully! :3\n");

    if (argc > 2)
    {
        if (strncmp(argv[2], "run", 3) == 0)
        {
            char run_command[100];
            sprintf(run_command, "code_%s.exe", code_idx);
            system("cls");
            system(run_command);
        }
    }

    return 0;
}
