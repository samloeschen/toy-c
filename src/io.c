#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"

int readAll(char *path, char **out) {
    FILE *filePtr = fopen(path, "rb");

    if (!filePtr) { return IO_ERROR_FILE_DOES_NOT_EXIST; }

    fseek(filePtr, 0L, SEEK_END);
    long size = ftell(filePtr);
    *out = malloc((size + 1) * sizeof(char));
    fseek(filePtr, 0L, SEEK_SET);

    fread(*out, sizeof(char), size, filePtr);
    (*out)[size] = '\0'; // just to be safe!
    fclose(filePtr);

    return 0;
}