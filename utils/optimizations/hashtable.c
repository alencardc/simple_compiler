#include <stdlib.h>
#include <string.h>
#include "hashtable.h"


HashTable* ht_init() {
  HashTable* ht = (HashTable*) malloc(sizeof(HashTable));
  if (ht == NULL)
    return NULL;

  HashEntry** table = (HashEntry**) malloc(sizeof(HashEntry*) * HT_SIZE);

  if(table == NULL)
    return NULL;

  int i;
  for(i = 0; i < HT_SIZE; i++)
    table[i] = NULL;

  ht->size = 0;
  ht->table = table;

  return ht;
}


HashEntry* ht_create_entry(const char* key, void* value) {
  HashEntry* entry = (HashEntry*) malloc(sizeof(HashEntry));

  if(entry == NULL || key == NULL)
    return NULL;

  entry->key = strdup(key);
  entry->value = value;

  return entry;
}

int ht_hash(const char *key) {
  unsigned int value = 5381;

  for(int i = 0; i < strlen(key); i++)
    value = ((value << 5) + value) + (int)key[i];

  return value % HT_SIZE;
}

int ht_insert(HashEntry* entry, HashTable* ht) {
  if(entry == NULL || ht == NULL)
      return -1;

  if(entry->key == NULL)
      return -1;

  int index = ht_hash(entry->key);

  //Hash collision
  if (ht->table[index] != NULL) {
    if (ht->table[index]->next == NULL) {
      ht->table[index]->next = entry;
    } else {
      HashEntry* curr_entry = ht->table[index]->next;
      while (curr_entry->next != NULL) {
        curr_entry = curr_entry->next;
      }
      curr_entry->next = entry;
    }
  } else {
    ht->table[index] = entry;
  }

  ht->size += 1;
  return index;
}

HashEntry* ht_get_entry(const char* key, HashTable *ht){
  int index = ht_hash(key);

  if(ht->table[index] == NULL)
    return NULL;

  if(strcmp(key, ht->table[index]->key) == 0)
    return ht->table[index];

  HashEntry* curr_entry;
  if(ht->table[index]->next != NULL){
    curr_entry = ht->table[index]->next;

    if(strcmp(key, curr_entry->key) == 0)
      return curr_entry;

    while(curr_entry->next != NULL){
      curr_entry = curr_entry->next;

      if(strcmp(key,curr_entry->key) == 0)
        return curr_entry;
    }
  }

  return NULL;
}


void ht_free_entry(HashEntry *entry) {
  if(entry == NULL)
      return;

  free(entry->key);
  //free(entry->value);

  ht_free_entry(entry->next);
  
  free(entry);
  return;
}

void ht_free(HashTable *ht) {
  if (ht == NULL)
    return;

  for(int i = 0; i < HT_SIZE; i++){
    ht_free_entry(ht->table[i]);
  }

  free(ht);
}