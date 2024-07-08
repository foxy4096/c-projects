/*
    This is a simple clone of unix famous `cat` command.
    But I am going to call it dog 🐶.
    Why, I don't know...
*/

#include <stdio.h>

int main(int argc, char **argv)
{

    if (argc < 2)
    {
        fprintf(stderr, "Usage: dog file1 [file2 ...]\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");

    if (!file)
    {
        perror("fopen");
    }

    char buffer[BUFSIZ];

    while (!feof(file))
    {
        fgets(buffer, sizeof(buffer), file);
        printf("%s", buffer);
    }
    
    fclose(file);

    return 0;
}
