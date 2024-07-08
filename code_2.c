#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "user32.lib");
#pragma comment(lib, "kernel32.lib");

int main(void)
{
    MessageBox(NULL, "Hello World!", "Hello", MB_OK);
    return 0;
}