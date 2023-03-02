#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "chomp.h"
#include "string_helpers.h"
#include "gmap.h"

void free_value(const void *key, void *value, void *arg);

int main(int argc, char **argv)
{
  if (argc < 2 || strlen(argv[1]) == 0 || argv[1][0] == '0')
    {
      fprintf(stderr, "USAGE: %s state\n", argv[0]);
      return 1;
    }

  int cols = strlen(argv[1]);
  int rows = 1;
  for (char *s = argv[1]; *s; s++)
    {
      if (!isdigit(*s) || *s < *(s + 1))
      {
        // string contains non-digit or increasing digits
        fprintf(stderr, "USAGE: %s state\n", argv[0]);
        return 1;
      }

      // rows is max over all digits in string
      int h = *s - '0';
      if (h > rows)
	{
	  rows = h;
	}
    }

  // get all the possible states in the game
  size_t count;
  char **states = chomp_states(rows, cols, &count);
  if (states == NULL)
    {
      fprintf(stderr, "%s: error allocating memory\n", argv[0]);
      return 1;
    }

  // DONE: create a map from states to winning moves
  gmap *moves = gmap_create(string_copy, string_compare, string_hash_java, free);
  if (moves == NULL)
    {
      fprintf(stderr, "%s: error allocating memory\n", argv[0]);
      goto destroy_states;
    }
      
  // terminal position is a win automatically
  char *terminal = string_copy("terminal");
  if (terminal == NULL)
    {
      fprintf(stderr, "%s: error allocating memory\n", argv[0]);
      goto destroy_map;
    }

  // DONE: store (states[0], terminal) in map
  gmap_put(moves, states[0], terminal);
  
  // compute position value for each other state
  for (size_t i = 1; i < count; i++)
    {
      // get successor states
      size_t succ_count;
      char **succ = chomp_successors(states[i], &succ_count);

      if (succ == NULL && succ_count > 0)
	{
	  fprintf(stderr, "%s: error allocating memory\n", argv[0]);
	  goto destroy_map;
	  return 1;
	}
	  
      // TO DO: sequential search through states s' in succ
      // (results of the moves you can make from states[i])
      // to find one that is a losing position (has NULL as
      // its associate value in the map).
      // winning move; moving to that state is a winning move b/c is
      // leaves the opponent with no winning move
      
      size_t j = 0;
      while (j < succ_count && gmap_get(moves, succ[j]) != NULL /* && succ[j] does not have NULL value in map*/)
      {
        j++;
      }

      if (j < succ_count)
      {
        // DONE: winning move is to s'; make a copy of it
        char *copy = string_copy(succ[j]);
        if (copy == NULL)
          {
            fprintf(stderr, "%s: error allocating memory\n", argv[0]);
            string_array_free(succ, succ_count);
            goto destroy_map;
          }

        // TO DO: store key states[i] in the map with value set to that copy
        gmap_put(moves, states[i], copy);
      }
      else
      {
        gmap_put(moves, states[i], NULL);
        // TO DO: states[i] is a losing position; save NULL as its
        // value in the map
      }

      // done with the array of successors
      string_array_free(succ, succ_count);
    }

  // TO DO: look up winning move for position user asked about
  // (change the NULL in the next line to retrieve the value for
  // argv[1] instead)
  char *win = gmap_get(moves, argv[1]);
  if (win == NULL)
    {
      printf("LOSS\n");
    }
  else
    {
      printf("WIN: %s\n", win);
    }

  // clean up
 destroy_map:
  gmap_for_each(moves, free_value, NULL);
  gmap_destroy(moves);
 destroy_states:
  string_array_free(states, count);
}


void free_value(const void *key, void *value, void *arg)\
{
  free(value);
}
