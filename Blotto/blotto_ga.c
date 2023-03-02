#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strategy.h"
#include "population.h"
#include "ga_helpers.h"
#include "arrays.h"

int main(int argc, char **argv)
{
  if (argc < 2)
    {
      fprintf(stderr, "USAGE: %s values... files... operations...\n", argv[0]);
      return 1;
    }


  int counti = 0;
  int count_battleground = 0;
  while(atoi(argv[1 + counti]) != 0)
  {
    count_battleground++;
    counti++;
  }

  if(count_battleground == 0)
  {
    return 1;
  }

  double battleground[count_battleground];
  for(int i = 0; i < count_battleground; i++)
  {
    battleground[i] = (double) atoi(argv[1 + i]);
  }

  double weight = 0;
  int dist;
  int distribution[count_battleground];

  counti = count_battleground + 1;
  int count_units = 0;

  FILE *infile = fopen(argv[counti], "r");
  if (!infile)
  {
    printf("%s: could not open %s\n", argv[0], argv[1]);
    return 1;
  }
  fscanf(infile, "%lf", &weight);
  for(int i = 0; i < count_battleground; i++)
  {
    fscanf(infile, "%d", &dist);
    distribution[i] = dist;
    count_units = count_units + distribution[i];
  }
  fclose(infile);

  population *p = population_create();
  int count_files = 0;

  for(int j = counti; j < argc && strcmp(argv[j], "-d") != 0 && strcmp(argv[j], "-o") != 0 && strcmp(argv[j], "-x") != 0 && argv[j][0] != '-'; j++)
  {
    if(strcmp(argv[j], "-d") != 0 && strcmp(argv[j], "-o") != 0 && strcmp(argv[j], "-x") != 0)
    {
      // printf("%s\n", argv[j]);
      FILE *infile = fopen(argv[j], "r");
      if (!infile)
      {
        population_destroy(p);
        return 1;
      }

      strategy *s = strategy_create(count_battleground, count_units);
      
      while(fscanf(infile, "%lf", &weight) == 1)
      {
        int i = 0;
        while(i < count_battleground)
        {
          fscanf(infile, "%d", &dist);
          distribution[i] = dist;
          i++;
        }
        strategy_add_distribution(s, distribution, weight);
      }

        population_add_strategy(p, s);
        fclose(infile);
    }
    count_files++;
  }
  int adds = population_size(p);
  int deletes = 0;
  for(int i = count_battleground + count_files; i < argc; i++)
  {
    if (strcmp(argv[i], "-d") == 0)
    {
      if(i == argc - 1)
      {
        population_destroy(p);
        return 1;
      }
      else if(atoi(argv[i + 1]) == 0)
      {
        population_destroy(p);
        return 1;
      }
      deletes += atoi(argv[i + 1]);

      if(deletes > adds || atoi(argv[i + 1]) > adds)
      {
        // fprintf(stderr, "Cannot delete more than population size!");
        population_destroy(p);
        return 1;
      }

      // drop/delete last population
      strategy **return_value = population_remove_last(p, atoi(argv[i + 1]));

      for(int j = 0; j < atoi(argv[i + 1]); j++)
      {
        strategy_destroy(return_value[j]);
      }
      free(return_value);
      // for return_value, you want to destroy strategies and free relevant memory
    }
    else if (strcmp(argv[i], "-o") == 0)
    {
      if(i != argc - 1 && strcmp(argv[i + 1], "-d") != 0 && strcmp(argv[i + 1], "-o") != 0 && strcmp(argv[i + 1], "-x") != 0)
      {
        population_destroy(p);
        return 1;
      }
      population_order(p, battleground);
    }
    else if (strcmp(argv[i], "-x") == 0)
    {
      if(i == argc - 1)
      {
        population_destroy(p);
        return 1;
      }
      else if(atoi(argv[i + 1]) == 0)
      {
        population_destroy(p);
        return 1;
      }

      if(atoi(argv[i + 1]) < 2 || atoi(argv[i + 1]) > population_size(p))
      {
        population_destroy(p);
        return 1;
      }

      for(int j = 1; j <= atoi(argv[i + 1]); j++)
      {
        adds += j;
      }

      for(size_t j = 0; j < atoi(argv[i + 1]); j++)
      {
        for(size_t k = j + 1; k < atoi(argv[i + 1]); k++)
        {
          strategy **cross = strategy_crossover(population_get(p, j), population_get(p, k));
          population_add_strategy(p, cross[0]);
          population_add_strategy(p, cross[1]);

          free(cross);
        }
      }
    }
    // else if(atoi(argv[i]) == 0)
    // {
    //   population_destroy(p);
    //   return 1;
    // }
  }

  int individuals = 0;
  for(int i = 0; i < population_size(p); i++)
  {
    printf("INDIVIDUAL %d\n", individuals);
    individuals++;
    strategy_print(stdout, population_get(p, i));
  }

  population_destroy(p);

  return 0;
}