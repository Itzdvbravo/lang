#include "../../TokensAndNode/TokensAndNode.h"
#include <stdbool.h>
// #include "../Evaluator.c"
#include "../../Stack/Stack.h"
#include "../Tokenizer.c"

Stack *getParamters(ASTNode *root_node, char **theStr, int check) {
    Stack *stack = createStack(5);
    while (**theStr != '\0') {
        Token token = getToken(theStr, root_node);
        // printf("FuncWork: %d\n", token.type);
        if ((check && (token.type != TOKEN_IDENTIFIER && token.type != TOKEN_COMMA && token.type != TOKEN_COMMA && token.type != TOKEN_BRACKET_CLOSE)) || (!check && (token.type == TOKEN_EOF || token.type == TOKEN_EOL))) {
            printf("Error: Expected identifier or comma, given %d\n", token.type);
            exit(1);
        }
        if (token.type == TOKEN_BRACKET_CLOSE) {break;}
        if (token.type == TOKEN_COMMA) {continue;}
        ASTNode *new_node = newNode(token.type, token.value, NULL, NULL, NULL, NULL);
        stack_push(stack, (void*)new_node);
    }
    return stack;
}

bool FuncWork(ASTNode **nn, ASTNode **rn, ASTNode **lon, ASTNode **ln, Token currentToken, ASTNode *(*tokenize_line)(char**, TokenType), char** theStr) {
    ASTNode *new_node = *nn;
    ASTNode *root_node = *rn;
    ASTNode *last_operator_node = *lon;
    ASTNode *last_node = *ln;

    bool found = true;

    switch (currentToken.type) {
        case TOKEN_FUNCTION_INITIZLIZER:
            if (root_node) {
                printf("Error: Function must be the root node\n");
                exit(1);
            }
            root_node = new_node;
            last_node = new_node;
            last_operator_node = new_node;
            break;    
        case TOKEN_BRACKET_OPEN: {
            if (root_node && root_node->type == TOKEN_FUNCTION_INITIZLIZER) {
                Stack *stack = getParamters(root_node, theStr, 1);
                new_node->value.type = TYPE_LIST;
                new_node->value.list = stack;
                new_node->type = TOKEN_PARAMETERS;
                root_node->left = new_node;
            } else if (last_node->type == TOKEN_IDENTIFIER) {
                Stack *stack = getParamters(root_node, theStr, 0);
                new_node->value.type = TYPE_LIST;
                new_node->value.list = stack;
                new_node->type = TOKEN_PARAMETERS;
                last_node->left = new_node;
                if (!root_node) {
                    root_node = last_node;
                }
            } else {return false;}
            break;
        }
        case TOKEN_SCOPE_BRACKET_CLOSE: {
            root_node = new_node;
            break;
        }
        case TOKEN_RETURN: {
            // if (root_node->type != TOKEN_FUNCTION_INITIZLIZER) {
            //     printf("Error: 'return' keyword used outside a function", root_node->type);
            //     exit(1);
            // }
            if (root_node) {
                printf("Root node: %d\n", root_node->type);
                printf("Error: unexpected return keyword\n");
                exit(1);
            }
            root_node = new_node;
            last_node = new_node;
            last_operator_node = new_node;
            break;
        }
        case TOKEN_SCOPE_BRACKET_OPEN: {
            if (root_node->type != TOKEN_FUNCTION_INITIZLIZER) return false;
            bool f = false;
            Stack *stack = createStack(10);
            // printf("Function Open\n");
            while (**theStr != '\0') {
                ASTNode* ast = tokenize_line(theStr, TOKEN_SCOPE_BRACKET_CLOSE);
                // printf("FuncWork: %d\n", ast->type);
                if(!ast) {*theStr += 1; continue;}
                if (ast->type == TOKEN_SCOPE_BRACKET_CLOSE) {
                    // printf("Function Close\n");
                    new_node->type = TOKEN_EOL;
                    f = true;
                    break;
                }
                stack_push(stack, (void *)ast);
                // Value result = evalutate(ast);
            }
            if (!f) {
                printf("funcWork: Expected closing bracket\n");
                exit(1);
            }
            new_node->value.type = TYPE_LIST_BODY;
            new_node->value.list = stack;
            root_node->body = new_node;
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