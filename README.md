# Hash Table

This is a hash table library implemented in C. The keys are strings and the values are void pointers.
The basic hash table operations are supported: `set`, `get`, `exists`, `delete`.

The following hashing algorithm is used:
```c
hash = 0;
while ((ch = *key++)) hash = (17 * hash + ch) % size;
```
(See [http://algs4.cs.princeton.edu/34hash/](http://algs4.cs.princeton.edu/34hash/) for more information about hash functions.)

Collisions are resolved with separate chaining and a singly linked list.
If the hash table is more than 50% full, it will increase the number of slots and rehash.
Likewise, if it's less than 12.5% full, it will decrease the number of slots and rehash.

The possible numbers of slots are all prime numbers, each size is roughly two times the previous size.
The maximum number of slots is `1000000007` so performance may decrease with more than  `1000000007 / 2` entries.

## Example
```c
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
```

## Public Interface
```c
// create hash table
struct ZHashTable *zcreate_hash_table(void);

// free hash table (note that this only frees the table and the entry structs, not the values)
void zfree_hash_table(struct ZHashTable *hash_table);

// set key to val (if there is already a value, overwrite it)
void zhash_set(struct ZHashTable *hash_table, char *key, void *val);

// get the value stored at key (if no value, then return NULL)
void *zhash_get(struct ZHashTable *hash_table, char *key);

// delete the entry stored at key and return the value
void *zhash_delete(struct ZHashTable *hash_table, char *key);

// return 1 if there is a value stored at the key and 0 otherwise
bool zhash_exists(struct ZHashTable *hash_table, char *key);
```

## Running Tests
```bash
cd ./test
./zhash_test.sh
```
Note that the test uses a program called [Valgrind](http://valgrind.org/) to detect memory
leaks. On some systems, like newer versions of OSX, Valgrind will report false positives.

## Notes
Tested on `Arch Linux 3.18.2` with `GCC 5.2.0`

Also tested on `OSX 10.10.5` with `Apple LLVM version 7.0.0`.
It works, but the Valgrind test may not be accurate.
