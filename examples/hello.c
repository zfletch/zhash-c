#include <stdio.h>
#include "../src/zhash.h"

// gcc -Wall hello.c ../src/zhash.c
// prints "hello world" to stdout
int main ()
{
  struct ZHashTable *hash_table;
  
  hash_table = zcreate_hash_table();
  
  zhash_set(hash_table, "hello", (void *) "world");

  if (zhash_exists(hash_table, "hello")) {
    printf("hello %s\n", (char *) zhash_get(hash_table, "hello"));
  }
  
  if (zhash_exists(hash_table, "goodbye")) {
   printf("goodbye %s\n", (char *) zhash_get(hash_table, "goodbye"));
  }

  zfree_hash_table(hash_table);

  return 0;
}
