#include <stdlib.h>
#include <stdio.h>
#include "./Frames.h"

VirtualMachine* createVM() {
    Stack* frames = createStack(5);
    Stack* objects = createStack(20);
    VirtualMachine* vm = (VirtualMachine *)(malloc(sizeof(VirtualMachine)));
    if (vm == NULL) {
        printf("Unable to allocate memory for virtual machine");
        exit(1);
    }

    vm->frames = frames;
    vm->objects = objects;
    return vm;
}

Frame* createFrame(Frame* parent, int hashTableSize) {
    HashTable* references = createHashTable(hashTableSize);
    Frame* frame = (Frame *)(malloc(sizeof(Frame)));
    if (frame == NULL) {
        printf("Unable to allocate memory for frame");
        exit(1);
    }
    frame->parent = parent;
    frame->references = references;
    return frame;
}

ASTNode* searchForRefInFrame(Frame *frame, char *key) {
    ASTNode* node = searchInHashTable(frame->references, key);
    if (node == NULL) {
        if (frame->parent != NULL) {
            node = searchInHashTable(frame->parent->references, key);
            return node;
        } else {
            return NULL;
        }
    }
    return node;
}

void insertFrame(VirtualMachine* vm, Frame *frame) {
    void* _frame = (void *)frame;
    stack_push(vm->frames, _frame);
}

void VMTrackObject(VirtualMachine* vm, LangObject* item) {
    stack_push(vm->objects, (void *)item);
}

void freeVM(VirtualMachine *vm) {
	for (int i = 0; i < vm->frames->top+1; i++) {
		//printf("Freeing frame %d\n", i);
		Frame* frame = (Frame*)stack_pop(vm->frames);
        clearStorage(frame->references);
        free(frame->references);
        free(frame);
	}
    for (int i = 0; i < vm->objects->top + 1; i++) {
        LangObject* obj = (LangObject*)stack_pop(vm->objects);
        /*freeLangObject(obj);*/
    }
	stack_free(vm->frames);
	stack_free(vm->objects);
    free(vm);
}