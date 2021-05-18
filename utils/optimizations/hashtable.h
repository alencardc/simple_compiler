#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#define HT_SIZE 7

typedef struct sHashEntry {
  char* key;
  void* value;
  struct sHashEntry* next;
} HashEntry;

typedef struct sHashTable {
  int size;
  HashEntry** table;
} HashTable;


HashTable* ht_init();


HashEntry* ht_create_entry(const char* key, void* value);

int ht_hash(const char *key);

int ht_insert(HashEntry* entry, HashTable* ht);

HashEntry* ht_get_entry(const char* key, HashTable *ht);


void ht_free_entry(HashEntry *entry);

void ht_free(HashTable *ht);

#endif // __HASHTABLE_H__