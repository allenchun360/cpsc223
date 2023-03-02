#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "strategy.h"
#include "blotto.h"
#include "arrays.h"

struct _strategy
{
  size_t num_locations;
  int num_units;
  int *distro;
  double weight;
  size_t size;
  size_t capacity;
};

#define STRATEGY_INITIAL_CAPACITY 1

strategy *strategy_create(size_t num_locations, size_t num_units)
{
  strategy *result = malloc(sizeof(strategy));

  if (result != NULL)
    {
      result->num_locations = num_locations;
      result->num_units = num_units;
      result->size = 0;
      result->capacity = STRATEGY_INITIAL_CAPACITY;
    }

  return result;
}

size_t strategy_count_locations(const strategy *s)
{
  if (s != NULL)
    {
      return s->num_locations;
    }
  else
    {
      return 0;
    }
}

int strategy_count_units(const strategy *s)
{
  if (s != NULL)
    {
      return s->num_units;
    }
  else
    {
      return 0;
    }
}

bool strategy_add_distribution(strategy *s, const int *dist, double w)
{
  if (s == NULL || w <= 0.0)
    {
      return false;
    }

  if (s->size == 1)
    {
      // TO DO -- mixed strategy should never be full
      return false;
    }

  // make a copy of the given distribution
  int *copy = malloc(sizeof(int) * s->num_locations);
  if (copy == NULL)
    {
      return false;
    }

  // compute the total number of units in the distribution
  int sum = 0;
  for (size_t i = 0; i < s->num_locations; i++)
    {
      sum += dist[i];
      copy[i] = dist[i];
    }

  // make sure the total is crrect
  if (sum != s->num_units)
    {
      free(copy);
      return false;
    }

  // save the copy of the distribution
  s->size = 1;
  s->distro = copy;
  s->weight = w;

  return true;
}
      
double strategy_expected_wins(const strategy *s1, const strategy *s2, const double *values)
{
  if (s1 == NULL || s2 == NULL || values == NULL
      || s1->num_locations != s2->num_locations
      || s1->num_units != s2->num_units)
    {
      return 0.0;
    }

  if (s1->size == 0 && s2->size == 0)
    {
      return 0.5;
    }
  else if (s1->size == 0)
    {
      return 0.0;
    }
  else if (s2->size == 0)
    {
      return 1.0;
    }
  
  return blotto_play_game(s1->num_locations, s1->distro, s2->distro, values);
}

strategy *strategy_copy(const strategy *s)
{
  if (s == NULL)
    {
      return NULL;
    }

  strategy *result = strategy_create(s->num_locations, s->num_units);
  if (result == NULL)
    {
      return NULL;
    }

  // TO DO -- this should perform a deep copy
  *result = *s;
  
  return result;
}

strategy **strategy_crossover(const strategy *s1, const strategy *s2)
{
  if (s1 == NULL || s2 == NULL
      || s1->num_locations != s2->num_locations
      || s1->num_units != s2->num_units)
    {
      return NULL;
    }

  // make array to hold two offspring
  strategy **offspring = malloc(sizeof(strategy *) * 2);
  if (offspring == NULL)
    {
      return NULL;
    }

  // 1st offspring is a copy of 1st parent
  offspring[0] = strategy_copy(s1);
  if (offspring[0] == NULL)
    {
      free(offspring);
      return NULL;
    }

  // 2nd offspring is a copy of 2nd parent
  offspring[1] = strategy_copy(s2);
  if (offspring[1] == NULL)
    {
      strategy_destroy(offspring[0]);
      free(offspring);
      return NULL;
    }

  return offspring;
}

void strategy_print(FILE *out, const strategy *s)
{
  if (out == NULL || s == NULL)
    {
      return;
    }

  arrays_print(out, s->num_locations, s->distro);
  fprintf(out, " %.3f\n", s->weight);
}

void strategy_destroy(strategy *s)
{
  if (s != NULL)
    {
      free(s->distro);
      free(s);
    }
}

