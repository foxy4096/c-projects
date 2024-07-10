/*

    A simple grep clone.
    I am going to name it: grab

*/

#include <stdio.h>
#include <string.h>

void print_usage(const char *program_name)
{
    printf("Usage: %s <needle> <haystack> [Options: -i, -n]\n", program_name);
    printf("Options:\n");
    printf("  -i\tCase insensitive search\n");
    printf("  -n\tShow line number\n");
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        print_usage(argv[0]);
        return 1;
    }

    int case_insensitive = 0;
    int show_line_number = 0;

    for (size_t i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-i") == 0)
        {
            case_insensitive = 1;
        }
        else if (strcmp(argv[i], "-n") == 0)
        {
            show_line_number = 1;
        }
    }

    char needle[256];
    strcpy(needle, argv[1]);
    char needle_lower[256];
    strcpy(needle_lower, needle);
    char line_lower[256];

    FILE *haystack = fopen(argv[2], "r");

    if (haystack == NULL)
    {
        perror("fopen");
        return 1;
    }

    char line[BUFSIZ];

    int line_num = 1;

    while (fgets(line, sizeof(line), haystack))
    {
        strcpy(line_lower, line);
        if (case_insensitive == 1)
        {
            strlwr(needle_lower);
            strlwr(line_lower);
        }

        if (strstr(line_lower, needle_lower))
        {
            if (show_line_number)
            {
                printf("%d: %s", line_num, line);
            }
            else
            {
                printf("%s", line);
            }
        }
        line_num++;
    }

    fclose(haystack);

    return 0;
}
