/*
 * Include the provided hash table library.
 */
#include "hashtable.h"
#include <stdio.h>
#include <ctype.h>

/*
 * Include the header file.
 */
#include "philphix.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>
#include <stdbool.h>
/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
#ifndef _PHILPHIX_UNITTEST
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 1;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(0x61C, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}
#endif /* _PHILPHIX_UNITTEST */

bool isAlphaAndNum(char ch) {
  bool isUpperCharacter = ch >= 'A' && ch <= 'Z';
  bool isLowerCharacter = ch >= 'a' && ch <= 'z';
  bool isNumber = ch >= '0' && ch <= '9';
  if (isUpperCharacter || isLowerCharacter || isNumber) {
    return true;
  }
  else {
    return false;
  }
}
/* Task 3 */
void readDictionary(char *dictName) {
  FILE *file = fopen(dictName, "r");
  if(file == NULL){
    fprintf(stderr, "open file failed \n");
    exit(61);
  }
  // 1 -> 1  
  const int lineSize = 700000;
  char line[lineSize];

  while (fgets(line, lineSize, file) != NULL) {
    // line format
    // a b\n\0
    // a b\0

    //copy the line to the heap
    char *newLine = (char *)malloc( sizeof(char) *(lineSize));  
    if(newLine == NULL){
      fprintf(stderr, "malloc failed \n");
      exit(1);
    }
    //newLine format
    // a b\n\0
    // a b\0
    strcpy(newLine,line);
    char *key = NULL;
    char *data = NULL;

    
    int i = 0;
    int firstBreakPoint = 0;
    int secondBreakPoint = 0;
    //find the firstKeyIdx
    {
      while (i < lineSize) {
        if (isAlphaAndNum(newLine[i])) {
            key = &(newLine[i]);
            break;
          }
          i++;
        }
        //find the lastKeyIdx, let it to the space;
        while (i < lineSize) {
          if (!isAlphaAndNum(newLine[i])) {
            firstBreakPoint = i;
            break;
          }
          i++;
        }
        //find the firstDataIdx
        while (i < lineSize) {
          if (!isspace(newLine[i])) {
            data = &(newLine[i]);
            break;
          }
          i++;
        }
        //find the lastDataIdx
        while (i < lineSize) {
          if (isspace(newLine[i]) || newLine[i] == '\0') {
            secondBreakPoint = i;
            break;
          }
          i++;
        }
        newLine[firstBreakPoint] = '\0';
        newLine[secondBreakPoint] = '\0';
    }    
    if(key != NULL && data != NULL){
      insertData(dictionary, key, data);
    }
  }  
}


//data 和key的大小不一定是相等的
#define DEBUG 0
/* Task 4 */
void processInput() {
  int lineSize = 7000000;
  char *line =(char*)malloc(sizeof(char) * lineSize);
  char ch;
  line[0] = '@';
  int prevIdx = 0;
  int currIdx = 0;
              
  while((ch = getchar()) != EOF){
    line[++currIdx] = ch;
    if(!isAlphaAndNum(line[currIdx])){
      int keySize = currIdx - prevIdx;
      if(keySize == 1){
        prevIdx = currIdx;
      }else{
        char *key1 = (char*) malloc(sizeof(char) * keySize);
        char *key2 = (char*) malloc(sizeof(char) * keySize);
        char *key3 = (char*) malloc(sizeof(char) * keySize);

        for(int k = 0; k < keySize; ++k){
          key1[k] = line[prevIdx + k + 1];
          key2[k] = line[prevIdx + k + 1];
          key3[k] = line[prevIdx + k + 1];
        }
        key1[keySize - 1] = '\0';
        key2[keySize - 1] = '\0';
        key3[keySize - 1] = '\0';
        for(int k = 1;  k < keySize - 1; ++k){
            key2[k] = tolower(key2[k]);
            key3[k] = tolower(key3[k]);
        }
        key3[0] = tolower(key3[0]);
        if(DEBUG){
          printf("key1 = %s\n", key1);
          printf("key2 = %s\n", key2);
          printf("key3 = %s\n", key3);
          printf("------------\n");
        }
        char *data1 = findData(dictionary, key1);
        char *data2 = findData(dictionary, key2);
        char *data3 = findData(dictionary, key3);
        if(data1 || data2 || data3){
          if(data1){
            currIdx = prevIdx + 1;
            int k = 0;
            while(data1[k] != '\0'){
              line[k + currIdx] = data1[k];
              k++;
            }
            currIdx += k;
            line[currIdx] = ch;  
          }else if(data2){
            currIdx = prevIdx + 1;
            int k = 0;
            while(data2[k] != '\0'){
              line[k + currIdx] = data2[k];
              k++;
            }
            currIdx += k;
            line[currIdx] = ch;
          }else if(data3){
            currIdx = prevIdx + 1;
            int k = 0;
            while(data3[k] != '\0'){
              line[k + currIdx] = data3[k];
              k++;
            }
            currIdx += k;
            line[currIdx] = ch;
          }
        }
        prevIdx = currIdx;
      }
    }
  }


  line[++currIdx] = ch;
  if(!isAlphaAndNum(line[currIdx])){
    int keySize = currIdx - prevIdx;
    if(keySize == 1){
      prevIdx = currIdx;
    }else{
      char *key1 = (char*) malloc(sizeof(char) * keySize);
      char *key2 = (char*) malloc(sizeof(char) * keySize);
      char *key3 = (char*) malloc(sizeof(char) * keySize);

      for(int k = 0; k < keySize; ++k){
        key1[k] = line[prevIdx + k + 1];
        key2[k] = line[prevIdx + k + 1];
        key3[k] = line[prevIdx + k + 1];
      }
      key1[keySize - 1] = '\0';
      key2[keySize - 1] = '\0';
      key3[keySize - 1] = '\0';
      for(int k = 1;  k < keySize - 1; ++k){
          key2[k] = tolower(key2[k]);
          key3[k] = tolower(key3[k]);
      }
      key3[0] = tolower(key3[0]);
      if(DEBUG){
        printf("key1 = %s\n", key1);
        printf("key2 = %s\n", key2);
        printf("key3 = %s\n", key3);
        printf("------------\n");
      }
      char *data1 = findData(dictionary, key1);
      char *data2 = findData(dictionary, key2);
      char *data3 = findData(dictionary, key3);
      if(data1 || data2 || data3){
        if(data1){
          currIdx = prevIdx + 1;
          int k = 0;
          while(data1[k] != '\0'){
            line[k + currIdx] = data1[k];
            k++;
          }
          currIdx += k;
          line[currIdx] = ch;  
        }else if(data2){
          currIdx = prevIdx + 1;
          int k = 0;
          while(data2[k] != '\0'){
            line[k + currIdx] = data2[k];
            k++;
          }
          currIdx += k;
          line[currIdx] = ch;
        }else if(data3){
          currIdx = prevIdx + 1;
          int k = 0;
          while(data3[k] != '\0'){
            line[k + currIdx] = data3[k];
            k++;
          }
          currIdx += k;
          line[currIdx] = ch;
        }
      }
      prevIdx = currIdx;
    }
  }


  if(!DEBUG){
    for(int k = 1; k < currIdx; ++k){
      printf("%c", line[k]);
    }
  }
}
