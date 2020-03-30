#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"

// errors
int readAll(char *path, char **out) {
    FILE *filePtr = fopen(path, "rb");

    if (!filePtr) { return IO_ERROR_FILE_DOES_NOT_EXIST; }

    fseek(filePtr, 0L, SEEK_END);
    long size = ftell(filePtr);
    *out = calloc(size + 1, sizeof(char));
    fseek(filePtr, 0L, SEEK_SET);

    size_t newLen = fread(*out, sizeof(char), size, filePtr);
    fclose(filePtr);

    return 0;
}