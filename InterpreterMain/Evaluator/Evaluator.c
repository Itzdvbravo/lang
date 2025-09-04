#pragma once

#include "../../TokensAndNode/TokensAndNode.h"
#include <stdlib.h>
#include "../../RuntimeStorage/VariableStorage.h"
#include "../../Frames/Frames.h"

#include "./Maths.c"
#include "./Identifier.c"
#include "./IfElse.c"

Value evalutate(ASTNode *node, VirtualMachine* vm, Frame* currentFrame) {
    Value value;
    value.type = TYPE_NOT_ACTUAL_VALUE;
    //freeing the value on token_assign issue, and on char returning the value itself

    if (node == NULL) {
        printf("Error: Null node trying to be evaluated\n");
        exit(1);
    }
    // printf("Eval: %d |\n ", node->type);

    switch (node->type) {
        case TOKEN_NUM:
            value = node->value;
            break;
        case TOKEN_CHAR:
            value = node->value;
			if (value.type == TYPE_STRING) {
				value.c = (char*)(malloc(sizeof(char) * strlen(value.c)));
				strncpy(value.c, node->value.c, strlen(value.c));
			}
            break;    
        case TOKEN_IDENTIFIER: {
            identifier(node, &value, vm, currentFrame, evalutate);
            break;
        }
        case TOKEN_ADD:
            addition(node, &value, vm, currentFrame, evalutate);
            break;
        case TOKEN_MINUS:
            minus(node, &value, vm, currentFrame, evalutate);
            break;
        case TOKEN_MULTIPLY:
            multiplication(node, &value, vm, currentFrame, evalutate);
            break;
        case TOKEN_DIVIDE:
            division(node, &value, vm, currentFrame, evalutate);
            break;
        case TOKEN_MODULUS:
            modulus(node, &value, vm, currentFrame, evalutate);
            break;    
        case TOKEN_GREATER_THAN:
            greaterOrLessThan(node, &value, vm, currentFrame, evalutate);
            break;
        case TOKEN_LESS_THAN:
            greaterOrLessThan(node, &value, vm, currentFrame, evalutate);
            break;
        case TOKEN_GREATER_THAN_EQUAL:
            greaterOrLessThan(node, &value, vm, currentFrame, evalutate);
            break;
        case TOKEN_LESS_THAN_EQUAL:
            greaterOrLessThan(node, &value, vm, currentFrame, evalutate);
            break;   
        case TOKEN_COMPARISON:
            comparison(node, &value, vm, currentFrame, evalutate);
            break;    
        case TOKEN_IF:
            ifelse(node, &value, vm, currentFrame, evalutate);
            break;     
        case TOKEN_ASSIGN: {
            ASTNode *var = newNode(TOKEN_IDENTIFIER, evalutate(node->right, vm, currentFrame), NULL, NULL, NULL, NULL);
			char* varName = (char *)(malloc(sizeof(char)*30));
			strncpy(varName, node->left->value.c, 30);
            insert(currentFrame->references, varName, var);
            break;
        }
        case TOKEN_FUNCTION_INITIZLIZER: {
            insert(currentFrame->references, node->value.c, node);
            break;
        }
        case TOKEN_RETURN: {
            value = evalutate(node->right, vm, currentFrame);
            break;
        }
        case TOKEN_FUNCTION_PRINT:
            Value val = evalutate(node->right, vm, currentFrame);
            if (val.type == TYPE_NUM) {
                printf("Print: %f\n", val.i);
            } else if (val.type == TYPE_STRING) {
                printf("Print: %s\n", val.c);
            } else if (val.type == TYPE_BOOL) {
                printf("Print: %s\n", val.b ? "True" : "False");
            } else {
                printf("Error: Unknown type for print\n");
            }
            break;    
        case TOKEN_PRIORITY:
            if (node->right) {
                node->right->left = node->left;
                node->left = NULL;
                value = evalutate(node->right, vm, currentFrame);
            } else {
                value = evalutate(node->left, vm, currentFrame);
            }
            break;
        case TOKEN_BRACKET_OPEN:
            printf("Error: Expected closing bracket\n");
            exit(1);
            break;
        default:
            break;
    }
    return value;
}