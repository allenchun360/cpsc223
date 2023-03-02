#ifndef __CITIES_H__
#define __CITIES_H__

#include <stdbool.h>

#include "location.h"

typedef struct
{
  char *name;
  location coord;
} city;

extern int city_count;
extern city cities[];

/**
 * Initializes the cities list.  This must be called at least once before
 * any of the functions are used.
 */
void initialize_city_database();

/**
 * Stores the location of the city with the given code in the location
 * pointed to by the pointer argument.  There is no
 * effect and the return value is false if the city is not present.
 *
 * @param code a string, non-NULL
 * @param a pointer to a location
 * @return true if the city was found and the location stored, false otherwise
 */
bool find_city(const char *code, location *loc);

#endif
