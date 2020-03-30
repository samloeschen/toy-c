#ifndef IO_H
#define IO_H

#define IO_ERROR_FILE_DOES_NOT_EXIST    -1
#define IO_ERROR_READ_ERROR             -2

int readAll(char *path, char **out);
#endif