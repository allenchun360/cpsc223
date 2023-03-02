#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "location.h"
#include "cities.h"

/**
 * Unit test for the cities module.  Primarily tests and
 * times the find_city function.
 *
 * @version 0.1 public 2020-09-16
 */

/**
 * Tests that the cities list is not hard-coded as a sorted list.
 *
 * @param code a string of up to 3 characters, non-NULL
 * @param start an index into the cities array
 * @param end an index into the cities array
 */
void test_not_hard_coded(char *code, int start, int end);

/**
 * Calls find_city with the given codes and prints the resulting coordinates
 * to standard output.
 *
 * @param n a nonnegative integer
 * @param code a pointer to an array of n non-NULL strings of up to 3 chars
 * @param on false to skip searching and printing (for baseline timing)
 */
void test_find_cities(int n, char **codes, bool on);

/**
 * Calls initialize_city_database for timing purposes.
 *
 * @param on true to do the initialization, false otherwise (for a baseline)
 */
void time_initialize(bool on);

/**
 * Determines the time efficiency settings from the given list of strings.
 * The test size is the next-to-last and the test on/off flag is the
 * last element in the array.  Size is given as the string representation
 * of a nonnegative integer in decimal, and the flag is "0" or "1".
 *
 * @param n a nonnegative integer
 * @param args an array of n non-NULL strings
 * @param test_size a pointer to an int, non-NULL
 * @param on a pointer to a bool, non-NULL
 */
void get_timing_settings(int n, char **args, int *test_size, bool *on)
{
  if (test_size != NULL && on != NULL)
    {
      if (n - 2 >= 0)
	{
	  *on = atoi(args[n - 1]) == 1;
	  *test_size = atoi(args[n - 2]);
	}	      
    }
}

int main(int argc, char **argv)
{
  if (argc < 2)
    {
      fprintf(stderr, "USAGE: %s test-number [test-arguments...]\n", argv[0]);
      return 1;
    }

  int test_num = atoi(argv[1]);
  bool on = false;
  int test_size = 0;
  on = (argc >= 4 && atoi(argv[argc - 2]) == 1);
  test_size = 0;
  if (on)
    {
      test_size = atoi(argv[argc - 1]);
      if (test_size <= 0)
	{
	  fprintf(stderr, "%s: test size must be positive\n", argv[0]);
	  return 1;
	}
    }

  switch (test_num)
    {
    case 1:
      test_not_hard_coded("000", 1000, 0);
      break;

    case 2:
      test_find_cities(argc - 2, argv + 2, true);
      break;

    case 3:
      get_timing_settings(argc, argv, &test_size, &on);
      time_initialize(on);
      break;
      
    case 4:
      get_timing_settings(argc, argv, &test_size, &on);
      test_find_cities(argc - 4, argv + 2, on);
      break;
    }

  return 0;
}

void test_not_hard_coded(char *code, int start, int end)
{
  if (start < 0 || end < 0 || start >= city_count || end >= city_count)
    {
      fprintf(stderr, "invalid index");
      return;
    }

  if (code == NULL || strlen(code) > 3)
    {
      fprintf(stderr, "invalid code");
      return;
    }

  char *copy = malloc(strlen(code) + 1);
  strcpy(copy, code);
  cities[start].name = code;
  
  initialize_city_database();
  printf("After inserting %s at %d and initializing, cities[%d].name = \"%s\"\n", code, start, end, cities[end].name);

  free(copy);
}

void test_find_cities(int n, char **codes, bool on)
{
  initialize_city_database();

  if (n <= 0)
    {
      return;
    }

  if (codes == NULL)
    {
      fprintf(stderr, "array must not be NULL\n");
      return;
    }
  
  for (int i = 0; i < n; i++)
    {
      if (codes[i] == NULL || strlen(codes[i]) > 3)
	{
	  fprintf(stderr, "invalid code\n");
	  return;
	}

      location loc;
      if (on)
	{
	  if (find_city(codes[i], &loc))
	    {
	      printf("%s: %.6f %.6f\n", codes[i], loc.lat, loc.lon);
	    }
	  else
	    {
	      printf("%s: not found\n", codes[i]);
	    }
	}
    }
}
 
void time_initialize(bool on)
{
  if (on)
    {
      initialize_city_database();
    }
}
