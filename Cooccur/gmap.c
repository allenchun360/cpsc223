#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "gmap.h"

typedef struct _node
{
  void *key;
  void *value;
  // struct _node *parent;
  struct _node *left;
  struct _node *right;
  size_t height;
} node;

struct gmap
{
  size_t capacity;
  size_t size;
  node **table;
  
  void *(*copy)(const void *);
  int (*compare)(const void *, const void *);
  size_t (*hash)(const void *);
  void (*free)(void *);
};

#define GMAP_INITIAL_CAPACITY 100

node *insert2(gmap *m, node *n, node *new);
void destroy_node(gmap *m, node *n);
void for_helper(node *n, void (*f)(const void *, void *, void *), void *arg);
size_t height(node *n);
node *rightShift(node *n);
node *leftShift(node *n);
node *insert(gmap *m, node *n, void *key, void *value);
node *new(void *key, void *value);
void gmap_distribute(gmap *m, node *n, node **table, size_t capacity);
size_t gmap_compute_index(const void *key, size_t (*hash)(const void *), size_t size);
void gmap_embiggen(gmap *m, size_t n);
void gmap_table_add(gmap *m, node **table, node *n, size_t (*hash)(const void *), int (*compare)(const void *, const void *), size_t capacity);
node *gmap_table_find_key(node **table, const void *key, int (*compare)(const void *, const void *), size_t (*hash)(const void *), size_t capacity);

gmap *gmap_create(void *(*cp)(const void *), int (*comp)(const void *, const void *), size_t (*h)(const void *s), void (*f)(void * k))
{
  gmap *result = malloc(sizeof(gmap));
  if (result != NULL)
    {
      result->size = 0;
      result->copy = cp;
      result->compare = comp;
      result->hash = h;
      result->free = f;
      result->table = malloc(sizeof(node *) * GMAP_INITIAL_CAPACITY);
      result->capacity = (result->table != NULL ? GMAP_INITIAL_CAPACITY : 0);
      for (size_t i = 0; i < result->capacity; i++)
      {
        result->table[i] = NULL;
      }
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
node *gmap_table_find_key(node **table, const void *key, int (*compare)(const void *, const void *), size_t (*hash)(const void *), size_t capacity)
{
  // compute starting location for search from hash function
  size_t i = gmap_compute_index(key, hash, capacity);
  node *curr = table[i];
  // return curr;
  while (curr != NULL && compare(curr->key, key) != 0)
    {
      if(compare(curr->key, key) > 0)
      {
        curr = curr->left;
      }
      else if(compare(curr->key, key) < 0)
      {
        curr = curr->right;
      }
    }
  return curr;
}

bool gmap_put(gmap *m, const void *key, void *value)
{
  if (m == NULL || key == NULL)
    {
      return false;
    }

  node *n = gmap_table_find_key(m->table, key, m->compare, m->hash, m->capacity);
  if (n != NULL)
    {
      // key already present
      n->value = value;
      return false;
    }
  else
    {
      size_t i = gmap_compute_index(key, m->hash, m->capacity);
      // make a copy of the key
      void *copy = m->copy(key);
      // void *copy_value = m->copy(value);
      
      if (copy != NULL)
      {
        // new key, value pair -- check capacity
        if (m->size >= m->capacity)
          {
            // grow
            gmap_embiggen(m, m->capacity * 2);
          }

          m->table[i] = insert(m, m->table[i], copy, value);
          return true;
      }
      else
      {
        free(copy);
        return false;
      }
    }
}

/**
 * Adds the given node containing a key and value into the appropriate chain the in the
 * given hash table.
 *
 * @param table a hash table
 * @param n a node containing a key and value
 * @param hash a hash function for keys
 * @param capacity the size of the hash table
 */
void gmap_table_add(gmap *m, node **table, node *n, size_t (*hash)(const void *), int (*compare)(const void *, const void *), size_t capacity)
{ 
  size_t i = gmap_compute_index(n->key, hash, capacity);
  table[i] = insert(m, table[i], n->key, n->value);
  free(n);
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
        gmap_distribute(m, m->table[i], bigger, bigger_capacity);
      }

      free(m->table);
      m->table = bigger;
      m->capacity = bigger_capacity;
    }
}

void gmap_distribute(gmap *m, node *n, node **table, size_t capacity)
{
  if(n != NULL)
  {
    gmap_distribute(m, n->left, table, capacity);
    gmap_distribute(m, n->right, table, capacity);
    n->left = NULL;
    n->right = NULL;
    n->height = 1;
    size_t i = gmap_compute_index(n->key, m->hash, capacity);
    table[i] = insert2(m, table[i], n);
  }
}

size_t gmap_compute_index(const void *key, size_t (*hash)(const void *), size_t size)
{
  return (hash(key) % size + size) % size;
}

bool gmap_contains_key(const gmap *m, const void *key)
{
  if (m == NULL || key == NULL)
    {
      return false;
    }

  return gmap_table_find_key(m->table, key, m->compare, m->hash, m->capacity) != NULL;
}

void *gmap_get(gmap *m, const void *key)
{
  if (m == NULL || key == NULL)
    {
      return NULL;
    }
  
  node *n = gmap_table_find_key(m->table, key, m->compare, m->hash, m->capacity);
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

  // TO DO: iterate over all chains as in Ex. 7
  for(int i = 0; i < m->capacity; i++)
  {
    for_helper(m->table[i], f, arg);
  }
}  

void for_helper(node *n, void (*f)(const void *, void *, void *), void *arg)
{
  if(n != NULL)
  {
    for_helper(n->left, f, arg);
    for_helper(n->right, f, arg);
    f(n->key, n->value, arg);
  }
}

void gmap_destroy(gmap *m)
{
  if (m == NULL)
    {
      return;
    }

  //gmap_validate(m);
  
  for (size_t i = 0; i < m->capacity; i++)
    {
      destroy_node(m, m->table[i]);
    }

  free(m->table);
  free(m);
}

void destroy_node(gmap *m, node *n)
{
  if(n != NULL)
  {
    destroy_node(m, n->left);
    destroy_node(m, n->right);
    m->free(n->key);
    // printf("debugging: %d\n", n->value);
    // if(n->value != NULL)
    // {
    //   // printf("%d", *(int *) n->value);
    //   m->free(n->value);
    // }
    free(n);

  }
}

node *rightShift(node *n) 
{ 
    node *p = n->left; 
    if(n->left == NULL)
    {
      return NULL;
    }
    node *c = p->right; 

    p->right = n; 
    n->left = c; 

    size_t max;
    if(height(n->left) > height(n->right))
    {
      max = height(n->left);
    }
    else
    {
      max = height(n->right);
    }
    n->height = max + 1; 
    size_t max2;
    if(height(p->left) > height(p->right))
    {
      max2 = height(p->left);
    }
    else
    {
      max2 = height(p->right);
    }
    p->height = max2 + 1; 

    return p; 
} 

node *leftShift(node *n) 
{ 
    node *p = n->right; 
    if(p == NULL)
    {
      return NULL;
    }
    node *c = p->left; 
   
    p->left = n; 
    n->right = c; 

    size_t max;
    if(height(n->left) > height(n->right))
    {
      max = height(n->left);
    }
    else
    {
      max = height(n->right);
    } 
    n->height = max + 1; 
    size_t max2;
    if(height(p->left) > height(p->right))
    {
      max2 = height(p->left);
    }
    else
    {
      max2 = height(p->right);
    }
    p->height = max2 + 1; 
  
    return p; 
} 

node* new(void *key, void* value)
{ 
    node* new_node = malloc(sizeof(node)); 
    new_node->key = key;
    new_node->value = value;
    new_node->left = NULL; 
    new_node->right = NULL; 
    new_node->height = 1;
    return new_node; 
} 


node *insert(gmap *m, node *n, void *key, void *value) 
{ 
    if (n == NULL)
    {
        m->size++;
        return new(key, value);
    }
  
    if (m->compare(n->key, key) > 0)
    {
        n->left = insert(m, n->left, key, value); 
    } 
    else if (m->compare(n->key, key) < 0) 
    {
        n->right = insert(m, n->right, key, value); 
    }
    else
    {
      return n; 
    }
  
    size_t max;
    if(height(n->left) > height(n->right))
    {
      max = height(n->left);
    }
    else
    {
      max = height(n->right);
    }
    n->height = 1 + max; 
  
    int difference = height(n->left) - height(n->right); 
  
    if (difference > 1 && key < n->left->key) 
        return rightShift(n); 
  
    if (difference < -1 && key > n->right->key) 
        return leftShift(n); 
  
    if (difference > 1 && key > n->left->key) 
    { 
        n->left =  leftShift(n->left); 
        return rightShift(n); 
    } 
  
    if (difference < -1 && key < n->right->key) 
    { 
        n->right = rightShift(n->right); 
        return leftShift(n); 
    } 

    return n; 
} 

size_t height(node *n) 
{ 
    if (n == NULL)
    {
      return 0;
    }
    return n->height; 
} 

node *insert2(gmap *m, node *n, node *new) 
{ 
    if (n == NULL)
    {
        return new;
    }
  
    if (m->compare(n->key, new->key) > 0)
    {
        n->left = insert2(m, n->left, new); 
    } 
    else if (m->compare(n->key, new->key) < 0) 
    {
        n->right = insert2(m, n->right, new); 
    }
    else
    {
      return n; 
    }
  
    size_t max;
    if(height(n->left) > height(n->right))
    {
      max = height(n->left);
    }
    else
    {
      max = height(n->right);
    }
    n->height = 1 + max; 
  
    int difference = height(n->left) - height(n->right); 
  
    if (difference > 1 && new->key < n->left->key) 
        return rightShift(n); 
  
    if (difference < -1 && new->key > n->right->key) 
        return leftShift(n); 
  
    if (difference > 1 && new->key > n->left->key) 
    { 
        n->left =  leftShift(n->left); 
        return rightShift(n); 
    } 
  
    if (difference < -1 && new->key < n->right->key) 
    { 
        n->right = rightShift(n->right); 
        return leftShift(n); 
    } 

    return n; 
} 