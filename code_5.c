/*

    A simple clone of another popular unix command called `echo`.
    I am going to call it echoe, so that it doesn't conflict with the standard echo command.

*/

#include <stdio.h>

void print_usage(const char *prog_name){
    printf("Usage: %s <string> [options]\n", prog_name);
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        print_usage(argv[0]);
        return 1;
    }
    for (size_t i = 1; i < argc; i++)
    {
        printf("%s ",argv[i]);
    }
    printf("\n");
    return 0;
    
}
