#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "gmap.h"

typedef struct _node
{
  void *key;
  void *value;
  struct _node *next;
} node;

struct gmap
{
  node **table;
  size_t capacity; // (# of slots, m)  
  size_t size; // (number of key/value pairs, n)
  
  void *(*copy)(const void *);
  int (*compare)(const void *, const void *);
  size_t (*hash)(const void *);
  void (*free)(void *);
};

#define GMAP_INITIAL_CAPACITY 4

size_t gmap_compute_index(const void *key, size_t (*hash)(const void *), size_t size);
void gmap_embiggen(gmap *m, size_t n);
void gmap_add(node **table, node *n, size_t (*hash)(const void *), size_t capacity);
node *gmap_find_key(node **table, const void *key, int (*compare)(const void *, const void *), size_t (*hash)(const void *), size_t capacity);

gmap *gmap_create(void *(*cp)(const void *), int (*comp)(const void *, const void *), size_t (*h)(const void *s), void (*f)(void * k))
{
  gmap *result = malloc(sizeof(gmap));
  if (result != NULL)
    {
      result->table = malloc(sizeof(node *) * GMAP_INITIAL_CAPACITY);
      result->capacity = GMAP_INITIAL_CAPACITY;
      for (size_t i = 0; i < result->capacity; i++)
	{
	  result->table[i] = NULL;
	}
      result->size = 0;
      
      result->copy = cp;
      result->compare = comp;
      result->hash = h;
      result->free = f;
    }
  return result;
}

size_t gmap_size(const gmap *m)
{
  if (m == NULL)
    {
      return 0;
    }
  
  return m->size;
}

/**
 * Returns the node where the given key is located, or NULL if it is not present.
 * where it would go if it is not present.
 * 
 * @param table a table with at least one free slot
 * @param key a string, non-NULL
 * @param capacity the capacity of table
 * @param hash a hash function for strings
 * @return a pointer to the node containing key, or NULL
 */
node *gmap_find_key(node **table, const void *key, int (*compare)(const void *, const void *), size_t (*hash)(const void *), size_t capacity)
{
  size_t h = hash(key) % capacity;
  
  // compute starting location for search from hash function
  node *curr = table[h];
  while (curr != NULL && compare(curr->key, key) != 0)
    {
      curr = curr->next;
    }
  return curr;
}

bool gmap_put(gmap *m, const void *key, void *value)
{
  if (m == NULL || key == NULL)
    {
      return false;
    }

  node *n = gmap_find_key(m->table, key, m->compare, m->hash, m->capacity);
  if (n != NULL)
    {
      // key already present
      n->value = value;
      return false;
    }
  else
    {
      // make a copy of the key
      void *copy = m->copy(key);
      
      if (copy != NULL)
	{
	  if (m->size >= m->capacity)
	    {
	      gmap_embiggen(m, m->capacity * 2);
	    }
	  
	  // add node
	  node *n = malloc(sizeof(node));
	  if (n != NULL)
	    {
	      n->key = copy;
	      n->value = value;
	      gmap_add(m->table, n, m->hash, m->capacity);
	      m->size++;
	      return true;
	    }
	  else
	    {
	      free(copy);
	      return false;
	    }
	}
      else
	{
	  return false;
	}
    }
}

/**
 * Adds the given node containing a key and value into the appropriate chain the in the
 * given hash table.
 *
 * @param head a pointer to the pointer to the head of the list to add to
 * @param n a node containing a key and value
 * @param hash a hash function for keys
 * @param capacity the size of the hash table
 */
void gmap_add(node **table, node *n, size_t (*hash)(const void *), size_t capacity)
{
  size_t h = hash(n->key) % capacity;
  n->next = table[h];
  table[h] = n;
}

void gmap_embiggen(gmap *m, size_t n)
{
  size_t bigger_capacity = n;
  node **bigger = calloc(bigger_capacity, sizeof(node *));
  if (bigger != NULL)
    {
      // would be better to do this without creating new nodes
      for (size_t i = 0; i < m->capacity; i++)
	{
	  node *curr = m->table[i];
	  while (curr != NULL)
	    {
	      node *next = curr->next;
	      gmap_add(bigger, curr, m->hash, bigger_capacity);
	      curr = next;
	    }
	}

      free(m->table);
      m->table = bigger;
      m->capacity = bigger_capacity;
    }
}

size_t gmap_compute_index(const void *key, size_t (*hash)(const void *), size_t size)
{
  return hash(key) % size;
}

bool gmap_contains_key(const gmap *m, const void *key)
{
  if (m == NULL || key == NULL)
    {
      return false;
    }
  // sequential search on linked list in slot h
  
  return gmap_find_key(m->table, key, m->compare, m->hash, m->capacity) != NULL;
}

void *gmap_get(gmap *m, const void *key)
{
  if (m == NULL || key == NULL)
    {
      return NULL;
    }
  
  node *n = gmap_find_key(m->table, key, m->compare, m->hash, m->capacity);
  if (n != NULL)
    {
      return n->value;
    }
  else
    {
      return NULL;
    }
}

void gmap_for_each(gmap *m, void (*f)(const void *, void *, void *), void *arg)
{
  if (m == NULL || f == NULL)
    {
      return;
    }

  // TO DO: iterate over all chains, starting at head of each
  for(int i = 0; i < m->capacity; i++)
  {
    node *curr = m->table[i]; /* was head */;
  while (curr != NULL)
    {
      f(curr->key, curr->value, arg);
      curr = curr->next;
    }
  }
  // node *curr = NULL; /* was head */;
  // while (curr != NULL)
  //   {
  //     f(curr->key, curr->value, arg);
  //     curr = curr->next;
  //   }
}  

void gmap_destroy(gmap *m)
{
  if (m == NULL)
    {
      return;
    }

  for (size_t i = 0; i < m->capacity; i++)
    {
      node *curr = m->table[i];
      while (curr != NULL)
      {
        m->free(curr->key);
        node *next = curr->next;
        free(curr);
        curr = next;
      }
    }
  
  // TO DO: fix memory leak
  free(m->table);
  
  free(m);
}
