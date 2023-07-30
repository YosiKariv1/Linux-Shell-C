#ifndef SHELL_H
#define SHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_ARGS 1024
#define READ 0
#define WRITE 1

typedef struct parseInfo {
    char *command;
    char **arguments;
} parseInfo;

// Function declarations
char *readline(char*);
parseInfo *parse(char *cmdLine);
void executeCommand(parseInfo *info);
void pwd();
int ifcd(parseInfo *info);
void nano(char* filename);
void cat(parseInfo *info);
void wc(char **arguments);
void cp(char **arguments);
void clear();
void grep(char **arguments);
void executePipe(parseInfo* info);
void ls(char **arguments);
int isPipe(char **arguments);
void printCurrentPath();
void printTree(char* path, int depth);

#endif // SHELL_FUNCTIONS_H