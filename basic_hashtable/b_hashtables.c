#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Basic hash table key/value pair
 ****/
typedef struct Pair {
  char *key;
  char *value;
} Pair;

/****
  Basic hash table
 ****/
typedef struct BasicHashTable {
  int capacity;
  Pair **storage;
} BasicHashTable;

/****
  Create a key/value pair to be stored in the hash table.
 ****/
Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
  pair->key = strdup(key);
  pair->value = strdup(value);

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(Pair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/****
  djb2 hash function

  Do not modify this!
 ****/
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char * u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}


/****
  Fill this in.

  All values in storage should be initialized to NULL
  (hint: look up `calloc`)
 ****/
BasicHashTable *create_hash_table(int capacity)
{
  BasicHashTable *ht = malloc(sizeof(BasicHashTable));

  ht->storage = calloc(capacity, sizeof(Pair *));
  ht->capacity = capacity;

  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  // Hash the key to get bucket index
  int index = hash(key, ht->capacity);

  // Create pair
  Pair *pair = create_pair(key, value);

  // Check if there's already a pair at index
  if (ht->storage[index] != NULL) {

    // Check if it is the key arg
    if (strcmp(ht->storage[index]->key, key) != 0) {
      printf("Overwriting existing value: %s", ht->storage[index]->value);
    }
    // Destroy current pair
    destroy_pair(ht->storage[index]);
  }

  // Set pair to index
  ht->storage[index] = pair;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  // Hash the key to get bucket index
  int index = hash(key, ht->capacity);

  // Check if there's a pair at index
  if (ht->storage[index] != NULL && strcmp(ht->storage[index]->key, key) == 0) {
    destroy_pair(ht->storage[index]);
    ht->storage[index] = NULL;
  } else {
    fprintf(stderr, "No value at key: %s", key);
    return NULL;
  }
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  // Hash the key to get bucket index
  int new_index = hash(key, ht->capacity);

  // Check if index is not NULL, key matches and return value
  if (ht->storage[new_index] != NULL && strcmp(ht->storage[index]->key, key) == 0) {
    return ht->storage[new_index]->value;
    } else {
      fprintf(stderr, "No value at key: %s", key);
      return NULL;
    }
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  // Free all pairs (including keys and values)
  for (int i = 0; i < ht->capacity; i++) {
    if (ht->storage[i] != NULL) {
      destroy_pair(ht->storage[i])
    }
  }
  // Free hash table storage
  free(ht->storage);

  // Free hash table
  free(ht);
}


#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);

  hash_table_insert(ht, "line", "Here today...\n");

  printf("%s", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");

  if (hash_table_retrieve(ht, "line") == NULL) {
    printf("...gone tomorrow. (success)\n");
  } else {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}
#endif
