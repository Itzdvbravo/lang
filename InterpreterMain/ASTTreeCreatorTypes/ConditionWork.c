#include "../../TokensAndNode/TokensAndNode.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Tokenizer.c"

Stack* getBody(ASTNode *node, char **theStr, ASTNode *(*tokenize_line)(char**, TokenType)) {
    bool wasClosed = false;
    Stack *stack = createStack(10);
    // printf("ConditionWork Open\n");
    while (**theStr != '\0') {
        ASTNode* ast = tokenize_line(theStr, TOKEN_SCOPE_BRACKET_CLOSE);
        // printf("ConditionWork: %d\n", ast->type);
        if(!ast) {*theStr += 1; continue;}
        if (ast->type == TOKEN_SCOPE_BRACKET_CLOSE) {
            // printf("ConditionWork Close\n");
            wasClosed = true;
            break;
        }
        stack_push(stack, (void *)ast);
        // Value result = evalutate(ast);
    }
    
    if (!wasClosed) {
        printf("ConditionWork: Expected closing bracket\n");
        exit(1);
    }    
    return stack;  
}

bool ConditionWork(ASTNode **nn, ASTNode **rn, ASTNode **lon, ASTNode **ln, Token currentToken, ASTNode *(*tokenize_line)(char**, TokenType), char** theStr) {
    ASTNode *new_node = *nn;
    ASTNode *root_node = *rn;
    ASTNode *last_operator_node = *lon;
    ASTNode *last_node = *ln;

    bool found = true;
    switch (currentToken.type) {
        case TOKEN_IF: {
            if (root_node) {
                printf("Error: Expected closing statement.\n");
                exit(1);
            }
            root_node = new_node;
            last_operator_node = new_node;
            last_node = new_node;
            break;
        }
        case TOKEN_COMPARISON: {
            if (!last_node) {
                printf("Left hand operand expected.\n");
                exit(1);
            }
            last_node->right = new_node;
            last_operator_node = new_node;
            last_node = new_node;
            break;
        }
        case TOKEN_ELSE: {
            if (!root_node || root_node->type != TOKEN_IF) {
                printf("Error: Expected if statement.\n");
                exit(1);
            }
            last_operator_node = new_node;
            root_node->left = new_node;
            break;
        }
        case TOKEN_SCOPE_BRACKET_OPEN: {
            if (root_node->type == TOKEN_IF && !root_node->body) {
                new_node->value.type = TYPE_LIST_BODY;
                new_node->value.list = getBody(root_node, theStr, tokenize_line);
                root_node->body = new_node;

                char *str = *theStr;
                char *temp = str;
                Token token = getToken(&str, new_node);
                str = temp;
                *theStr = str;
                if (token.type != TOKEN_ELSE) {
                    Value val = {0};
                    new_node = newNode(TOKEN_EOL, val, NULL, NULL, NULL, NULL);
                }
            } else if (last_operator_node && last_operator_node->type == TOKEN_ELSE) {
                if (!root_node || root_node->type != TOKEN_IF) {
                    printf("Error: Expected if statement.\n");
                    exit(1);
                }

                last_operator_node->value.type = TYPE_LIST_BODY;
                last_operator_node->value.list = getBody(last_operator_node, theStr, tokenize_line);

                last_operator_node = new_node;
                Value val = {0};
                new_node = newNode(TOKEN_EOL, val, NULL, NULL, NULL, NULL);
                break;
            } else {return false;}
            break;
        }
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