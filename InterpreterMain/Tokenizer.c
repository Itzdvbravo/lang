#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../TokensAndNode/TokensAndNode.h"
#pragma warning(disable : 4996)

float returnContinuousNumber(char** str) {
    char *curStr = *str;
    int numCount = 0;
    float num = 0;
    while (*curStr != '\0' && *curStr != ' ' && *curStr != ';' && isdigit(*curStr)) {
        num = num * 10 + (*curStr - '0');
        numCount++;
        curStr++;
    }
    *str = curStr;
    return num;
}

Token getToken(char **str, ASTNode *root_node) {
    Token token;
    token.value.type = TYPE_NOT_ACTUAL_VALUE;
    while (**str != '\0') {
        if (**str  == ' ') {
            *str += 1;
            continue;
        } else if (**str  == 's' && *(*str+1) == 'e' && *(*str+2) == 't' && *(*str+3) == ' ') {
            token.type = TOKEN_VAR_INITIALIZER;
            *str += 4;
            return token;
        } else if (isdigit(**str)) {
            token.type = TOKEN_NUM;
            token.value.type = TYPE_NUM;
            token.value.i = returnContinuousNumber(str);
            return token;
        } else if (**str == '=' && *(*str+1) == '=') {
            token.type = TOKEN_COMPARISON;
            *str += 2;
            return token;
        } else if (**str == '=') {
            token.type = TOKEN_ASSIGN;
            *str += 1;
            return token;
        } else if (**str == ';') {
            token.type = TOKEN_EOL;
            *str += 1;
            return token;
        } else if (**str == '+') {
            token.type = TOKEN_ADD;
            *str += 1;
            return token;
        } else if (**str == '-') {
            token.type = TOKEN_MINUS;
            *str += 1;
            return token;
        } else if (**str == 'i' && *(*str+1) == 'f') {
            token.type = TOKEN_IF;
            *str += 2;
            return token;
        } else if (**str == 'e' && *(*str+1) == 'l' && *(*str+2) == 's' && *(*str+3) == 'e') {
            token.type = TOKEN_ELSE;
            *str += 4;
            return token;
        } else if (**str == '*') {
            token.type = TOKEN_MULTIPLY;
            *str += 1;
            return token;
        } else if (**str == '/') {
            token.type = TOKEN_DIVIDE;
            *str += 1;
            return token;
        } else if (**str == '%') {
            token.type = TOKEN_MODULUS;
            *str += 1;
            return token;
        } else if (**str == '>') {
            token.type = TOKEN_GREATER_THAN;
            *str += 1;
            return token;
        } else if (**str == '<') {
            token.type = TOKEN_LESS_THAN;
            *str += 1;
            return token;
        } else if (**str == '(') {
            token.type = TOKEN_BRACKET_OPEN;
            *str += 1;
            return token;
        } else if (**str == ')') {
            token.type = TOKEN_BRACKET_CLOSE;
            *str += 1;
            return token;
        } else if (**str == ',') {
            token.type = TOKEN_COMMA;
            *str += 1;
            return token;
        } else if (**str == '{') {
            token.type = TOKEN_SCOPE_BRACKET_OPEN;
            *str += 1;
            return token;
        } else if (**str == '}') {
            token.type = TOKEN_SCOPE_BRACKET_CLOSE;
            *str += 1;
            return token;
        } else if (**str == 'p' && *(*str+1) == 'r' && *(*str+2) == 'i' && *(*str+3) == 'n' && *(*str+4) == 't') {
            token.type = TOKEN_FUNCTION_PRINT;
            *str += 5;
            return token;
        } else if (**str == 'f' && *(*str+1) == 'u' && *(*str+2) == 'n' && *(*str+3) == 'c') {
            token.type = TOKEN_FUNCTION_INITIZLIZER;
            *str += 4;
            return token;
        } else if (**str == 'r' && *(*str+1) == 'e' && *(*str+2) == 't' && *(*str+3) == 'u' && *(*str+4) == 'r' && *(*str+5) == 'n') {
            token.type = TOKEN_RETURN;
            *str += 6;
            return token;
        } else if (**str == '"') {
            token.type = TOKEN_CHAR;
            *str += 1;
            token.value.type = TYPE_STRING;
            int i = 0;
            while (**str != '\0' && **str != '"') {
                *str += 1;
                i++;
            }
            if (**str == '"') {
                *str += 1;
            } else {
                printf("Error: Expected closing quote\n");
                exit(1);
            }
            char *string = (char *)malloc(i+1);
            if (string == NULL) {
                printf("Error: Memory allocation failed\n");
                exit(1);
            }
            strncpy(string, *str - i - 1, i);
            string[i] = '\0';
            token.value.c = string;
            return token;
        } else {
            token.type = TOKEN_IDENTIFIER;
            token.value.type = TYPE_STRING;
            char *identifier = (char *)malloc(sizeof(char)*30); // Allocate memory for the identifier
            if (identifier == NULL) {
                printf("Error: Memory allocation failed\n");
                exit(1);
            }
            int i = 0;
            while (**str != '\0' && isalpha((unsigned char)**str)) {
                if (i >= 29) {
                    printf("Error: Identifier too long\n");
                    exit(1);
                }
                identifier[i] = **str;
                i++;
                *str += 1;
            }
            identifier[i] = '\0';
            token.value.c = identifier;
            return token;
        }
    }
    token.type = TOKEN_EOF;
    return token;
}