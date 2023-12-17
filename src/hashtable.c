#include "hashtable.h"

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This creates a new hash table of the specified size and with
 * the given hash function and comparison function.
 */
HashTable *createHashTable(int size, unsigned int (*hashFunction)(void *),
                           int (*equalFunction)(void *, void *)) {
  int i = 0;
  HashTable *newTable = malloc(sizeof(HashTable));
  if (NULL == newTable) {
    fprintf(stderr, "malloc failed \n");
    exit(1);
  }
  newTable->size = size;
  newTable->buckets = malloc(sizeof(struct HashBucketEntry *) * size);
  if (NULL == newTable->buckets) {
    fprintf(stderr, "malloc failed \n");
    exit(1);
  }
  for (i = 0; i < size; i++) {
    newTable->buckets[i] = NULL;
  }
  newTable->hashFunction = hashFunction;
  newTable->equalFunction = equalFunction;
  return newTable;
}

/* Task 1.2 */
void insertData(HashTable *table, void *key, void *data) {
  int size = table->size;
  unsigned int index = table->hashFunction(key) % size;

  HashBucketEntry *newEntry = (HashBucketEntry*)malloc(sizeof(struct HashBucketEntry));
  if(newEntry == NULL){
    fprintf(stderr, "malloc failed \n");
    exit(1);
  }
  newEntry->key = key;
  newEntry->data = data;
  newEntry->next = NULL; 

  if(table->buckets[index] == NULL){
    table->buckets[index] = newEntry;
  }else{
    newEntry->next = table->buckets[index]->next;
    table->buckets[index]->next = newEntry;
  }
}

/* Task 1.3 */
void *findData(HashTable *table, void *key) {
  //safe check
  if(table == NULL){
    return NULL;
  }

  int size = table->size;
  unsigned int index = table->hashFunction(key) % size;
  
  HashBucketEntry *head = table->buckets[index];

  while(head != NULL){
    if(table->equalFunction(head->key, key)){
      return head->data;
    }
    head = head->next;
  }
  return NULL;
}

/* Task 2.1 */
unsigned int stringHash(void *s) {
  unsigned char *str = (unsigned char *)s;
  unsigned int hash = 5381; // Initial hash value
  while (*str != '\0') {
    hash = ((hash << 5) + hash) + *str; // hash * 33 + *str
    str++;
  }
  return hash;
}

/* Task 2.2 */
int stringEquals(void *s1, void *s2) {
  char *newS1 = (char*)s1;
  char *newS2 = (char*)s2;
  if(strlen(newS1) != strlen(newS2)){
    return 0;
  }
  for(int i = 0; i < strlen(newS1); ++i){      
    if(newS1[i] != newS2[i]){
      return 0;
    }
  }
  return 1;
}