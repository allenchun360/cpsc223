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

  return 0;
}
