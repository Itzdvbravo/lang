#include <stdio.h>
#include <stdlib.h>
#include "./LangObject.h"

LangObject* createLangObject(ASTNode *node) {
    LangObject* object = (LangObject *)(malloc(sizeof(LangObject)));
    if (object == NULL) {
        printf("Unable to allocate memory for LangObject");
        exit(1);
    }
    object->node = node;
    object->isMarked = false;
    return object;
}