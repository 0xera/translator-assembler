
#include "hash.h"


HashMap *newHashMap(int size) {
    HashMap *map = (HashMap *) calloc(size, sizeof(HashMap));
    for (int i = 0; i < size; i++) {
        map[i].elem = NULL;
    }
    return map;

}

HashMapInf *newHashMapInf(int size) {
    HashMapInf *mapInf = (HashMapInf *) malloc(sizeof(HashMapInf));
    HashMap *map = (HashMap *) calloc(size, sizeof(HashMap));
    for (int i = 0; i < size; i++) {
        map[i].elem = NULL;
    }
    mapInf->hashMap = (HashMap *) malloc(sizeof(HashMap));
    mapInf->hashMap = map;
    mapInf->sizeOfMap = 0;
    mapInf->shift = 0;
    return mapInf;

}

int getHash(char *key, int quantity, int shift) {

    unsigned short word = 0;
    unsigned short temp = 0;
    for (int i = 0; i < strlen(key); i += 2) {
        temp = key[i];
        temp <<= 8;
        if (i != strlen(key) - 1) temp |= key[i + 1];
        temp = (temp << shift) | (temp >> shift);
        word = (word + temp) % TWO_BYTE;
    }
    return word % quantity;
}

HashMapInf *fillingHashMap(Map *map, int quantity) {
    int simpleNumbers[1000] = SIMPLE_NUMBERS;
    int collisions = 0;
    for (int k = 0; k <= 1000; k++) {

        if (simpleNumbers[k] < quantity) continue;
        for (int shift = 1; shift <= 8; shift++) {
            HashMap *hashMap = initHashMap(map, quantity, simpleNumbers[k], shift, &collisions);
            if (collisions <= 2) {
                HashMapInf *mapInf = (HashMapInf *) malloc(sizeof(HashMapInf));
                mapInf->hashMap = (HashMap *) malloc(sizeof(HashMap));
                mapInf->hashMap = hashMap;
                mapInf->sizeOfMap = simpleNumbers[k];
                mapInf->shift = shift;
                return mapInf;
            }
        }
    }
}

HashMap *initHashMap(Map *map, int quantity, int simpleNumber, int shift, int *collision) {

    HashMap *hashMap = newHashMap(simpleNumber);
    int countOfColl = 0;

    for (int i = 0; i < quantity; i++) {
        int hash = getHash(map[i].key, simpleNumber, shift);
        countOfColl += insertKey(hashMap, map[i], hash);
    }
    *collision = countOfColl;
    return hashMap;

}

int insertKey(HashMap *hashMap, Map map, int hash) {
    int countOfColl = 0;

    struct HashElement *tempHashElement = hashMap[hash].elem;
    struct HashElement *hashElement = (struct HashElement *) malloc(sizeof(struct HashElement));

    hashElement->key = (char *) calloc(strlen(map.key), sizeof(char));
    strcpy(hashElement->key, map.key);
    hashElement->value = map.value;
    hashElement->next = NULL;

    if (!tempHashElement) {
        hashMap[hash].elem = hashElement;
    } else {
        countOfColl++;
        while (tempHashElement->next) tempHashElement = tempHashElement->next;
        tempHashElement->next = hashElement;
    }

    return countOfColl;

}

void printHashMapInf(HashMapInf *hashMapInf, char *name) {
    int margin;
    printf("\n%s hash hashMap:\n", name);
    for (int i = 0; i < hashMapInf->sizeOfMap; i++) {
        struct HashElement *hashElement = hashMapInf->hashMap[i].elem;
        if (hashElement) {
            printf("%d)", i);
            margin = 0;
            do {
                for (int j = 0; j < margin; j++) {
                    printf("     ");
                }
                printf(" Key: %s, Value: %d\n\n", hashElement->key, hashElement->value);
                hashElement = hashElement->next;
                margin++;
            } while (hashElement);
        } else {
            printf("%d)\n\n", i);
        }
    }
}

int getHashElement(HashMap *map, char *key, int quantity, int shift) {
    int hash = getHash(key, quantity, shift);

    struct HashElement *hashElement = map[hash].elem;

    if (hashElement) {
        if (!strcmp(hashElement->key, key)) {
            return hashElement->value;
        } else {
            while (hashElement) {
                if (!strcmp(hashElement->key, key)) {
                    return hashElement->value;
                }
                hashElement = hashElement->next;
            }
        }
    }
    return 0;
}

int isHashElementOf(HashMap *map, char *key, int quantity, int shift) {
    int hash = getHash(key, quantity, shift);

    struct HashElement *hashElement = map[hash].elem;

    if (hashElement) {
        if (!strcmp(hashElement->key, key)) {
            return 1;
        } else {
            while (hashElement) {
                if (!strcmp(hashElement->key, key)) {
                    return 1;
                }
                hashElement = hashElement->next;
            }
        }
    }
    return 0;
}

//int main() {
//
//    printHashMap(fillingHashMap(fillingMap(R, QUANTITY_OF_REGISTERS, 1), QUANTITY_OF_REGISTERS), "test");
//    return 0;
//}
