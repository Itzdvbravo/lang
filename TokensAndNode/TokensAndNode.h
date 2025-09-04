#pragma once
#include "./TokensList.h"
#include "../Stack/Stack.h"
#include <stdbool.h>

typedef enum {
    TYPE_NOT_ACTUAL_VALUE,
    TYPE_NUM,
    TYPE_STRING,
    TYPE_LIST_BODY,
    TYPE_LIST,
    TYPE_BOOL,
} ValueType;

typedef struct Value  {
    ValueType type;
    union {
        float i;
        char *c;
        bool b;
        Stack *list;
    };
} Value;

typedef struct {
    TokenType type;
    Value value;
} Token;

typedef struct ASTNode {
    TokenType type;      
    Value value;         
    struct ASTNode* left;
    struct ASTNode* body;
    struct ASTNode* right;
    struct ASTNode* parent;
    bool isMarked;
} ASTNode;

ASTNode* newNode(TokenType type, Value value, ASTNode *left, ASTNode *right, ASTNode *parent, ASTNode *body);
void freeNodes(ASTNode** node);

// typedef struct {
//     Token *tokens;
//     int count;
//     int MAX_SIZE;
// } TokenList;