#ifndef ZHASH_H
#define ZHASH_H

#include <stdbool.h>

// hash table
// keys are strings
// values are void *pointers

#define COUNT_OF(arr) (sizeof(arr) / sizeof(*arr))
#define zfree free

// struct representing an entry in the hash table
struct ZHashEntry {
  char *key;
  void *val;
  struct ZHashEntry *next;
};

// struct representing the hash table
// size_index is an index into the hash_sizes array in hash.c
struct ZHashTable {
  size_t size_index;
  size_t entry_count;
  struct ZHashEntry **entries;
};

// hash table creation and destruction
struct ZHashTable *zcreate_hash_table(void);
void zfree_hash_table(struct ZHashTable *hash_table);

// hash operations
void zhash_set(struct ZHashTable *hash_table, char *key, void *val);
void *zhash_get(struct ZHashTable *hash_table, char *key);
void *zhash_delete(struct ZHashTable *hash_table, char *key);
bool zhash_exists(struct ZHashTable *hash_table, char *key);

// hash entry creation and destruction
struct ZHashEntry *zcreate_entry(char *key, void *val);
void zfree_entry(struct ZHashEntry *entry, bool recursive);

// other functions
size_t zgenerate_hash(struct ZHashTable *hash, char *key);
void zhash_rehash(struct ZHashTable *hash_table, size_t size_index);

#endif
