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

void set_pair_with_new_key(LinkedPair *stored_pair, LinkedPair *new_pair)
{
  // Traverse to end of linked list, pair->next == NULL
  
  // Check if this is the tail
  if (stored_pair->next == NULL) {
    
    // Set new_pair to next
    stored_pair->next = new_pair;

  // Recursively traverse, check the next LinkedPair until next == NULL
  } else {
    set_pair_with_new_key(stored_pair->next, new_pair);
  }
}

void hash_table_insert(HashTable *ht, char *key, char *value)
{
  // Hash the key to get bucket index
  unsigned int new_index = hash(key, ht->capacity);

  // Grab reference to stored pair
  LinkedPair *stored_pair = ht->storage[new_index];

  // Create new LinkedPair
  LinkedPair *new_pair = create_pair(key, value);

  // Check if bucket is full
  if (stored_pair != NULL) {

    // Check if key is different than existing key
    if (strcmp(stored_pair->key, key) != 0) {

      // Traverse LinkedList and set new pair
      set_pair_with_new_key(stored_pair, new_pair);

    // If key is the same
    } else {

      // Overwrite with warning 
      printf("Overwriting existing value: %s", stored_pair->value);

      // Destroy current pair
      destroy_pair(stored_pair);

      // Set pair to stored pair
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

void remove_pair_with_key(HashTable *ht, int new_index, LinkedPair *previous_pair, LinkedPair *stored_pair, char *key)
{
    // Check if there is a LinkedPair
  if (stored_pair != NULL) {

    // Check if LinkedPair key matches
    if (strcmp(stored_pair->key, key) == 0) {
      
      // Reassign pointers
      if(previous_pair != NULL) {
        previous_pair->next = stored_pair->next;
      }
      
      // Remove Pair / Free Memory 
      destroy_pair(stored_pair);

      // Set removed to null
      ht->storage[new_index] = NULL;
    
    } else {
      // Recursively traverse, and return value
      remove_pair_with_key(ht, new_index, stored_pair, stored_pair->next, key);
    }
    
  } else {
    // Key not found return NULL
    fprintf(stderr, "No value at key: %s", key);
  }
}

void hash_table_remove(HashTable *ht, char *key)
{ 
  // Hash the key to get bucket index
  unsigned int new_index = hash(key, ht->capacity);

  // Grab reference to stored pair
  LinkedPair *stored_pair = ht->storage[new_index];

  // Call recursive helper function
  remove_pair_with_key(ht, new_index, NULL, stored_pair, key);
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */

char *get_value_from_linked_list(LinkedPair *stored_pair, char *key)
{
  // Check if there is a LinkedPair at new_index
  if (stored_pair != NULL) {

    // Check if LinkedPair key matches
    if (strcmp(stored_pair->key, key) == 0) {
      
      // Return value
      return stored_pair->value;
    
    } else {
      // Recursively traverse, and return value
      return get_value_from_linked_list(stored_pair->next, key);
    }

  } else {

    // Key not found return NULL
    return NULL;
  }
}

char *hash_table_retrieve(HashTable *ht, char *key)
{
  // Hash the key to get bucket index
  unsigned int new_index = hash(key, ht->capacity);

  // Grab reference to stored pair
  LinkedPair *stored_pair = ht->storage[new_index];

  // Recursively traverse, and return value
  return get_value_from_linked_list(stored_pair, key);
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
