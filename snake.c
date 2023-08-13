#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>

#define HEIGHT 30
#define WIDTH 50

bool g_isGameOver = false;
int g_x, g_y, g_fruit_x, g_fruit_y, g_score = 0;

// https://stackoverflow.com/questions/421860/capture-characters-from-standard-input-without-waiting-for-enter-to-be-pressed
char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}

void clear()
{
  system("cls||clear");
}

int random_in_range(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

void change_fruit_position()
{
  g_fruit_x = random_in_range(1, WIDTH - 2);
  g_fruit_y = random_in_range(1, HEIGHT - 2);
}

void draw()
{
  int x;
  int y;
  
  for (y = 0; y < HEIGHT; y++)
  {
    for (x = 0; x < WIDTH; x++)
    {
      if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1)
      {
        printf("#");
      }
      else if (g_y == y && g_x == x)
      {
        printf("0");  
      }
      else if (g_fruit_y == y && g_fruit_x == x)
      {
        printf("*");
      }
      else
      {
        printf(" ");
      }
      if (x == WIDTH - 1)
      {
        printf("\n");
      }
    }
  }

  printf("\nCurrent score: %d\n", g_score);
}

void process_input(char c)
{
  switch (c) {
    case 'w':
      g_y--;
      break;
    case 'a':
      g_x--;
      break;
    case 's':
      g_y++;
      break;
    case 'd':
      g_x++;
      break;
    case 'q':
      exit(0);
      break;
  }

  // touched a border -> game over
  if (g_x == 0 || g_y == 0 || g_x == WIDTH - 1 || g_y == HEIGHT - 1)
  {
    g_isGameOver = true;
  }

  // caught the fruit!
  if (g_x == g_fruit_x && g_y == g_fruit_y)
  {
    g_score++;
    change_fruit_position();
  }
}

int main ()
{
  // set the initial positions of everything
  g_y = HEIGHT / 2;
  g_x = WIDTH / 2;

  change_fruit_position();
  draw();
  
  while (!g_isGameOver)
  {
    char c;
    c = getch();
    process_input(c);
    clear();
    draw();
  }
  return 0;
}
