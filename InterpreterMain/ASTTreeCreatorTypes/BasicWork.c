#include "../../TokensAndNode/TokensAndNode.h"
#include <stdbool.h>
#include <stdlib.h>

ASTNode *firstParentOfType(ASTNode *node, int type) {
    ASTNode *firstParentOfType = node;
    while (firstParentOfType) {
        if (firstParentOfType->type == type) return firstParentOfType;
        firstParentOfType = firstParentOfType->parent;
    }
    return firstParentOfType;
}

bool BasicWork(ASTNode **nn, ASTNode **rn, ASTNode **lon, ASTNode **ln, Token currentToken) {
    bool found = true;

    ASTNode *new_node = *nn;
    ASTNode *root_node = *rn;
    ASTNode *last_operator_node = *lon;
    ASTNode *last_node = *ln;

    switch (currentToken.type) {
        case TOKEN_VAR_INITIALIZER:
            if (!root_node) root_node = new_node;
            last_node = new_node;
            last_operator_node = new_node;
            break;
        case TOKEN_IDENTIFIER:
            // new_node->left = root_node;
            if (last_node && (last_node->type == TOKEN_VAR_INITIALIZER || last_node->type == TOKEN_FUNCTION_INITIZLIZER)) {
                last_node->value = new_node->value;
                free(new_node);
            } else if (last_node) {
                last_node->right = new_node;
                last_node = new_node;
            }
            break;
        case TOKEN_ASSIGN:
            if (!last_node || (last_node->type != TOKEN_IDENTIFIER && last_node->type != TOKEN_VAR_INITIALIZER)) {
                printf("Error: Expected identifier before assignment, given %d\n", last_node ? last_node->type : -1);
                exit(1);
            }
            new_node->left = last_node;
            last_node = new_node;
            if (last_operator_node && last_operator_node->type != TOKEN_VAR_INITIALIZER) last_operator_node->right = new_node;
            if (!root_node || root_node->type == TOKEN_VAR_INITIALIZER) root_node = new_node;
            last_operator_node = new_node;
            break;    
        case TOKEN_BRACKET_OPEN:
            // new_node->left = root_node;
            if (last_operator_node) {
                last_operator_node->right = new_node;
                new_node->parent = last_operator_node;
            }
            last_operator_node = new_node;
            last_node = new_node;
            break;        
        case TOKEN_BRACKET_CLOSE:
            if (!last_operator_node) {
                printf("Error: Expected operator before closing bracket");
                exit(1);
            }

            ASTNode *returnedItem = firstParentOfType(last_operator_node, TOKEN_BRACKET_OPEN);
            if (!returnedItem || returnedItem->type != TOKEN_BRACKET_OPEN) {
                printf("Error: Expected opening bracket before closing bracket, got %d", returnedItem->type); //here
                exit(1);
            }
            new_node->type = TOKEN_PRIORITY;
            returnedItem->parent->right = new_node;
            new_node->left = returnedItem->right;
            new_node->parent = returnedItem->parent;
            last_operator_node = new_node;
            last_node = new_node;
            free(returnedItem);
            break;
        case TOKEN_FUNCTION_PRINT:
            if (last_operator_node) {
                last_operator_node->right = new_node;
                new_node->parent = last_operator_node;
            } else {
                root_node = new_node;
            }
            last_operator_node = new_node;
            last_node = new_node;
            break;    
        case TOKEN_NUM:
            // new_node->left = root_node;
            // if (last_node->type == TOKEN_ASSIGN || last_node->type == TOKEN_ADD || last_node->type == TOKEN_MINUS || last_node->type == TOKEN_MULTIPLY || last_node->type == TOKEN_DIVIDE) {
              last_node->right = new_node;
            // }
            last_node = new_node;
            break;
        case TOKEN_CHAR:
            // new_node->left = root_node;
            // if (last_node->type == TOKEN_ASSIGN || last_node->type == TOKEN_ADD || last_node->type == TOKEN_MINUS || last_node->type == TOKEN_MULTIPLY || last_node->type == TOKEN_DIVIDE) {
                last_node->right = new_node;
            // }
            last_node = new_node;
            break;    
        default:
            found = false;
            break;
    }

    *nn = new_node;
    *rn = root_node;
    *lon = last_operator_node;
    *ln = last_node;

    return found;
}