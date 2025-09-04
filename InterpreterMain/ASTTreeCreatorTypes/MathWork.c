#include "../../TokensAndNode/TokensAndNode.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//maths needs to be fixed
ASTNode* lastOfType(ASTNode *node, int type, int type2, int type3) {
    ASTNode *lastOfType = node;

    while (lastOfType->parent->type == type || lastOfType->parent->type == type2 || lastOfType->parent->type == type3) {
        lastOfType = lastOfType->parent;
    }
    return lastOfType;
}

ASTNode* topMostOperator(ASTNode *node) {
    ASTNode *lastOfType = node;
    while (lastOfType->parent->type == TOKEN_MULTIPLY || lastOfType->parent->type == TOKEN_DIVIDE || 
        lastOfType->parent->type == TOKEN_MINUS || lastOfType->parent->type == TOKEN_ADD || 
        lastOfType->parent->type == TOKEN_MODULUS || lastOfType->parent->type == TOKEN_GREATER_THAN ||
        lastOfType->parent->type == TOKEN_LESS_THAN
    ) {
        lastOfType = lastOfType->parent;
    }
    return lastOfType;
}

void moveUp(ASTNode *replacing, ASTNode *replacer) {
    replacing->parent->right = replacer;
    replacer->left = replacing;
    replacer->parent = replacing->parent;
    replacing->parent = replacer;
}

void addNode(ASTNode **root_node, ASTNode **last_node, ASTNode **last_operator_node, ASTNode *new_node) {
    if ((*last_node)->type != TOKEN_NUM && (*last_node)->type != TOKEN_CHAR && (*last_node)->type != TOKEN_IDENTIFIER && (*last_node)->type != TOKEN_PRIORITY) {
        printf("Error: Expected number or identifier before addition, given %d\n", (*last_node)->type);
        exit(1);
    }
    if (last_operator_node && (*last_operator_node)->type > TOKEN_ADD && (*last_operator_node)->type <= TOKEN_MODULUS) {
        ASTNode *lastType = lastOfType(*last_operator_node, TOKEN_MULTIPLY, TOKEN_DIVIDE, TOKEN_MINUS);
        moveUp(lastType, new_node);
    } else {
        new_node->left = *last_node;
        if (last_operator_node) {
            (*last_operator_node)->right = new_node;
            new_node->parent = *last_operator_node;
        }

    }
    *last_node = new_node;
    *last_operator_node = new_node; 
}

void subtractNode(ASTNode **root_node, ASTNode **last_node, ASTNode **last_operator_node, ASTNode *new_node, Token currentToken) {
    // if ((*last_node)->type != TOKEN_NUM && (*last_node)->type != TOKEN_CHAR && (*last_node)->type != TOKEN_IDENTIFIER && (*last_node)->type != TOKEN_PRIORITY) {
    //     printf("Error: Expected number or identifier before addition, given %d\n", (*last_node)->type);
    //     exit(1);
    // }
    if (last_operator_node && (*last_operator_node)->type >= TOKEN_MINUS && (*last_operator_node)->type <= TOKEN_MODULUS) {
        ASTNode *lastType = lastOfType(*last_operator_node, TOKEN_MULTIPLY, TOKEN_DIVIDE, TOKEN_MINUS);
        moveUp(lastType, new_node);
    } else {
        if ((*last_node)->type != TOKEN_NUM && (*last_node)->type != TOKEN_CHAR && (*last_node)->type != TOKEN_IDENTIFIER && (*last_node)->type != TOKEN_PRIORITY) {
            Value value;
            value.type = TYPE_NUM;
            value.i = 0;
            *last_node = newNode(TOKEN_NUM, value, NULL, NULL, NULL, NULL);
        }
        new_node->left = *last_node;
        if (last_operator_node) {
            (*last_operator_node)->right = new_node;
            new_node->parent = *last_operator_node;
        }

    }
    *last_node = new_node;
    *last_operator_node = new_node; 
}

void multiplyDivideModulusNode(ASTNode **root_node, ASTNode **last_node, ASTNode **last_operator_node, ASTNode *new_node) {
    if ((*last_node)->type != TOKEN_NUM && (*last_node)->type != TOKEN_IDENTIFIER && (*last_node)->type != TOKEN_PRIORITY) {
        printf("Error: Expected number or identifier before multiplication/division, given %d\n", (*last_node)->type);
        exit(1);
    }
    if (*last_operator_node && (*last_operator_node)->type >= TOKEN_MULTIPLY && (*last_operator_node)->type <= TOKEN_MODULUS) {
        ASTNode *lastType = lastOfType(*last_operator_node, TOKEN_DIVIDE, TOKEN_MULTIPLY, TOKEN_MODULUS);
        moveUp(lastType, new_node);
    } else {
        new_node->left = *last_node;
        if (*last_operator_node) {
            new_node->parent = *last_operator_node;
            (*last_operator_node)->right = new_node;
        }
    }
    
    *last_node = new_node;
    *last_operator_node = new_node; 
}

void greaterOrLess(ASTNode **root_node, ASTNode **last_node, ASTNode **last_operator_node, ASTNode *new_node) {
    if ((*last_node)->type != TOKEN_NUM && (*last_node)->type != TOKEN_IDENTIFIER && (*last_node)->type != TOKEN_PRIORITY) {
        printf("Error: Expected number or identifier before comparison, given %d\n", (*last_node)->type);
        exit(1);
    }
    if (*last_operator_node && (*last_operator_node)->type >= TOKEN_ADD && (*last_operator_node)->type <= TOKEN_LESS_THAN_EQUAL) {
        ASTNode *lastType = topMostOperator(*last_operator_node);
        moveUp(lastType, new_node);
    } else {
        new_node->left = *last_node;
        if (*last_operator_node) {
            new_node->parent = *last_operator_node;
            (*last_operator_node)->right = new_node;
        }
    }
    
    *last_node = new_node;
    *last_operator_node = new_node; 
}

void comparisonWork(ASTNode **root_node, ASTNode **last_node, ASTNode **last_operator_node, ASTNode *new_node) {
    if ((*last_node)->type != TOKEN_NUM && (*last_node)->type != TOKEN_IDENTIFIER && (*last_node)->type != TOKEN_PRIORITY) {
        printf("Error: Expected number or identifier before comparison, given %d\n", (*last_node)->type);
        exit(1);
    }
    if (*last_operator_node && ((*last_operator_node)->type >= TOKEN_ADD && (*last_operator_node)->type <= TOKEN_LESS_THAN_EQUAL)) {
        ASTNode *lastType = topMostOperator(*last_operator_node);
        moveUp(lastType, new_node);
    } else {
        new_node->left = *last_node;
        if (*last_operator_node) {
            new_node->parent = *last_operator_node;
            (*last_operator_node)->right = new_node;
        }
    }
    
    *last_node = new_node;
    *last_operator_node = new_node; 
}

bool MathWork(ASTNode **nn, ASTNode **rn, ASTNode **lon, ASTNode **ln, Token currentToken) {
    ASTNode *new_node = *nn;
    ASTNode *root_node = *rn;
    ASTNode *last_operator_node = *lon;
    ASTNode *last_node = *ln;

    bool found = true;
    switch (currentToken.type) {
        case TOKEN_ADD:
                addNode(&root_node, &last_node, &last_operator_node, new_node);
                break;      
        case TOKEN_MINUS:
                subtractNode(&root_node, &last_node, &last_operator_node, new_node, currentToken);
                break;
        case TOKEN_MULTIPLY:
                multiplyDivideModulusNode(&root_node, &last_node, &last_operator_node, new_node);
                break;    
        case TOKEN_DIVIDE:
                multiplyDivideModulusNode(&root_node, &last_node, &last_operator_node, new_node);
                break; 
        case TOKEN_MODULUS:
                multiplyDivideModulusNode(&root_node, &last_node, &last_operator_node, new_node);
                break;     
                case TOKEN_GREATER_THAN:
                greaterOrLess(&root_node, &last_node, &last_operator_node, new_node);
                break;
        case TOKEN_COMPARISON:
                comparisonWork(&root_node, &last_node, &last_operator_node, new_node);
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