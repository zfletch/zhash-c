#include <stdio.h>
#include "../src/zsorted_hash.h"

// gcc -Wall -Wextra sorted_hello.c ../src/zhash.c ../src/zsorted_hash.c
// prints "hello world" in English and French to stdout
int main ()
{
  struct ZSortedHashTable *hash_table;
  struct ZIterator *iterator;

  hash_table = zcreate_sorted_hash_table();

  zsorted_hash_set(hash_table, "hello", (void *) "world");
  zsorted_hash_set(hash_table, "bonjour", (void *) "le monde");

  for (iterator = zcreate_iterator(hash_table);
      ziterator_exists(iterator); ziterator_next(iterator)) {

    printf("%s %s\n", (char *) ziterator_get_key(iterator),
        (char *) ziterator_get_val(iterator));
  }

  zfree_iterator(iterator);
  zfree_sorted_hash_table(hash_table);

  return 0;
}
