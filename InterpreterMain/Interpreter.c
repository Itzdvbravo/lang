#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include "TokenizeLine.c"
#include "../TokensAndNode/TokensAndNode.h"
#include "./Evaluator/Evaluator.c"
#include "../Frames/Frames.h"

#define INITIAL_SIZE = 50;

void process_lines(char *str) {
    VirtualMachine* vm = createVM();
    Frame* frame = createFrame(NULL, 97);
    insertFrame(vm, frame);

    while (*str != '\0') {
        ASTNode* ast = tokenize_line(&str, TOKEN_EOL);
        //printf(" - Root Node %d -\n", ast->type);
        if(!ast) {str+=1; continue;}

        Value result = evalutate(ast, vm, frame);
        while (result.type == TYPE_LIST_BODY) {
            result.type = TYPE_NOT_ACTUAL_VALUE;
            for (int i = 0; i <= result.list->top; i++) {
                ASTNode* node = (ASTNode*)result.list->array[i];
                result = evalutate(node, vm, frame);
            }
        }
        //Think about freeing nodes later
        freeNodes(&ast);
    }
	freeVM(vm);
	// clearStorage();
}

void Interpret(char *str) {
    process_lines(str);
    // clearStorage();
}