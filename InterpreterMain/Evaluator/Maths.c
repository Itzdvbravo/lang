#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../TokensAndNode/TokensAndNode.h"
#include "../../Frames/Frames.h"
#pragma warning(disable : 4996)

void addition(ASTNode *node, Value *value, VirtualMachine *vm, Frame* currentFrame, Value (*evalutate)(ASTNode *, VirtualMachine*, Frame*)) {
    Value leftValue = evalutate(node->left, vm, currentFrame);
    Value rightValue = evalutate(node->right, vm, currentFrame);
    if (leftValue.type == TYPE_NUM && rightValue.type == TYPE_NUM) {
        value->type = TYPE_NUM; value->i = leftValue.i + rightValue.i;
    } else if (leftValue.type == TYPE_STRING && rightValue.type == TYPE_STRING) {
        int totalLength = strlen(leftValue.c) + strlen(rightValue.c);
        char *result = (char *)malloc(totalLength + 1);
        result[0] = '\0';
        strncat(result, leftValue.c, totalLength - strlen(result));
        strncat(result, rightValue.c, totalLength - strlen(result));
        value->type = TYPE_STRING; value->c = result;
    } else {
        printf("Error: Invalid types for addition, given %d & %d\n", leftValue.type, rightValue.type);
        exit(1);
    }
}

void minus(ASTNode *node, Value *value, VirtualMachine *vm, Frame* currentFrame, Value (*evalutate)(ASTNode *, VirtualMachine*, Frame*)) {
    Value leftValue = evalutate(node->left, vm, currentFrame);
    Value rightValue = evalutate(node->right, vm, currentFrame);
    if (!(leftValue.type == TYPE_NUM && rightValue.type == TYPE_NUM)) {
        printf("Error: Invalid types for subtraction\n");
        exit(1);
    }
    value->type = TYPE_NUM; value->i = leftValue.i - rightValue.i;
}

void multiplication(ASTNode *node, Value *value, VirtualMachine *vm, Frame* currentFrame, Value (*evalutate)(ASTNode *, VirtualMachine*, Frame*)) {
    Value leftValue = evalutate(node->left, vm, currentFrame);
    Value rightValue = evalutate(node->right, vm, currentFrame);
    if (!(leftValue.type == TYPE_NUM && rightValue.type == TYPE_NUM)) {
        printf("Error: Invalid types for multiplication\n");
        exit(1);
    }
    value->type = TYPE_NUM; value->i = leftValue.i * rightValue.i;
}

void division(ASTNode *node, Value *value, VirtualMachine *vm, Frame* currentFrame, Value (*evalutate)(ASTNode *, VirtualMachine*, Frame*)) {
    Value leftValue = evalutate(node->left, vm, currentFrame);
    Value rightValue = evalutate(node->right, vm, currentFrame);
    if (!(leftValue.type == TYPE_NUM && rightValue.type == TYPE_NUM)) {
        printf("Error: Invalid types for division\n");
        exit(1);
    }  
    if (rightValue.i == 0) {
        printf("Error: Division by zero\n");
        exit(1);
    }
    value->type = TYPE_NUM; value->i = leftValue.i / rightValue.i;
}

void modulus(ASTNode *node, Value *value, VirtualMachine *vm, Frame* currentFrame, Value (*evalutate)(ASTNode *, VirtualMachine*, Frame*)) {
    Value leftValue = evalutate(node->left, vm, currentFrame);
    Value rightValue = evalutate(node->right, vm, currentFrame);
    // printf("Left: %d, Right: %d\n", leftValue.type, rightValue.type);
    if (!(leftValue.type == TYPE_NUM && rightValue.type == TYPE_NUM)) {
        printf("Error: Invalid types for division\n");
        exit(1);
    }  
    if (rightValue.i == 0) {
        printf("Error: Division by zero\n");
        exit(1);
    }
    value->type = TYPE_NUM; value->i = (int)leftValue.i % (int)rightValue.i;
}

void greaterOrLessThan(ASTNode *node, Value *value, VirtualMachine *vm, Frame* currentFrame, Value (*evalutate)(ASTNode *, VirtualMachine*, Frame*)) {
    Value leftValue = evalutate(node->left, vm, currentFrame);
    Value rightValue = evalutate(node->right, vm, currentFrame);
    if (!((leftValue.type == TYPE_NUM || leftValue.type == TYPE_BOOL) && (rightValue.type == TYPE_NUM || rightValue.type == TYPE_BOOL))) {
        printf("Error: Invalid types for greater than comparison\n");
        exit(1);
    }
    if (leftValue.type == TYPE_BOOL) {leftValue.i = leftValue.b ? 1 : 0;}
    if (rightValue.type == TYPE_BOOL) {rightValue.i = rightValue.b ? 1 : 0;}
    if (node->type == TOKEN_GREATER_THAN) {
        value->type = TYPE_BOOL; value->b = leftValue.i > rightValue.i;
    } else if (node->type == TOKEN_LESS_THAN) {
        value->type = TYPE_BOOL; value->b = leftValue.i < rightValue.i;
    } else if (node->type == TOKEN_GREATER_THAN_EQUAL) {
        value->type = TYPE_BOOL; value->b = leftValue.i >= rightValue.i;
    } else if (node->type == TOKEN_LESS_THAN_EQUAL) {
        value->type = TYPE_BOOL; value->b = leftValue.i <= rightValue.i;
    } 
}

void comparison(ASTNode *node, Value *value, VirtualMachine *vm, Frame* currentFrame, Value (*evalutate)(ASTNode *, VirtualMachine*, Frame*)) {
    Value leftValue = evalutate(node->left, vm, currentFrame);
    Value rightValue = evalutate(node->right, vm, currentFrame);

    if (leftValue.type != rightValue.type && !((leftValue.type == TYPE_NUM || rightValue.type == TYPE_NUM) && (leftValue.type == TYPE_BOOL || rightValue.type == TYPE_BOOL))) {
        printf("Error: Invalid types for comparison\n");
        exit(1);
    }
    if (leftValue.type == TYPE_BOOL) {leftValue.type = TYPE_NUM; leftValue.i = leftValue.b ? 1 : 0;}
    if (rightValue.type == TYPE_BOOL) {rightValue.type = TYPE_NUM; rightValue.i = rightValue.b ? 1 : 0;}
    
    if (leftValue.type == TYPE_NUM) {
        value->type = TYPE_BOOL; value->b = leftValue.i == rightValue.i;
    } else if (leftValue.type == TYPE_STRING) {
        value->type = TYPE_BOOL; value->b = strcmp(leftValue.c, rightValue.c) == 0;
    } else {
        printf("Error: Invalid types for comparison\n");
        exit(1);
    }
}