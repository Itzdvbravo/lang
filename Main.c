#define _CRTDBG_MAP_ALLO
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <crtdbg.h>
#include "InterpreterMain/Interpreter.c"

int main() {
    FILE *file = fopen("Code.txt", "r"); 
    if (file == NULL) {
        printf("Could not open the filew.\n");
        return 1;
    }

    char lines[100*256] = "";
    char line[256] = "";
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        strcat(lines, line);  
    }
    fclose(file);
    Interpret(lines);
    _CrtDumpMemoryLeaks();
    return 0;
}