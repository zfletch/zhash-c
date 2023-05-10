#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "../src/zsorted_hash.h"

// generate strings of random  ASCII characters 64 to 126
// between 5 and 20 characters
static char *random_string()
{
  size_t length, ii;
  char *str;

  length = rand() % 15 + 5;
  str = malloc((length + 1) * sizeof(char));

  for (ii = 0; ii < length; ii++) {
    str[ii] = rand() % 62 + 64;
  }
  str[length] = '\0';

  return str;
}

static void zsorted_hash_set_test()
{
  size_t size, ii;
  char **keys, **vals;
  struct ZSortedHashTable *hash_table;

  size = 100;
  hash_table = zcreate_sorted_hash_table();
  keys = malloc(size * sizeof(char *));
  vals = malloc(size * sizeof(char *));

  for (ii = 0; ii < size; ii++) {
    keys[ii] = random_string();
    vals[ii] = random_string();
    zsorted_hash_set(hash_table, keys[ii], (void *) vals[ii]);
  }

  assert(zsorted_hash_count(hash_table) == size);

  for (ii = 0; ii < size; ii++) {
    assert(strcmp((char *) zsorted_hash_get(hash_table, keys[ii]), vals[ii]) == 0);
  }

  for (ii = 0; ii < size; ii++) {
    free(keys[ii]);
    free(vals[ii]);
  }

  free(keys);
  free(vals);
  zfree_sorted_hash_table(hash_table);
}

static void zsorted_hash_delete_test()
{
  size_t size, ii;
  char **keys, **vals;
  struct ZSortedHashTable *hash_table;

  size = 100;
  hash_table = zcreate_sorted_hash_table();
  keys = malloc(size * sizeof(char *));
  vals = malloc(size * sizeof(char *));

  for (ii = 0; ii < size; ii++) {
    keys[ii] = random_string();
    vals[ii] = random_string();
    zsorted_hash_set(hash_table, keys[ii], (void *) vals[ii]);
  }

  assert(zsorted_hash_count(hash_table) == size);

  for (ii = 0; ii < 7 * size / 8; ii++) {
    zsorted_hash_delete(hash_table, keys[ii]);
  }

  for (ii = 0; ii < size; ii++) {
    if (ii < 7 * size / 8) {
      assert(zsorted_hash_get(hash_table, keys[ii]) == NULL);
    } else {
      assert(strcmp((char *) zsorted_hash_get(hash_table, keys[ii]), vals[ii]) == 0);
    }
  }

  assert(zsorted_hash_count(hash_table) == size - 7 * size / 8);

  for (ii = 0; ii < size; ii++) {
    free(keys[ii]);
    free(vals[ii]);
  }

  free(keys);
  free(vals);
  zfree_sorted_hash_table(hash_table);
}

static void zsorted_hash_exists_test()
{
  struct ZSortedHashTable *hash_table;

  hash_table = zcreate_sorted_hash_table();

  zsorted_hash_set(hash_table, "hello", (void *) "world");
  zsorted_hash_set(hash_table, "nothing", NULL);

  assert(zsorted_hash_exists(hash_table, "hello") == true);
  assert(zsorted_hash_exists(hash_table, "nothing") == true);
  assert(zsorted_hash_get(hash_table, "nothing") == NULL);
  assert(zsorted_hash_exists(hash_table, "nope") == false);

  zfree_sorted_hash_table(hash_table);
}

static void ziterator_test()
{
  size_t size, ii;
  char **keys, **vals;
  struct ZSortedHashTable *hash_table;

  size = 100;
  hash_table = zcreate_sorted_hash_table();
  keys = malloc(size * sizeof(char *));
  vals = malloc(size * sizeof(char *));

  for (ii = 0; ii < size; ii++) {
    keys[ii] = random_string();
    vals[ii] = random_string();
    zsorted_hash_set(hash_table, keys[ii], (void *) vals[ii]);
  }

  zsorted_hash_delete(hash_table, keys[0]);
  zsorted_hash_delete(hash_table, keys[20]);
  zsorted_hash_delete(hash_table, keys[size - 1]);
  zsorted_hash_set(hash_table, keys[90], (void *) vals[90]);

  struct ZIterator *iterator = zcreate_iterator(hash_table);

  for (ii = 1; ii < size - 1; ii++) {
    if (ii != 20) {
      assert(ziterator_exists(iterator) == true);
      assert(strcmp(ziterator_get_key(iterator), keys[ii]) == 0);
      assert(strcmp((char *) ziterator_get_val(iterator), vals[ii]) == 0);

      ziterator_next(iterator);
    }
  }

  assert(ziterator_exists(iterator) == false);
  assert(ziterator_get_key(iterator) == NULL);
  assert(ziterator_get_val(iterator) == NULL);

  ziterator_next(iterator);
  ziterator_prev(iterator);

  for (ii = size - 2; ii > 0; ii--) {
    if (ii != 20) {
      assert(ziterator_exists(iterator) == true);
      assert(strcmp(ziterator_get_key(iterator), keys[ii]) == 0);
      assert(strcmp((char *) ziterator_get_val(iterator), vals[ii]) == 0);

      ziterator_prev(iterator);
    }
  }

  assert(ziterator_exists(iterator) == false);
  assert(ziterator_get_key(iterator) == NULL);
  assert(ziterator_get_val(iterator) == NULL);

  for (ii = 0; ii < size; ii++) {
    free(keys[ii]);
    free(vals[ii]);
  }

  free(keys);
  free(vals);
  zfree_iterator(iterator);
  zfree_sorted_hash_table(hash_table);
}

int main()
{
  zsorted_hash_set_test();
  zsorted_hash_delete_test();
  zsorted_hash_exists_test();
  ziterator_test();

  return 0;
}
