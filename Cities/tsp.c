#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#include "location.h"
#include "cities.h"

typedef double distance_t;

#define ERROR (stderr)

void print_usage(const char *command);
int read_input(FILE *in, int n, char names[][4], location *locs);

void route_nearest(int n, int tour[], distance_t dist[][n]);
void route_insert(int n, int tour[], distance_t dist[][n]);
void normalize(int n, int tour[], char [][4]);
void swap(int arr[], int i, int j);

double calculate_total(int n, int tour[], distance_t dist[][n]);

int main(int argc, char **argv)
{
    initialize_city_database();

    if (argc < 3)
    {
        print_usage(argv[0]);
        return 1;
    }

    FILE* infile = fopen(argv[2], "r");
    if(!infile)
    {
        fprintf(ERROR, "%s: couldn't open %s\n", argv[0], argv[1]);
        return 1;
    }

    int num_locations;
    fscanf(infile, "%d", &num_locations);
    if(num_locations < 2)
    {
        fprintf(ERROR, "%s: too few cities\n", argv[0]);
        fclose(infile);
        return 1;
    }

    char cities[num_locations][4];
    location locs[num_locations];

    if (read_input(infile, num_locations, cities, locs) < num_locations)
    {
        fprintf(ERROR, "%s: error in input file\n", argv[0]);
        fclose(infile);
        return 1;
    }
    fclose(infile);

    distance_t table[num_locations][num_locations];
    for(int i = 0; i < num_locations; i++)
    {
        for (int j = 0; j < num_locations; j++)
        {
            table[i][j] = location_distance(locs + i, locs + j);
        }
    }

    int tour[num_locations];
    for (int i = 0; i < num_locations; i++)
    {
        tour[i] = i;
    }


    if (strcmp(argv[1], "-given") == 0)
    {
    }
    else if (strcmp(argv[1], "-nearest") == 0)
    {
        route_nearest(num_locations, tour, table);
    }
    else if (strcmp(argv[1], "-insert") == 0)
    {
        route_insert(num_locations, tour, table);
        normalize(num_locations, tour, cities);
    }
    else
    {
        print_usage(argv[0]);
        return 1;
    }

    double total = calculate_total(num_locations, tour, table);
    printf("%8s:%10.2f",argv[1], total);
    for(int i = 0; i < num_locations; i++)
    {
        printf(" %s", cities[tour[i]]);
    }
    printf(" %s\n", cities[0]);

    return 0;
}


void print_usage(const char *command)
{
    fprintf(stderr, "USAGE: %s [-given | -nearest | -insert] input-file\n", command);
}

int read_input(FILE *in, int n, char cities[][4], location *locs)
{
    for (int i = 0; i < n; i++)
    {
        if (fscanf(in, "%4s", cities[i]) != 1)
        {
            return i;
        }
        cities[i][3] = '\0';

        find_city(cities[i], locs + i);
    }

    return n;
}

void route_nearest(int n, int tour[], distance_t dist[][n])
{
    for(int next = 1; next < n - 1; next++)
    {
        double closest_dist = dist[tour[next - 1]][tour[next]];
        int closest = next;
        for (int cand = next + 1; cand < n; cand++)
        {
            if (dist[tour[next - 1]][tour[cand]] < closest_dist)
            {
                closest = cand;
                closest_dist = dist[tour[next - 1]][tour[cand]];
            }
        }

        int temp = tour[next];
        tour[next] = tour[closest];
        tour[closest] = temp;
    }
}

void route_insert(int n, int tour[], distance_t dist[][n])
{
    for (int i = 0; i < n; i++)
    {
        tour[i] = i;
    }

    double min = INFINITY;
    int closest_from, closest_to;
    for (int from = 0; from < n; from++)
    {
        for (int to = from + 1; to < n; to++)
        {
            if (dist[from][to] < min)
            {
                min = dist[from][to];
                closest_from = from;
                closest_to = to;
            }
        }
    }

    double total = dist[closest_from][closest_to] * 2;
    swap(tour, 0, closest_from);
    swap(tour, 1, closest_to);

    for (int connected = 2; connected < n; connected++)
    {
        min = INFINITY;
        int next;
        for (int city = connected; city < n; city++)
        {
            for (int to = 0; to < connected; to++)
            {
                if (dist[tour[city]][tour[to]] < min)
                {
                    next = city;
                    min = dist[tour[city]][tour[to]];
                }
            }
        }
    

        min = INFINITY;
        int best_insert_point;
        for (int insert = 1; insert <= connected; insert++)
        {
            double subtotal = total - dist[tour[insert % connected]][tour[insert - 1]];
            double newtotal = subtotal + dist[tour[insert - 1]][tour[next]] + dist[tour[next]][tour[insert % connected]];
            if (newtotal < min)
            {
                min = newtotal;
                best_insert_point = insert;
            }
        }

        swap(tour, connected, next);
        for (int j = connected; j > best_insert_point; j--)
        {
            swap(tour, j, j - 1);
        }
        total = min;
    }
}

double calculate_total(int n, int tour[], distance_t dist[][n])
{
    double total = 0.0;
    for (int i = 1; i < n; i++)
    {
        total += dist[tour[i - 1]][tour[i]];
    }
    return total + dist[tour[n-1]][tour[0]];
}

void swap(int arr[], int i, int j)
{
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void normalize(int n, int tour[], char cities[][4])
{
    int temp[n];

    int zero_loc = 0;
    while (zero_loc < n && tour[zero_loc] != 0)
    {
        zero_loc++;
    }

    for (int i = 0; i < n; i++)
    {
        temp[i] = tour[(zero_loc + i) % n];
    }

    for (int i = 0; i < n; i++)
    {
        tour[i] = temp[i];
    }

    if (strcmp(cities[tour[1]], cities[tour[n - 1]]) > 0)
    {
        for (int i = 1; n - i > i; i++)
        {
            swap(tour, i, n - i);
        }
    }
}