//
// Created by aydarov on 11.05.2019.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


#define SIMPLE_NUMBERS {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211}
#define TWO_BYTE 65536
#define W 2
typedef struct HashElement {
    char *key;
    int value;
    struct HashElement *next;
};

typedef struct {
    struct HashElement *elem;
} HashMap;
typedef struct {
    HashMap *hashMap;
    int sizeOfMap;
    int shift;
} HashMapInf;
typedef struct {
    char *key;
    int value;
} Map;

HashMapInf *fillingHashMap(Map *map, int quantity);

int insertKey(HashMap *hashMap, Map map, int hash);

int getHashElement(HashMap *hashMap, char *key, int quantity, int shift);

int isHashElementOf(HashMap *hashMap, char *key, int quantity, int shift);

HashMap *newHashMap(int size);

HashMapInf *newHashMapInf(int size);

HashMap *initHashMap(Map *map, int quantity, int simpleNumber, int shift, int *collision);

int getHash(char *key, int quantity, int shift);


void printHashMapInf(HashMapInf *hashMapInf, char *name);

