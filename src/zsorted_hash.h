#ifndef ZSORTED_HASH_H
#define ZSORTED_HASH_H

#include <stdbool.h>

#include "./zhash.h"

// sorted hash table, built on top of zhash
// keys are strings
// values are void *pointers
// keys are sorted according to insertion order by default

// struct to represent the linked list used to iterate through entries
struct ZSortedEntry {
  char *key;
  void *val;
  struct ZSortedEntry *next;
  struct ZSortedEntry *prev;
};

// struct representing a sorted hash table
struct ZSortedHashTable {
  struct ZHashTable *table;
  struct ZSortedEntry *first;
  struct ZSortedEntry *last;
};

// struct used for iteration through values
enum ZIteratorStatus {
  ZBEFORE_FIRST,
  ZAFTER_LAST,
  ZWITHIN_BOUNDS,
  ZNO_ENTRIES
};
struct ZIterator {
  enum ZIteratorStatus status;
  struct ZSortedEntry *entry;
};

// sorted hash table creation and destruction
struct ZSortedHashTable *zcreate_sorted_hash_table(void);
void zfree_sorted_hash_table(struct ZSortedHashTable *hash_table);

// sorted hash table operations
void zsorted_hash_set(struct ZSortedHashTable *hash_table, char *key, void *val);
void *zsorted_hash_get(struct ZSortedHashTable *hash_table, char *key);
void *zsorted_hash_delete(struct ZSortedHashTable *hash_table, char *key);
bool zsorted_hash_exists(struct ZSortedHashTable *hash_table, char *key);

// iterator creation and destruction
struct ZIterator *zcreate_iterator(struct ZSortedHashTable *hash_table);
void zfree_iterator(struct ZIterator *iterator);

// iteration functions
size_t zsorted_hash_count(struct ZSortedHashTable *hash_table);
bool ziterator_exists(struct ZIterator *iterator);
char *ziterator_get_key(struct ZIterator *iterator);
void *ziterator_get_val(struct ZIterator *iterator);
void ziterator_next(struct ZIterator *iterator);
void ziterator_prev(struct ZIterator *iterator);

#endif
