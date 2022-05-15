// Gabriel Couto
// COP 3502 Spring 2020
// ga350657

// including the libraries needed
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "Hashtastic.h"
#include <math.h>

unsigned int hash(int key);

// finding the next prime number
int nextPrime(int n)
{
  int i, root, keepGoing = 1;

  if (n % 2 == 0)
    ++n;

  while (keepGoing)
  {
    keepGoing = 0;

    root = sqrt(n);

    for (i = 3; i <= root; i++)
    {
      if (n % i == 0)
      {
        n += 2;
        keepGoing = 1;

        break;
      }
    }
  }

  return n;
}

// making and initializing the elements in the hash tab;e
HashTable *makeHashTable(int capacity)
{
  int i;
  HashTable *Hash;

  Hash = malloc(sizeof(HashTable));

  if (Hash == NULL)
    return NULL;

  Hash->capacity = capacity;

  if (capacity <= 0)
    Hash->capacity = DEFAULT_CAPACITY;

  if (capacity <= 0)
    Hash->array = malloc(sizeof(int) * DEFAULT_CAPACITY);

  else
    Hash->array = malloc(sizeof(int) * capacity);

  if (Hash->array == NULL)
  {
    free(Hash);
    return NULL;
  }

  for(i = 0; i < Hash->capacity; i++)
  {
    Hash->array[i] = UNUSED;
  }

  Hash->size = 0;
  Hash->hashFunction = NULL;
  Hash->probing = LINEAR;

  Hash->stats.opCount = 0;
  Hash->stats.collisions = 0;

  return Hash;
}

// free the hash table
HashTable *destroyHashTable(HashTable *h)
{
  free(h->array);
  free(h);

  return NULL;
}

// set the probing type
int setProbingMechanism(HashTable *h, ProbingType probing)
{
  if (h == NULL)
    return HASH_ERR;

  if (probing == QUADRATIC)
  {
    h->probing = QUADRATIC;
    return HASH_OK;
  }

  if (probing == LINEAR)
  {
    h->probing = LINEAR;
    return HASH_OK;
  }

  else
    return HASH_ERR;
}

// using a poniter set the hash function
int setHashFunction(HashTable *h, unsigned int (*hashFunction)(int))
{
  if (h == NULL)
    return HASH_ERR;

  h->hashFunction = hashFunction;

  return HASH_OK;
}

// checks if the table is half
int isAtLeastHalfEmpty(HashTable *h)
{
  if (h == NULL)
    return 0;

  if (h->capacity <= 0)
    return 0;

  if ((h->size * 2) <= h->capacity)
    return 1;

  else
    return 0;
}

// if the table is more then half full
int expandHashTable(HashTable *h)
{
  int n = 0, i = 0, j = 0 ,cap, index, org;
  int *arr, *temp;

  if (h == NULL)
    return HASH_ERR;

  // if probing is quadrtic make an apporiate array
  if (h->probing == QUADRATIC)
  {
    n = nextPrime((h->capacity * 2 + 1));
    arr = malloc(sizeof(int) * n);

    if (arr == NULL)
      return HASH_ERR;
  }

  // if probing is linear make an apporiate array
  if (h->probing == LINEAR)
  {
    n = h->capacity * 2 + 1;
    arr = malloc(sizeof(int) * n);

    if (arr == NULL)
      return HASH_ERR;
  }

  // intialize new array
  for (i = 0; i < n; i++)
    arr[i] = UNUSED;

  h->size = 0;

  temp = h->array;
  h->array = arr;

  cap = h->capacity;
  h->capacity = n;

  // insert the old nums in the new array
  for (i = 0; i < cap; i++)
  {
    j = 0;
    if (temp[i] != UNUSED)
    {
      index = org = (h->hashFunction(temp[i]) % h->capacity);

      // if probing is LINEAR make an apporiate array
      if (h->probing == LINEAR)
      {
        while (h->array[index] != UNUSED)
        {
          h->stats.collisions++;
          j++;
          index = (org + j) % h->capacity;

          if (h->array[index] == DIRTY)
            break;
        }
      }

      // if probing is quadrtic make an apporiate array
      if (h->probing == QUADRATIC)
      {
        while (h->array[index] != UNUSED)
        {
          if (h->array[index] == DIRTY)
            break;

          h->stats.collisions++;
          j++;
          index = (org + j * j) % h->capacity;
        }
      }

      // value
      h->array[index] = temp[i];
      // increment size
      h->size++;
    }
  }
  free(temp);
  return HASH_OK;
}

// insert value
int insert(HashTable *h, int key)
{
  int index, i = 0, j = 0, collide = 0, org;

  if (h == NULL)
    return HASH_ERR;

  if (h->hashFunction == NULL)
    return HASH_ERR;

  // checks and expands the hash table is more then half full
  if (isAtLeastHalfEmpty(h) == 0)
  {
    expandHashTable(h);
  }

  // finds the first index
  index = org = (h->hashFunction(key) % h->capacity);

  // if probing is linear probe for an empty space
  if (h->probing == LINEAR)
  {
    while (h->array[index] != UNUSED)
    {
      h->stats.collisions++;
      i++;
      index = (org + i) % h->capacity;

      // break is the val is dirty
      if (h->array[index] == DIRTY)
        break;
    }
  }

  // if probing is quadrtic probe for an empty space
  if (h->probing == QUADRATIC)
  {
    while (h->array[index] != UNUSED)
    {
      if (h->array[index] == DIRTY)
        break;

      h->stats.collisions++;
      i++;
      index = (org + i * i) % h->capacity;
    }
  }

  // places the key
  h->array[index] = key;
  // incremnt opcount  and size
  h->stats.opCount++;
  h->size++;

  return HASH_OK;
}

// search for a value
int search(HashTable *h, int key)
{
  int index, i = 0, j = 0, org;

  if (h == NULL)
  {
    return -1;
  }

  if (h->hashFunction == NULL)
  {
    return -1;
  }

  // incremnt opcount and find initial index
  h->stats.opCount++;
  index = org = (h->hashFunction(key) % h->capacity);

  // if probing is linear probe for key
  if (h->probing == LINEAR)
  {
    for (i = 0; i < h->capacity; i++)
    {
      if (h->array[index] != key)
      {
        if (h->array[index] == UNUSED)
          return -1;

        h->stats.collisions++;
        j++;
        index = (org + (j)) % h->capacity;
      }

      else
        return index;
    }
    return -1;
  }

  // if probing is quadrtic porbe for key
  else if (h->probing == QUADRATIC)
  {
    for (i = 0; i < h->capacity; i++)
    {
      if (h->array[index] != key)
      {
        if (h->array[index] == UNUSED)
          return -1;

        h->stats.collisions++;
        j++;
        index = (org + (j * j)) % h->capacity;
      }

      else
        return index;
    }
    return -1;
  }
  else
    return -1;
}


// uses the search function and deletes the key at t he array given by search
int delete(HashTable *h, int key)
{
  int index;

  if (h == NULL)
  {
    h->stats.opCount++;
    return -1;
  }

  if (h->hashFunction == NULL)
  {
    h->stats.opCount++;
    return -1;
  }

  index = search(h, key);

  if (index == -1)
    return -1;

  h->array[index] = DIRTY;
  h->size--;

  return index;
}

double difficultyRating(void)
{
  return 4;
}

double hoursSpent(void)
{
  return 9;
}
