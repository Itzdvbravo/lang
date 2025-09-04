#pragma once

typedef struct Stack {
    int top;
    int capacity;
    void **array;
} Stack;

Stack* createStack(int capacity);
void stack_push(Stack* stack, void* item);
void* stack_pop(Stack* stack);
void stack_free(Stack* stack);