#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>


#include "cooccur.h"
#include "gmap.h"
#include "string_key.h"

struct cooccurrence_matrix
{
  gmap *elements;
  char **keys;
  size_t count;
  double **frequency;
  int max_length;
};

cooccurrence_matrix *cooccur_create(char *key[], size_t n)
{
  cooccurrence_matrix *result = malloc(sizeof(cooccurrence_matrix));
  if(result != NULL)
  {
    result->elements = gmap_create(duplicate, compare_keys, hash29, free);
    if(result->elements == NULL)
    {
      free(result);
      return NULL;
    }
    result->keys = malloc(sizeof(char *) * n);
    if(result->keys != NULL)
      {
        for(int i = 0; i < n; i++)
        {
          result->keys[i] = malloc(sizeof(char) * (strlen(key[i]) + 1));
          strcpy(result->keys[i], key[i]);
        }
      }

    for(int i = 0; i < n; i++)
    {
      int *ptr = malloc(sizeof(int));
      if(ptr != NULL)
      {
        *ptr = i;
        if(!gmap_put(result->elements, key[i], ptr))
        {
          free(ptr);
          free(result->elements);
          free(result);
          return NULL;
        }
      }
      // free(ptr);
      
      
      
    }
    int max = strlen(key[0]);
    for(int i = 0; i < n; i++)
    {
      if(max <= strlen(key[i]))
      {
        max = strlen(key[i]);
      }
    }
    result->max_length = max;
    result->count = n;
    result->frequency = malloc(sizeof(double *) * n);
    for(size_t i = 0; i < n; i++)
    {
      result->frequency[i] = calloc(n, sizeof(double));
    }
    return result;
  }
  return NULL;
}

void cooccur_update(cooccurrence_matrix *mat, char **context, size_t n)
{
  if(mat == NULL || context == NULL)
  {
    return;
  }
  for(size_t i = 0; i < n; i++)
  {
    for(size_t j = 0; j < n; j++)
    {
      if(gmap_contains_key(mat->elements, context[i]) && gmap_contains_key(mat->elements, context[j]) && gmap_get(mat->elements, context[i]) != NULL && gmap_get(mat->elements, context[j]) != NULL)
      {
        mat->frequency[* (int *) gmap_get(mat->elements, context[i])][* (int *)gmap_get(mat->elements, context[j])]++;
      }
    }
  }
}

char **cooccur_read_context(cooccurrence_matrix *mat, FILE *stream, size_t *n)
{
  if(feof(stream))
  {
    return NULL;
  }
  gmap *check = gmap_create(duplicate, compare_keys, hash29, free);
  char store[mat->max_length + 1];
  char character;
  size_t count = 0;
  size_t count2 = 0;
  char **result = malloc(sizeof(char *) * mat->count);
  while(true)
  {
    character = fgetc(stream);
    if(character != ' ' && !feof(stream) && character != '\n' && count <= mat->max_length)
    {
      store[count] = character;
      count++;
    }
    else
    {
      store[count] = '\0';
      count = 0;
      if(gmap_contains_key(mat->elements, store) && !gmap_contains_key(check, store))
      {
        // int *ptr = malloc(sizeof(int));
        // *ptr = count2;
        gmap_put(check, store, 0);
        result[count2] = malloc(sizeof(char) * (strlen(store) + 1));
        strcpy(result[count2], store);
        count2++;
      }
    }
    if(character == '\n' || feof(stream))
    {
      break;
    }
  }
  *n = count2;
  gmap_destroy(check);
  return result;
}

double *cooccur_get_vector(cooccurrence_matrix *mat, const char *word)
{
  double *result = malloc(sizeof(double) * mat->count);
  for(size_t i = 0; i < mat->count; i++)
  {
    if(gmap_contains_key(mat->elements, word))
      {
        if(mat->frequency[* (int *) gmap_get(mat->elements, word)][* (int *) gmap_get(mat->elements, word)] == 0)
        {
          result[i] = 0;
        }
        else
        {
          result[i] = mat->frequency[* (int *) gmap_get(mat->elements, word)][i] / mat->frequency[* (int *) gmap_get(mat->elements, word)][* (int *) gmap_get(mat->elements, word)];
        }
      }
  }
  return result;
}

void cooccur_destroy(cooccurrence_matrix *mat)
{
  if(mat == NULL)
  {
    return;
  }
  if(mat->frequency == NULL && mat->elements == NULL)
  {
    return;
  }


  for(size_t i = 0; i < mat->count; i++)
  {
    free(mat->frequency[i]);
  }
  free(mat->frequency);
  for(int i = 0; i < mat->count; i++)
  {
    // printf("%d", sizeof(gmap_get(mat->elements, mat->keys[i])));
    free(gmap_get(mat->elements, mat->keys[i]));
    free(mat->keys[i]);
  }
  free(mat->keys);
  gmap_destroy(mat->elements);
  free(mat);
}
