#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../TokensAndNode/TokensAndNode.h"
#include "../../Frames/Frames.h"


void ifelse(ASTNode *node, Value *value, VirtualMachine *vm, Frame* currentFrame, Value (*evalutate)(ASTNode *, VirtualMachine*, Frame*)) {
    if (!node->body) return;
    ASTNode *condition = node->right;
    ASTNode *body = node->body;
    ASTNode *else_node = node->left;

    Value val = evalutate(condition, vm, currentFrame);
    bool isTrue = true;

    if (val.type == TYPE_BOOL) {
        isTrue = val.b;
    } else if (val.type == TYPE_NUM) {
        isTrue = val.i != 0;
    } else if (val.type == TYPE_STRING) {
        isTrue = strcmp(val.c, "") != 0;
    } else {
        printf("Error: Invalid condition type\n");
        exit(1);
    }

    if (isTrue) {
        // for (int i = 0; i <= body->value.list->top; i++) {
        //     ASTNode* node = (ASTNode*)body->value.list->array[i];
        //     evalutate(node);
        // }
        *value = body->value;
    } else if(else_node) {
        *value = else_node->value;
        // if (else_body) {
        //     for (int i = 0; i <= else_body->value.list->top; i++) {
        //         ASTNode* node = (ASTNode*)else_body->value.list->array[i];
        //         evalutate(node);
        //     }
        // }
    }
}