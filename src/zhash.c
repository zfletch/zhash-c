#include <string.h>
#include <stdlib.h>
#include "./zhash.h"

// helper functions
static size_t next_size_index(size_t size_index);
static size_t previous_size_index(size_t size_index);
static struct ZHashTable *zcreate_hash_table_with_size(size_t size_index);
static void *zmalloc(size_t size);
static void *zcalloc(size_t num, size_t size);

// possible sizes for hash table; must be prime numbers
static const size_t hash_sizes[] = {
  53, 101, 211, 503, 1553, 3407, 6803, 12503, 25013, 50261,
  104729, 250007, 500009, 1000003, 2000029, 4000037, 10000019,
  25000009, 50000047, 104395301, 217645177, 512927357, 1000000007
};

struct ZHashTable *zcreate_hash_table(void)
{
  return zcreate_hash_table_with_size(0);
}

static struct ZHashTable *zcreate_hash_table_with_size(size_t size_index)
{
  struct ZHashTable *hash_table;

  hash_table = zmalloc(sizeof(struct ZHashTable));

  hash_table->size_index = size_index;
  hash_table->entry_count = 0;
  hash_table->entries = zcalloc(hash_sizes[size_index], sizeof(void *));

  return hash_table;
}

void zfree_hash_table(struct ZHashTable *hash_table)
{
  size_t size, ii;

  size = hash_sizes[hash_table->size_index];

  for (ii = 0; ii < size; ii++) {
    struct ZHashEntry *entry;

    if ((entry = hash_table->entries[ii])) zfree_entry(entry, true);
  }

  zfree(hash_table->entries);
  zfree(hash_table);
}

void zhash_set(struct ZHashTable *hash_table, char *key, void *val)
{
  size_t size, hash;
  struct ZHashEntry *entry;

  hash = zgenerate_hash(hash_table, key);
  entry = hash_table->entries[hash];

  while (entry) {
    if (strcmp(key, entry->key) == 0) {
      entry->val = val;
      return;
    }
    entry = entry->next;
  }

  entry = zcreate_entry(key, val);

  entry->next = hash_table->entries[hash];
  hash_table->entries[hash] = entry;
  hash_table->entry_count++;

  size = hash_sizes[hash_table->size_index];

  if (hash_table->entry_count > size / 2) {
    zhash_rehash(hash_table, next_size_index(hash_table->size_index));
  }
}

void *zhash_get(struct ZHashTable *hash_table, char *key)
{
  size_t hash;
  struct ZHashEntry *entry;

  hash = zgenerate_hash(hash_table, key);
  entry = hash_table->entries[hash];

  while (entry && strcmp(key, entry->key) != 0) entry = entry->next;

  return entry ? entry->val : NULL;
}

void *zhash_delete(struct ZHashTable *hash_table, char *key)
{
  size_t size, hash;
  struct ZHashEntry *entry;
  void *val;

  hash = zgenerate_hash(hash_table, key);
  entry = hash_table->entries[hash];

  if (entry && strcmp(key, entry->key) == 0) {
    hash_table->entries[hash] = entry->next;
  } else {
    while (entry) {
      if (entry->next && strcmp(key, entry->next->key) == 0) {
        struct ZHashEntry *deleted_entry;

        deleted_entry = entry->next;
        entry->next = entry->next->next;
        entry = deleted_entry;
        break;
      }
      entry = entry->next;
    }
  }

  if (!entry) return NULL;

  val = entry->val;
  zfree_entry(entry, false);
  hash_table->entry_count--;

  size = hash_sizes[hash_table->size_index];

  if (hash_table->entry_count < size / 8) {
    zhash_rehash(hash_table, previous_size_index(hash_table->size_index));
  }

  return val;
}

bool zhash_exists(struct ZHashTable *hash_table, char *key)
{
  size_t hash;
  struct ZHashEntry *entry;

  hash = zgenerate_hash(hash_table, key);
  entry = hash_table->entries[hash];

  while (entry && strcmp(key, entry->key) != 0) entry = entry->next;

  return entry ? true : false;
}

struct ZHashEntry *zcreate_entry(char *key, void *val)
{
  struct ZHashEntry *entry;
  char *key_cpy;

  key_cpy = zmalloc((strlen(key) + 1) * sizeof(char));
  entry = zmalloc(sizeof(struct ZHashEntry));

  strcpy(key_cpy, key);
  entry->key = key_cpy;
  entry->val = val;

  return entry;
}

void zfree_entry(struct ZHashEntry *entry, bool recursive)
{
  if (recursive && entry->next) zfree_entry(entry->next, recursive);

  zfree(entry->key);
  zfree(entry);
}

size_t zgenerate_hash(struct ZHashTable *hash_table, char *key)
{
  size_t size, hash;
  char ch;

  size = hash_sizes[hash_table->size_index];
  hash = 0;

  while ((ch = *key++)) hash = (17 * hash + ch) % size;

  return hash;
}

void zhash_rehash(struct ZHashTable *hash_table, size_t size_index)
{
  size_t hash, size, ii;
  struct ZHashEntry **entries;

  if (size_index == hash_table->size_index) return;

  size = hash_sizes[hash_table->size_index];
  entries = hash_table->entries;

  hash_table->size_index = size_index;
  hash_table->entries = zcalloc(hash_sizes[size_index], sizeof(void *));

  for (ii = 0; ii < size; ii++) {
    struct ZHashEntry *entry;

    entry = entries[ii];
    while (entry) {
      struct ZHashEntry *next_entry;

      hash = zgenerate_hash(hash_table, entry->key);
      next_entry = entry->next;
      entry->next = hash_table->entries[hash];
      hash_table->entries[hash] = entry;

      entry = next_entry;
    }
  }

  zfree(entries);
}

static size_t next_size_index(size_t size_index)
{
  if (size_index == COUNT_OF(hash_sizes)) return size_index;

  return size_index + 1;
}

static size_t previous_size_index(size_t size_index)
{
  if (size_index == 0) return size_index;

  return size_index - 1;
}

static void *zmalloc(size_t size)
{
  void *ptr;

  ptr = malloc(size);

  if (!ptr) exit(EXIT_FAILURE);

  return ptr;
}

static void *zcalloc(size_t num, size_t size)
{
  void *ptr;

  ptr = calloc(num, size);

  if (!ptr) exit(EXIT_FAILURE);

  return ptr;
}
