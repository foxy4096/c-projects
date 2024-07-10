#include <stdio.h>
#include <string.h>

void *get_site(char *name, char *about, char *email)
{
    printf("<html><body><h1>Hello, I am %s</h1><b>r<br><p>%s</p><br>Contact: %s</body><html></body>", name, about, email);
}

int main(int argc, char const *argv[])
{
    printf("WEBSITE GENERATOR\n");
    char name[100];
    char about[256];
    char email[100];
    printf("Enter Your name \n");
    scanf("%99s", &name);
    printf("Enter about you \n");
    scanf("%[^\n]", &about);
    printf("Enter Your email \n");
    scanf("%99s", &email);

    get_site(name, about, email);
    return 0;
}
