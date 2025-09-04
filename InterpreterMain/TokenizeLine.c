#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "./ASTTreeCreatorTypes/MathWork.c"
#include "./ASTTreeCreatorTypes/BasicWork.c"
#include "./ASTTreeCreatorTypes/FuncWork.c"
#include "./ASTTreeCreatorTypes/ConditionWork.c"
#include "./Tokenizer.c"

ASTNode* tokenize_line(char **theStr, TokenType extraReturningToken) {
    Token currentToken;
    currentToken.type = TOKEN_PREPROCESSING;

    ASTNode *root_node = NULL;
    ASTNode *last_operator_node = NULL;
    ASTNode *last_node = NULL;


    int count = 0;
    while (**theStr != '\0' && currentToken.type != TOKEN_EOF && currentToken.type != TOKEN_EOL) {
        currentToken = getToken(theStr, root_node);
        // printf("Token %d | ", currentToken.type);
        // printf("Token %d %d |\n", currentToken.type, extraReturningToken);
        if (count > 1000) {
            printf(" || Error: Too many tokens || \n");
            exit(1);
        };

        ASTNode *new_node = newNode(currentToken.type, currentToken.value, NULL, NULL, NULL, NULL);

        //the alignment is required this way
        bool found = MathWork(&new_node, &root_node, &last_operator_node, &last_node, currentToken);
        
        if (!found) {
            found = ConditionWork(&new_node, &root_node, &last_operator_node, &last_node, currentToken, tokenize_line, theStr);
        }
        if (!found) {
            found = FuncWork(&new_node, &root_node, &last_operator_node, &last_node, currentToken, tokenize_line, theStr);
        } 
        if (!found) {
            found = BasicWork(&new_node, &root_node, &last_operator_node, &last_node, currentToken);
        }

        if (new_node->type == extraReturningToken || new_node->type == TOKEN_EOL) {
			if (new_node->type == TOKEN_EOL) { freeNodes(&new_node); }
            return root_node;
        }

        if (!found && currentToken.type != TOKEN_EOL && currentToken.type != TOKEN_EOF) {
            printf("TokenizeLine: Unknown token type %d\n", currentToken.type);
            exit(1);
        }

        
        count++;
    }

    return root_node;
}