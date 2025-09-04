#include "./TokensAndNode.h"
#include <stdlib.h>

ASTNode* newNode(TokenType type, Value value, ASTNode *left, ASTNode *right, ASTNode *parent, ASTNode *body) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value;
    node->left = left;
    node->right = right;
    node->parent = parent;
    node->body = body;
    node->isMarked = false;
    return node;
}

void freeNodeInternals(ASTNode* node) {
    // printf("Freeing node: ");
    // printf(" %d\n", node->type);
    switch (node->type) {
        case TOKEN_CHAR:
		    free(node->value.c);
            break;
		case TOKEN_IDENTIFIER:
			if (node->value.type == TYPE_STRING) {
				free(node->value.c);
			}
			break;
		case TOKEN_VAR_INITIALIZER:
            if (node->value.type == TYPE_STRING) {
                free(node->value.c);
            }
            break;
        default:
			break;
    }
}

void freeNodes(ASTNode** node) {
    if (!*node) return;
    //if ((*node)->isMarked) return;
	freeNodeInternals(*node);
    if ((*node)->left) freeNodes(&((*node)->left));
    if ((*node)->right) freeNodes(&((*node)->right));
    free(*node);
    *node = NULL;
}
