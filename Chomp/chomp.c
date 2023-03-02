#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "chomp.h"

char **chomp_states(size_t rows, size_t cols, size_t *n)
{
  if (rows > 9)
    {
      return NULL;
    }

  // count[i][j] is the number of rows*(i+1) chomp states with exactly j in the last column
  size_t count[cols + 1][rows + 1];
  for (size_t r = 0; r <= rows; r++)
    {
      count[0][r] = 1;
    }

  for (size_t c = 2; c <= cols + 1; c++)
    {
      for (size_t r = 0; r <= rows; r++)
	{
	  size_t sum = 0;
	  for (size_t r2 = r; r2 <= rows; r2++)
	    {
	      sum += count[c - 2][r2];
	      if (sum <= 0)
		{
		  // overflow
		  return NULL;
		}
	    }
	  count[c - 1][r] = sum;
	}
    }

  *n = count[cols][0];

  char **result = malloc(sizeof(char *) * *n);
  if (result != NULL)
    {
      // last state is whole pan
      result[*n - 1] = malloc(sizeof(char) * (cols + 1));
      if (result[*n - 1] != NULL)
	{
	  for (size_t i = 0; i < cols; i++)
	    {
	      result[*n - 1][i] = '0' + rows;
	    }
	  result[*n - 1][cols] = '\0';
	}

      // each previous state is obtained by decrementing the next
      for (size_t i = *n - 2; i <= *n - 2; i--)
	{
	  if (result[i + 1] != NULL)
	    {
	      result[i] = malloc(sizeof(char) * (cols + 1));
	      if (result[i] != NULL)
		{
		  strcpy(result[i], result[i + 1]);
		  // find last non-zero (know there will be one)
		  int j = cols - 1;
		  while (result[i][j] == '0')
		    {
		      j--;
		    }
		  
		  // decrement it
		  result[i][j]--;

		  // set all subsequent to equal it
		  for (int k = j + 1; k < cols; k++)
		    {
		      result[i][k] = result[i][j];
		    }
		}
	    }
	}
    }
  else
    {
      *n = 0;
    }

  return result;
}

char **chomp_successors(const char *s, size_t *n)
{
  *n = 0;

  size_t cols = strlen(s);

  // can take any spot; count the remaining spots to get number of successors
  for (size_t c = 0; c < cols; c++)
    {
      if (!isdigit(s[c]))
	{
	  *n = 0;
	  return NULL;
	}
      *n += s[c] - '0';
    }

  // allocate an array to hold all successors
  char **succ = malloc(sizeof(char *) * *n);
  if (succ != NULL)
    {
      // iterate over all (r, c) positions to eat
      size_t curr = 0;
      for (size_t c = 0; c < cols; c++)
	{
	  for (size_t r = 0; r < s[c] - '0'; r++)
	    {
	      succ[curr] = malloc(sizeof(char) * (cols + 1));
	      if (succ[curr] != NULL)
		{
		  // successor is same up to column c,
		  // r rows left in column c, and
		  // min(r, original height) in subsequent columns
		  strcpy(succ[curr], s);
		  succ[curr][c] = '0' + r;
		  for (size_t c2 = c + 1; c2 < cols; c2++)
		    {
		      if (succ[curr][c2] > succ[curr][c])
			{
			  succ[curr][c2] = succ[curr][c];
			}
		    }
		}
	      curr++;
	    }
	}
    }

  return succ;
}
