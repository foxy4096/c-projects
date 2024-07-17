#include <stdio.h>
#include <string.h>

void *get_site(char *name, char *about, char *email)
{
    FILE *fp;
    char filename[50];
    sprintf(filename, "%s.html", name);
    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        perror("Error opening file");
    }

    char html_content[500];

    sprintf(html_content, "<html><body><h1>Hello, I am %s</h1><b>r<br><p>%s</p><br>Contact: %s</body><html></body>", name, about, email);
    fwrite(html_content, sizeof(char), strlen(html_content), fp);
    fclose(fp);
    printf("Website generated successfully!\n");
}

int main(int argc, char const *argv[])
{
    printf("WEBSITE GENERATOR\n");
    char name[100];
    char about[256];
    char email[100];
    printf("Enter Your name \n");
    scanf("%s", &name);
    printf("Enter about you \n");
    scanf("%s", &about);
    printf("Enter Your email \n");
    scanf("%s", &email);

    get_site(name, about, email);
    return 0;
}
