#pragma once

#include "../Stack/Stack.h"
#include "../LangObject/LangObject.h"
#include "../RuntimeStorage/VariableStorage.h"


typedef struct VirtualMachine {
    Stack *frames;
    Stack *objects;
} VirtualMachine;

typedef struct Frame {
    struct Frame *parent;
    HashTable *references;
} Frame;

VirtualMachine* createVM();
Frame* createFrame(Frame* parent, int hashTableSize);
ASTNode* searchForRefInFrame(Frame *frame, char *key);
void insertFrame(VirtualMachine* vm, Frame *frame);
void VMTrackObject(VirtualMachine* vm, LangObject* item);
void freeVM(VirtualMachine* vm);
