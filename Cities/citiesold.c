#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cities.h"

// int city_count;
// city cities[];

void merge_sort(int n, const city *in, city *out);

void merge(int n1, const city *a1, int n2, const city *a2, city *out);

void initialize_city_database()
{
    city *sorted = malloc(city_count * sizeof(city));

    merge_sort(city_count, cities, sorted);

    memcpy(cities, sorted, city_count * sizeof(city));

    free(sorted);
}

void merge_sort(int n, const city *a, city *out)
{
    city *a1;
    city *a2;

    if(n < 2)
    {
        memcpy(out, a, sizeof(city) * n);
    }
    else
    {
        a1 = malloc(sizeof(city) * (n/2));
        a2 = malloc(sizeof(city) * (n - n/2));

        merge_sort(n/2, a, a1);
        merge_sort(n - n/2, a + n/2, a2);

        merge(n/2, a1, n - n/2, a2, out);

        free(a1);
        free(a2);
    }
    
}

void merge(int n1, const city *a1, int n2, const city *a2, city *out)
{
    int i1 = 0;
    int i2 = 0;
    int iout = 0;

    while(i1 < n1 || i2 < n2)
    {
        if(i2 >= n2 || ((i1 < n1) && (strcmp(a1[i1].name, a2[i2].name) < 0)))
        {
            out[iout] = a1[i1];
            i1++;
        }
        else
        {
            out[iout] = a2[i2];
            i2++;
        }

        iout++;
    }
}

bool find_city(const char *code, location *loc)
{

    int i = city_count/2;
    int x = city_count/2;

    while(i > 1)
    {

        i = i / 2;
        if(strcmp(code, cities[x].name) == 0)
        {
            *loc = cities[x].coord;
            printf("%s", cities[x].name);
            return true;
        }
        if(strcmp(code, cities[x].name) < 0)
        {
            x = x - i;
        }
        else if (strcmp(code, cities[x].name) > 0)
        {
            x = x + i;
        }

    }

    return false;
    
}