#pragma once

#include <stdbool.h>
#include "../TokensAndNode/TokensAndNode.h"

typedef struct LangObject {
    ASTNode *node;
    bool isMarked;
} LangObject;

LangObject* createLangObject(ASTNode *node);