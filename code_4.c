/*

    A simple grep clone.
    I am going to name it: grab

*/

#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s needle haystack\n", argv[0]);
        return 1;
    }

    char needle[256];
    strcpy(needle, argv[1]);

    FILE *haystack = fopen(argv[2], "r");

    if (haystack == NULL)
    {
        perror("fopen");
        return 1;
    }

    char line[BUFSIZ];

    while (fgets(line, sizeof(line), haystack))
    {
        if (strstr(line, needle))
        {
            printf("%s", line);
        }
    }

    fclose(haystack);

    return 0;
}
