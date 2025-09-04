#include <string.h>
#include <stdlib.h>
#include "../Stack/Stack.h"
#include "./VariableStorage.h"

HashTable* createHashTable(int size) {
    HashTable *table = (HashTable*)malloc(sizeof(HashTable));
    table->size = size;
    table->array = (Variable**)malloc(sizeof(Variable*) * size);
    for (int i = 0; i < size; i++) {
        table->array[i] = NULL;
    }
    return table;
}

int hash(HashTable *table, char *str){
    unsigned long h = 0;
    int c;

    while ((c = *str++))
        h = c + (h << 6) + (h << 16) - h;
    return h % table->size;
}

void insert(HashTable *table, char *key, ASTNode *node) {
    int index = hash(table, key);
    Variable *var = (malloc(sizeof(Variable)));
    node->isMarked = true;
    var->key = key;
    var->node = node;
    var->next = table->array[index];
    table->array[index] = var;
}

ASTNode* searchInHashTable(HashTable *table, char *key) {
    int index = hash(table, key);
    Variable *var = table->array[index];
    while (var) {
        if (strcmp(var->key, key) == 0) return var->node;
        var = var->next;
    }
	return NULL;
}

void clearStorage(HashTable *table) {
    for (int i = 0; i < table->size; i++) {
		Variable* var = table->array[i];
        while (var != NULL) {
			Variable* temp = var->next;
            free(var->key);
			if (var->node) {
				freeNodes(&(var->node));
			}
			free(var);
			var = temp;
        }
    }
	free(table->array);
}

// void clearStorage() {
//     for (int i = 0; i < TABLE_SIZE; i++) {
//         Variable* var = hashTable[i];
//         while (var != NULL) {
//             Variable* temp = var->next;
//             // if (var->value.type == TYPE_STRING) {
//             //     free(var->value.c);
//             // } else if (var->value.type == TYPE_LIST) {
//             //     // if (var->type == TOKEN_FUNCTION_INITIZLIZER) {
//             //     //     int i = var->value.list->top;
//             //     //     while (i >= 0) {
//             //     //         ASTNode* node = (ASTNode*)stack_pop(var->value.list);
//             //     //         freeNodes(&node);
//             //     //         i--;
//             //     //     }
//             //     // }
//             //     stack_free(var->value.list);
//             // }
//             free(var);
//             var = temp;
//         }
//     }
// }