#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
  char character;
  int color;
} tile;

int zpow(int a, int b);
tile** sqgrid(int l);
void printsqgrid(tile** grid, int l);
void solve(tile** grid, int l, int iniy, int inix, int fy, int fx);
void addl(tile** grid, int y, int x, int t);

char pch = '@';
int gcolor = 21;
int gl = 0;
FILE* fp;

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "Use:\n$ ./adoq <n>\n");
    exit(1);
  }

  fp = fopen("results.ans", "w");

  if (fp == NULL)
  {
    fprintf(stderr, "Can't open results file");
  }

  srand(time(NULL));

  int k = atoi(argv[1]);
  int m = zpow(2, k);
  gl = m;

  int fy = rand() % m;
  int fx = rand() % m;

  tile** grid = sqgrid(m);

  grid[fy][fx].character = '#';
  grid[fy][fx].color = 1;

  solve(grid, m, 0, 0, fy, fx);

  printsqgrid(grid, m);

  free(grid);
}

int zpow(int a, int b)
{
  int r = 1;
  for (int i = 0; i < b; i++)
  {
    r *= a;
  }
  return r;
}

tile** sqgrid(int l)
{
  tile** grid = calloc(l, sizeof(tile*));

  if (grid == NULL)
  {
    fprintf(stderr, "Out of memory\n");
    exit(1);
  }

  for (int i = 0; i < l; i++)
  {
    tile* row = calloc(l, sizeof(tile));
    if (row == NULL)
    {
      fprintf(stderr, "Out of memory\n");
      exit(1);
    }
    grid[i] = row;
  }

  for (int y = 0; y < l; y++)
  {
    for (int x = 0; x < l; x++)
    {
      grid[y][x].character = '.';
      grid[y][x].color = 0;
    }
  }

  return grid;
}

void printsqgrid(tile** grid, int l)
{
  l += 2;
  for (int y = 0; y < l; y++)
  {
    for (int x = 0; x < l-1; x++)
    {
      if (y == 0 || y == l-1)
      {
        if (x == 0 || x == l-1)
        {
          putc('+', fp);
        } else {
          putc('-', fp);
          putc('+', fp);
        }
      } else if (x == 0 || x == l-1) {
        putc('|', fp);
      } else {
        fprintf(fp, "\033[38;5;%dm%c\x1b[0m", grid[y-1][x-1].color, grid[y-1][x-1].character);
        if (grid[y-1][x-1].color != grid[y-1][x].color)
        {
          putc('|', fp);
        } else {
          if (grid[y-1][x-1].character == '.')
          {
            putc('|', fp);
          } else {
            fprintf(fp, "\033[38;5;%dm%c\x1b[0m", grid[y-1][x-1].color, grid[y-1][x-1].character);
          }
        }
      }
    }
    putc('\n', fp);
    if (y > 0 && y < l-2)
    {
      putc('+', fp);
      for (int x = 1; x < l-1; x++)
      {
        if (grid[y-1][x-1].color != grid[y][x-1].color)
        {
          putc('-', fp);
        } else {
          if (grid[y-1][x-1].character == '.')
          {
            putc('-', fp);
          } else {
            fprintf(fp, "\033[38;5;%dm%c\x1b[0m", grid[y-1][x-1].color, grid[y-1][x-1].character);
          }
        }
        putc('+', fp);
      }
      putc('\n', fp);
    }
  }
}

void solve(tile** grid, int l, int iniy, int inix, int fy, int fx)
{
  if (l == 1)
  {
    return;
  }

  printsqgrid(grid, gl);
  fprintf(fp, "\n");

  int nl = l/2;

  int ny = iniy + nl-1;
  int nx = inix + nl-1;

  /* debug */
  /* fprintf(fp, "c:%c l:%d nl:%d iy:%d ix:%d ny:%d nx:%d fy:%d fx:%d\n", pch, l, nl, iniy, inix, ny, nx, fy, fx); */

  /* top left */
  if (fy < iniy + nl && fx < inix + nl)
  {
    addl(grid, ny, nx, 3);
    solve(grid, nl, iniy, inix, fy, fx);
    solve(grid, nl, iniy, nx+1, ny, nx+1);
    solve(grid, nl, ny+1, inix, ny+1, nx);
    solve(grid, nl, ny+1, nx+1, ny+1, nx+1);
  } else if (fy < iniy + nl && fx >= inix + nl) { /* top right */
    addl(grid, ny, nx, 2);
    solve(grid, nl, iniy, inix, ny, nx);
    solve(grid, nl, iniy, nx+1, fy, fx);
    solve(grid, nl, ny+1, inix, ny+1, nx);
    solve(grid, nl, ny+1, nx+1, ny+1, nx+1);
  } else if (fy >= iniy + nl && fx < inix + nl) { /* bottom left */
    addl(grid, ny, nx, 1);
    solve(grid, nl, iniy, inix, ny, nx);
    solve(grid, nl, iniy, nx+1, ny, nx+1);
    solve(grid, nl, ny+1, inix, fy, fx);
    solve(grid, nl, ny+1, nx+1, ny+1, nx+1);
  } else /* if (fy >= iniy + nl && fx >= inix + nl) */ { /* bottom right */
    addl(grid, ny, nx, 0);
    solve(grid, nl, iniy, inix, ny, nx);
    solve(grid, nl, iniy, nx+1, ny, nx+1);
    solve(grid, nl, ny+1, inix, ny+1, nx);
    solve(grid, nl, ny+1, nx+1, fy, fx);
  }



}

void addl(tile** grid, int y, int x, int t)
{
  switch (t)
  {
    /* ##
     * #
     */
    case 0:
      grid[y][x].character = pch;
	grid[y][x].color = gcolor;
      grid[y][x+1].character = pch;
	grid[y][x+1].color = gcolor;
      grid[y+1][x].character = pch;
	grid[y+1][x].color = gcolor;
      break;

    /* ##
     *  #
     */
    case 1:
      grid[y][x].character = pch;
	grid[y][x].color = gcolor;
      grid[y][x+1].character = pch;
	grid[y][x+1].color = gcolor;
      grid[y+1][x+1].character = pch;
	grid[y+1][x+1].color = gcolor;
      break;

    /* #
     * ##
     */
    case 2:
      grid[y][x].character = pch;
	grid[y][x].color = gcolor;
      grid[y+1][x].character = pch;
	grid[y+1][x].color = gcolor;
      grid[y+1][x+1].character = pch;
	grid[y+1][x+1].color = gcolor;
      break;

    /*  #
     * ##
     */
    case 3:
      grid[y][x+1].character = pch;
	grid[y][x+1].color = gcolor;
      grid[y+1][x].character = pch;
	grid[y+1][x].color = gcolor;
      grid[y+1][x+1].character = pch;
	grid[y+1][x+1].color = gcolor;
      break;
  }

  /* pch = ((pch-'0'+1) % 76) + '0'; */
  gcolor = ((gcolor + ((rand() % 8) + 1)) % 210) + 21;
}
