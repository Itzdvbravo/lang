#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../TokensAndNode/TokensAndNode.h"
#include "../../RuntimeStorage/VariableStorage.h"
#include "../../Frames/Frames.h"

bool runBody(Value body, Value* value, VirtualMachine *vm, Frame* currentFrame, Value (*evalutate)(ASTNode *, VirtualMachine*, Frame*)) {
    for (int i = 0; i <= body.list->top; i++) {
        ASTNode* node = (ASTNode*)body.list->array[i];
        Value val = evalutate(node, vm, currentFrame);
        bool found = false;
        while (val.type == TYPE_LIST_BODY) {
            val.type = TYPE_NOT_ACTUAL_VALUE;
            found = runBody(val, value, vm, currentFrame, evalutate);
            if (found) return true;
        }
        if (node->type == TOKEN_RETURN) {
            *value = val;
            return true;
            break;
        }
    }
    return false;
}

void identifier(ASTNode *node, Value *value, VirtualMachine *vm, Frame* currentFrame, Value (*evalutate)(ASTNode *, VirtualMachine*, Frame*)) {
    ASTNode *found_node = searchForRefInFrame(currentFrame, node->value.c);

    if (found_node == NULL) {
        printf("Error: Variable '%s' not found\n", node->value.c);
        exit(1);
    }
    if (found_node->type == TOKEN_IDENTIFIER) {
        *value = found_node->value; 
    } else if (found_node->type == TOKEN_FUNCTION_INITIZLIZER) {
        ASTNode* para_values = node->left;
        ASTNode* para_identifier = found_node->left;
        ASTNode* body = found_node->body;
        if (para_values == NULL) {
            printf("Error: Function %s has no parameters\n", node->value.c);
            exit(1);
        } else if (para_values->type != TOKEN_PARAMETERS) {
            printf("Error: Function %s has no parameters\n", node->value.c);
            exit(1);
        } else if (para_identifier == NULL) {
            printf("Error: Function %s has no parameters\n", node->value.c);
            exit(1);
        } else if (body == NULL) {
            printf("Error: Function %s has no body\n", node->value.c);
            exit(1);
        } else if (para_values->value.list->top != para_identifier->value.list->top) {
            printf("Error: Function %s expected %d arguments, provided %d\n", node->value.c, para_identifier->value.list->top + 1, para_values->value.list->top + 1);
            exit(1);
        } else {
            // printf("Function %s called\n", node->value.c);
            Frame* newFrame = createFrame(currentFrame, 97);
            for (int i = 0; i <= para_identifier->value.list->top; i++) {
                ASTNode* identifier = (ASTNode*)para_identifier->value.list->array[i];
                ASTNode* var = (ASTNode*)para_values->value.list->array[i];
                var->type = identifier->type;
                insert(newFrame->references, identifier->value.c, var);
            }
            // printf("Function body %s called\n", node->value.c);
            runBody(body->value, value, vm, newFrame, evalutate);
        }
    } else {
        printf("Error: Variable '%s' has unsupported type\n", node->value.c);
        exit(1);
    }
}