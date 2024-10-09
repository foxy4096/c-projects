// This is a snake game 🐍

#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

#define WIDTH 15
#define HEIGHT 20

int is_game_over;
int x, y, fruit_x, fruit_y, score;
int tail_x[100], tail_y[100];

int n_tail;

enum Direction
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum Direction dir;

void setup()
{
    is_game_over = 0;
    dir = STOP;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    fruit_x = rand() % WIDTH;
    fruit_y = rand() % HEIGHT;
    score = 0;
}

void draw(){
    system("cls");
    for (int i = 0; i < WIDTH; i++)
    {
        printf("==");
    }
    printf("\n");
    
}

int main(int argc, char const *argv[])
{
    // TODO: Do the main thing...
}
