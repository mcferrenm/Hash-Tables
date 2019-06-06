#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
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

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(capacity * sizeof(HashTable));

  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair *));

  return ht;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */

void set_pair_with_new_key(LinkedPair *current_bucket, LinkedPair *new_pair)
{
  // Traverse to end of linked list, pair->next == NULL
  
  // Check if this is the tail
  if (current_bucket->next == NULL) {
    
    // Set new_pair to next
    current_bucket->next = new_pair;

  // Recursively traverse, check the next LinkedPair until next == NULL
  } else {
    set_pair_with_new_key(current_bucket->next, new_pair);
  }
}

void hash_table_insert(HashTable *ht, char *key, char *value)
{
  // Hash the key to get bucket index
  unsigned int new_index = hash(key, ht->capacity);

  // Grab reference to current bucket
  LinkedPair *current_bucket = ht->storage[new_index];

  // Create new LinkedPair
  LinkedPair *new_pair = create_pair(key, value);

  // Check if bucket is full
  if (current_bucket != NULL) {

    // Check if key is different than existing key
    if (strcmp(current_bucket->key, key) != 0) {

      // Traverse LinkedList and set new pair
      set_pair_with_new_key(current_bucket, new_pair);

    // If key is the same
    } else {

      // Overwrite with warning 
      printf("Overwriting existing value: %s", current_bucket->value);

      // Destroy current pair
      destroy_pair(current_bucket);

      // Set pair to current bucket
      ht->storage[new_index] = new_pair;
    }
  // If bucket is empty
  } else {

    // set new pair
    ht->storage[new_index] = new_pair;
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{

}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  // Hash the key to get bucket index
  unsigned int new_index = hash(key, ht->capacity);

  // Check if there is a LinkedPair at new_index
  if (ht->storage[new_index] != NULL) {

    // Check if LinkedPair key matches
    if (strcmp(ht->storage[new_index]->key, key) == 0) {
      
      // Return value
      return ht->storage[new_index]->value;
    
    } else {
      
      // TODO Check if there is a next LinkedPair
      return NULL;
    }

  } else {

    // Key not found return NULL
    return NULL;
  }
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{

}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht;

  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
