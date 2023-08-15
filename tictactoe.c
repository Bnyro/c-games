#include <stdio.h>
#include <stdbool.h>
#include <termios.h>

#define UNASSIGNED 0
#define PLAYERONE 1
#define PLAYERTWO 2
#define CHARONE 'X'
#define CHARTWO 'O'

int g_grid[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
int g_currentPlayer = PLAYERONE;

char get_player_char(int player)
{
  switch (player)
  {
    case PLAYERONE:
      return CHARONE;
    case PLAYERTWO:
      return CHARTWO;
  }

  return ' ';
}

void render()
{
  system("clear");
  
  int x, y;

  for (y = 0; y < 3; y++)
  {
    if (y != 0)
    {
      printf("\n---------\n");
    }
    for (x = 0; x < 3; x++)
    {
      if (x != 0)
      {
        printf(" | ");
      }
      
      char player = get_player_char(g_grid[x][y]);
      printf("%c", player);
    }
  }

  printf("\n");
}

bool handle_input(int x, int y)
{
  if (x > 2 || y > 2)
  {
    return false;
  }

  if (g_grid[x][y] != UNASSIGNED)
  {
    return false;
  }
  
  g_grid[x][y] = g_currentPlayer;

  return true;
}

int get_horizontal(int a, int b)
{
  return g_grid[a][b];
}

int get_vertical(int a, int b)
{
  return g_grid[b][a];
}

bool check_straight( int (*get_field) (int, int) )
{
  for (int a = 0; a < 3; a++)
  {
    int owner = UNASSIGNED;
    int b;
    
    for (int b = 0; b < 3; b++)
    {
      int current_field = get_field(a, b);
      if (b == 0)
      {
        owner = current_field;
      }
      else if (owner != current_field)
      {
        owner = UNASSIGNED;
        break;
      }
    }

    if (owner != UNASSIGNED)
    {
      return true;
    }
  }

  return false;
}

bool check_for_win()
{
  if (check_straight(&get_horizontal))
  {
    return true;
  }

  if (check_straight(&get_vertical))
  {
    return true;
  }

  int owner = UNASSIGNED;
  
  for (int i = 0; i < 3; i++)
  {
    int current_field = g_grid[i][2 - i];
    if (i == 0)
    {
      owner = current_field;
    }
    else if (owner != current_field)
    {
      owner = UNASSIGNED;
      break;
    }
  }

  if (owner != UNASSIGNED)
  {
    return true;
  }

  for (int i = 2; i >= 0; i--)
  {
    int current_field = g_grid[i][2 - i];
    if (i == 2)
    {
      owner = current_field;
    }
    else if (owner != current_field)
    {
      owner = UNASSIGNED;
      break;
    }
  }

  return owner != UNASSIGNED;
}

int main()
{
  render();

  while (true) {
    int x, y;

    printf("\nCurrent player: %d\n", g_currentPlayer);
    printf("X (1, 2, 3): ");
    scanf("%d", &x);
    printf("Y(1, 2, 3): ");
    scanf("%d", &y);

    bool accepted = handle_input(x - 1, y - 1);
    render();
    if (!accepted)
    {
      continue;
    }
    
    if (check_for_win())
    {
      printf("\nPlayer %d wins the game. Congratulations!\n", g_currentPlayer);
      break;
    }

    if (g_currentPlayer == PLAYERONE)
    {
      g_currentPlayer = PLAYERTWO;
    }
    else
    {
      g_currentPlayer = PLAYERONE;
    }
  }
  
  return 0;
}
