#include "common.h"

unsigned int hash(const char *key)
{
  unsigned int hash_value = 0;
  for (int i = 0; key[i] != '\0'; i++)
  {
    hash_value = hash_value * 31 + key[i];
  }
  return hash_value % MAX_SIZE;
}

AssocArray *assocArrayCreate()
{
  AssocArray *a = (AssocArray *)malloc(sizeof(AssocArray));
  if (!a)
    return NULL;

  for (int i = 0; i < MAX_SIZE; i++)
  {
    a->table[i] = NULL;
  }
  a->size = 0;
  return a;
}

void AssocArrayPut(AssocArray *a, const char *key, void *value)
{
  if (!a || !key)
    return;

  unsigned int index = hash(key);
  kv *current = a->table[index];

  while (current)
  {
    if (strcomp(current->key, key) == 0)
    {
      current->value = value;
      return;
    }
    current = current->next;
  }
}

void *AssocArrayGet(AssocArray *a, const char *key)
{
  if (!a || !key)
    return NULL;

  unsigned int index = hash(key);
  kv *current = a->table[index];

  while (current)
  {
    if (strcmp(current->key, key) == 0)
    {
      return current->value;
    }
    current = current->next;
  }
  return NULL;
}

void AssocArrayFree(AssocArray *a)
{
  if (!a)
    return;

  for (int i = 0; i < MAX_SIZE; i++)
  {
    kv *current = a->table[i];
    while (current)
    {
      kv *next = current->next;
      free(current->key);
      free(current);
      current = next;
    }
  }
  free(a);
}
