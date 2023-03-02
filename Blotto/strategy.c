#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "strategy.h"
#include "blotto.h"
#include "arrays.h"


typedef struct _distribution
{
  int *dist;
  double w;
} distribution;

struct _strategy
{
  size_t num_locations;
  int num_units;
  distribution *distro;
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
      result->distro = calloc(STRATEGY_INITIAL_CAPACITY, sizeof(distribution));
      result->weight = 0;
      if(result->distro == NULL)
      {
        free(result);
        return NULL;
      }
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

  for(int i = 0; i < s->size; i++)
  {
    if (arrays_compare(s->num_locations, s->distro[i].dist, dist) == 0)
    {
      s->distro[i].w += w;
      s->weight += w;
      return true;
    }
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

  if(s->size == s->capacity)
  {
    distribution *temp = realloc(s->distro, 2 * sizeof(distribution) * s->capacity);
    if(temp == NULL)
    {
      return false;
    }
    else
    {
      s->distro = temp;
      s->capacity = 2 * s->capacity;
    }
  }

  // save the copy of the distribution
  if(s->size == 0)
  {
    s->size++;
    s->distro[s->size - 1].dist = copy;
    s->distro[s->size - 1].w = w;
    s->weight = s->weight + w;
    return true;
  }
  else
  {
    for(int i = 0; i < s->size; i++)
    {
      if(i == 0 && arrays_compare(s->num_locations, copy, s->distro[i].dist) < 0)
      {
        for(int j = s->size; j > i; j--)
        {
          s->distro[j].dist = s->distro[j - 1].dist;
          s->distro[j].w = s->distro[j - 1].w;
        }
        s->distro[i].dist = copy;
        s->distro[i].w = w;
        s->weight += w;
        s->size++;
        return true;
      }
      else if(arrays_compare(s->num_locations, copy, s->distro[i].dist) < 0 && arrays_compare(s->num_locations, copy, s->distro[i - 1].dist) > 0)
      {
        for(int j = s->size; j > i; j--)
        {
          s->distro[j].dist = s->distro[j - 1].dist;
          s->distro[j].w = s->distro[j - 1].w;
        }
        s->distro[i].dist = copy;
        s->distro[i].w = w;
        s->weight += w;
        s->size++;
        return true;
      }
      else
      {
        s->size++;
        s->distro[s->size - 1].dist = copy;
        s->distro[s->size - 1].w = w;
        s->weight = s->weight + w;
        return true;
      }
    }
  }
  // s->size++;
  // s->distro[s->size - 1].dist = copy;
  // s->distro[s->size - 1].w = w;
  // s->weight = s->weight + w;
  // s->size = 1;
  // s->distro = copy;
  // s->weight = w;
  return false;
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

  double result = 0;
  for(int i = 0; i < s1->size; i++)
  {
    for(int j = 0; j < s2->size; j++)
    {
      result += (s2->distro[j].w/s2->weight) * (s1->distro[i].w/s1->weight) * blotto_play_game(s1->num_locations, s1->distro[i].dist, s2->distro[j].dist, values);
    }
  }
  
  return result;
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

  for(int i = 0; i < s->size; i++)
  {
    strategy_add_distribution(result, s->distro[i].dist, s->distro[i].w);
  }
  
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

  // for(int i = 0; i < offspring[0]->size; i++)
  // {
  //   for(int j = i + 1; j < offspring[0]->size; j++)
  //   {
  //     if(arrays_compare(offspring[0]->num_locations, offspring[0]->distro[i].dist, offspring[0]->distro[j].dist) > 0)
  //     {
  //       int *temp = offspring[0]->distro[j].dist;
  //       offspring[0]->distro[j].dist = offspring[0]->distro[i].dist;
  //       offspring[0]->distro[i].dist = temp;

  //       double tempw = offspring[0]->distro[j].w;
  //       offspring[0]->distro[j].w = offspring[0]->distro[i].w;
  //       offspring[0]->distro[i].w = tempw;
  //     }
  //   }
  // }

  // for(int i = 0; i < offspring[1]->size; i++)
  // {
  //   for(int j = i + 1; j < offspring[1]->size; j++)
  //   {
  //     if(arrays_compare(offspring[1]->num_locations, offspring[1]->distro[i].dist, offspring[1]->distro[j].dist) > 0)
  //     {
  //       int *temp = offspring[1]->distro[j].dist;
  //       offspring[1]->distro[j].dist = offspring[1]->distro[i].dist;
  //       offspring[1]->distro[i].dist = temp;

  //       double tempw = offspring[1]->distro[j].w;
  //       offspring[1]->distro[j].w = offspring[1]->distro[i].w;
  //       offspring[1]->distro[i].w = tempw;
  //     }
  //   }
  // }

  if(offspring[1]->size < offspring[0]->size)
  {
    for(int i = 1; i < offspring[1]->size; i++)
    {
      if(i % 2 != 0)
      {
        int *copy = offspring[1]->distro[i].dist;
        offspring[1]->distro[i].dist = offspring[0]->distro[i].dist;
        offspring[0]->distro[i].dist = copy;

        double copyw = offspring[1]->distro[i].w;
        offspring[1]->distro[i].w = offspring[0]->distro[i].w;
        offspring[0]->distro[i].w = copyw;

      }
    }
  }
  else
  {
    for(int i = 1; i < offspring[0]->size; i++)
    {
      if(i % 2 != 0)
      {
        int *copy = offspring[0]->distro[i].dist;
        offspring[0]->distro[i].dist = offspring[1]->distro[i].dist;
        offspring[1]->distro[i].dist = copy;

        double copyw = offspring[0]->distro[i].w;
        offspring[0]->distro[i].w = offspring[1]->distro[i].w;
        offspring[1]->distro[i].w = copyw;
      }
    }
  }

  return offspring;
}

void strategy_print(FILE *out, const strategy *s)
{
  if (out == NULL || s == NULL)
    {
      return;
    }

  // for(int i = 0; i < s->size; i++)
  // {
  //   for(int j = i + 1; j < s->size; j++)
  //   {
  //     if(arrays_compare(s->num_locations, s->distro[i].dist, s->distro[j].dist) > 0)
  //     {
  //       int *temp = s->distro[j].dist;
  //       s->distro[j].dist = s->distro[i].dist;
  //       s->distro[i].dist = temp;

  //       double tempw = s->distro[j].w;
  //       s->distro[j].w = s->distro[i].w;
  //       s->distro[i].w = tempw;
  //     }
  //   }
  // }

  for(int i = 0; i < s->size; i++)
  {
    arrays_print(out, s->num_locations, s->distro[i].dist);
    fprintf(out, " %.3f\n", s->distro[i].w);
  }
}

void strategy_destroy(strategy *s)
{
  if (s != NULL)
    {
      for(int i = 0; i < s->size; i++)
      {
        free(s->distro[i].dist);
      }
      free(s->distro);
      free(s);
    }
}

