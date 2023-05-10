#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define ZCOUNT_OF(arr) (sizeof(arr) / sizeof(*arr))
#define zfree free

#include "./zhash.h"
#include "./zsorted_hash.h"

static struct ZSortedEntry *zcreate_sorted_entry(char *key, void *val);
static void zfree_sorted_entry(struct ZSortedEntry *entry, bool recursive);
static void *zmalloc(size_t size);

struct ZSortedHashTable *zcreate_sorted_hash_table(void)
{
  struct ZSortedHashTable *hash_table;

  hash_table = zmalloc(sizeof(struct ZSortedHashTable));

  hash_table->table = zcreate_hash_table();
  hash_table->first = NULL;
  hash_table->last = NULL;

  return hash_table;
}

void zfree_sorted_hash_table(struct ZSortedHashTable *hash_table)
{
  zfree_hash_table(hash_table->table);
  zfree_sorted_entry(hash_table->first, true);
  zfree(hash_table);
}

void zsorted_hash_set(struct ZSortedHashTable *hash_table, char *key, void *val)
{
  struct ZSortedEntry *entry;

  if ((entry = (struct ZSortedEntry *) zhash_get(hash_table->table, key))) {
    entry->val = val;

    return;
  }

  entry = zcreate_sorted_entry(key, val);

  if (hash_table->last) {
    entry->prev = hash_table->last;
    hash_table->last->next = entry;
  } else {
    entry->prev = NULL;
    hash_table->first = entry;
  }

  entry->next = NULL;
  hash_table->last = entry;

  zhash_set(hash_table->table, key, (void *) entry);
}

void *zsorted_hash_get(struct ZSortedHashTable *hash_table, char *key)
{
  struct ZSortedEntry *entry;

  entry = (struct ZSortedEntry *) zhash_get(hash_table->table, key);

  return entry ? entry->val : NULL;
}

void *zsorted_hash_delete(struct ZSortedHashTable *hash_table, char *key)
{
  struct ZSortedEntry *entry;
  void *val;

  entry = (struct ZSortedEntry *) zhash_get(hash_table->table, key);

  if (!entry) return NULL;

  val = entry->val;

  if (entry->next) {
    entry->next->prev = entry->prev;
  } else {
    hash_table->last = entry->prev;
  }

  if (entry->prev) {
    entry->prev->next = entry->next;
  } else {
    hash_table->first = entry->next;
  }

  zhash_delete(hash_table->table, key);
  zfree_sorted_entry(entry, false);

  return val;
}

bool zsorted_hash_exists(struct ZSortedHashTable *hash_table, char *key)
{
  return zhash_exists(hash_table->table, key);
}

struct ZIterator *zcreate_iterator(struct ZSortedHashTable *hash_table)
{
  struct ZIterator *iterator;

  iterator = zmalloc(sizeof(struct ZIterator));

  iterator->entry = hash_table->first;

  if (iterator->entry) {
    iterator->status = ZWITHIN_BOUNDS;
  } else {
    iterator->status = ZNO_ENTRIES;
  }

  return iterator;
}

void zfree_iterator(struct ZIterator *iterator)
{
  zfree(iterator);
}

size_t zsorted_hash_count(struct ZSortedHashTable *hash_table)
{
  return hash_table->table->entry_count;
}

bool ziterator_exists(struct ZIterator *iterator)
{
  if (iterator->status != ZWITHIN_BOUNDS) return false;

  return true;
}

char *ziterator_get_key(struct ZIterator *iterator)
{
  if (iterator->status != ZWITHIN_BOUNDS) return NULL;

  return iterator->entry->key;
}

void *ziterator_get_val(struct ZIterator *iterator)
{
  if (iterator->status != ZWITHIN_BOUNDS) return NULL;

  return iterator->entry->val;
}

void ziterator_next(struct ZIterator *iterator)
{
  if (iterator->status == ZBEFORE_FIRST) {
    iterator->status = ZWITHIN_BOUNDS;

    return;
  }

  if (iterator->status == ZWITHIN_BOUNDS) {
    if (iterator->entry->next) {
      iterator->entry = iterator->entry->next;
    } else {
      iterator->status = ZAFTER_LAST;
    }
  }
}

void ziterator_prev(struct ZIterator *iterator)
{
  if (iterator->status == ZAFTER_LAST) {
    iterator->status = ZWITHIN_BOUNDS;

    return;
  }

  if (iterator->status == ZWITHIN_BOUNDS) {
    if (iterator->entry->prev) {
      iterator->entry = iterator->entry->prev;
    } else {
      iterator->status = ZBEFORE_FIRST;
    }
  }
}

static struct ZSortedEntry *zcreate_sorted_entry(char *key, void *val)
{
  struct ZSortedEntry *entry;

  entry = zmalloc(sizeof(struct ZSortedEntry));

  entry->key = key;
  entry->val = val;

  return entry;
}

static void zfree_sorted_entry(struct ZSortedEntry *entry, bool recursive)
{
  struct ZSortedEntry *next;

  if (!recursive) entry->next = NULL;

  while (entry) {
    next = entry->next;

    zfree(entry);

    entry = next;
  }
}

static void *zmalloc(size_t size)
{
  void *ptr;

  ptr = malloc(size);

  if (!ptr) exit(EXIT_FAILURE);

  return ptr;
}
