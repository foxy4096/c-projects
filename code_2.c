#include <windows.h>
#include <stdio.h>

/* 
    The `windows.h` help to manage the windows kernel and some files
*/

#pragma comment(lib, "user32.lib"); // For MessageBox
#pragma comment(lib, "kernel32.lib"); // For Managing windows kernel

int main(void)
{
    MessageBox(NULL, "Hello World!", "Hello", MB_OK);
    return 0;
}