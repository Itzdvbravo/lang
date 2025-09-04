#include <stdio.h>
#include <stdlib.h>
#include "./Stack.h"

Stack* createStack(int capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        printf("Unable to allocate memory for stack");
        exit(1);
    }
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (void**)malloc(stack->capacity * sizeof(void*));
    if (stack->array == NULL) {
        printf("Unable to allocate memory for stack array");
        exit(1);
    }
    return stack;
}

void stack_push(Stack* stack, void* item) {
    if (stack->capacity - 1 == stack->top) {
        stack->capacity *= 2;
        stack->array = (void**)realloc(stack->array, stack->capacity * sizeof(void *));
        if (stack->array == NULL) {
            printf("Unable to reallocate memory for stack array");
            exit(1);
        }
    }
    stack->array[++stack->top] = item;
}

void* stack_pop(Stack* stack) {
    if (stack->top == -1) {return NULL;}
    int index = stack->top--;
    void* item = stack->array[index];
    stack->array[index] = NULL;
    return item;
}

void stack_free(Stack* stack) {
    if (stack == NULL) return;
    free(stack->array);
    free(stack);
}