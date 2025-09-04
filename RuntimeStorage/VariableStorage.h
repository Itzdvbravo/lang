#pragma once
#include "../TokensAndNode/TokensAndNode.h"


typedef struct Variable {
    char *key;
    ASTNode *node;
    
    struct Variable *next;
} Variable;

typedef struct HashTable {
    int size;
    Variable **array;
} HashTable;

HashTable* createHashTable(int size);
int hash(HashTable *table, char *str);
void insert(HashTable *table, char *key, ASTNode *node);
ASTNode* searchInHashTable(HashTable *table, char *key);
void clearStorage(HashTable *table);
