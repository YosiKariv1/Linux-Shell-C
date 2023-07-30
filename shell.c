///////////// HW2 - OS /////////////
///// Yosi Kariv - 205507460 /////
///// Eldar Abramobich - 316462795 ///
////////////////////////////////////

#include "shell.h"


//Reads what is written and returns the line
char *readline(char* prompt) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    printf("%s", prompt);
    read = getline(&line, &len, stdin);

    if (read == -1) {
        free(line);
        return NULL;
    }

    // Remove trailing newline character
    if (line[read - 1] == '\n') {
        line[read - 1] = '\0';
    }

    return line;
}

//tooks the line and changes it to a command and its arguments
parseInfo *parse(char *cmdLine) {
    const char *delimiters = " \n";
    char *command = strtok(cmdLine, delimiters);
    char **arguments = malloc(sizeof(char*) * MAX_ARGS);
    int argCount = 0;

    while (command != NULL) {
        arguments[argCount] = malloc(strlen(command) + 1);
        strcpy(arguments[argCount], command);
        argCount++;
        command = strtok(NULL, delimiters);
    }

    arguments[argCount] = NULL; // Properly NULL-terminate the arguments array

    parseInfo *info = malloc(sizeof(parseInfo));
    info->command = arguments[0];
    info->arguments = &arguments[1];

    printf("Command: %s \n", info->command);
    for (int i = 0; info->arguments[i] != NULL; i++) {
        printf("Argument %d: %s \n", i, info->arguments[i]);
    }

    return info;
}



void executeCommand(parseInfo *info) {
    int out_fd = -1;

    for (int i = 0; info->arguments[i] != NULL; i++) {
        if (strcmp(info->arguments[i], ">") == 0) {
            if (info->arguments[i + 1] != NULL) {
                out_fd = open(info->arguments[i + 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
                if (out_fd < 0) {
                    perror("Error opening output file");
                    exit(1);
                }
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);

                // Remove '>' and the output file from the arguments array
                info->arguments[i] = NULL;
                break;
            } else {
                fprintf(stderr, "Error: Output file not specified after '>'\n");
                exit(1);
            }
        }
    }
    
    if (strcmp(info->command, "pwd") == 0) {
        pwd();
    } else if (strcmp(info->command, "nano") == 0) {
        nano(info->arguments[0]);
    } else if (strcmp(info->command, "cat") == 0) {
        cat(info);
    } else if (strcmp(info->command, "wc") == 0) {
        wc(info->arguments);
    } else if (strcmp(info->command, "ls") == 0) {
        ls(info->arguments);
    } else if (strcmp(info->command, "cp") == 0) {
        cp(info->arguments);
    } else if (strcmp(info->command, "grep") == 0) {
        grep(info->arguments);
    }else if (strcmp(info->command, "tree") == 0) {
        char *path;
        if (info->arguments[0] == NULL) {
            path = ".";
        } else {
            path = info->arguments[0];
        }
        printTree(path, 0);  
    }else if (strcmp(info->command, "clear") == 0) {
        clear();
    } else {
        fprintf(stderr, "Command not found: %s\n", info->command);
    }
}



//1
void pwd(){
    execlp("pwd","pwd", NULL);
}

//2
int ifcd(parseInfo *info)
{
    if (strcmp(info->command, "cd") == 0) 
    {
        // Go back to root
        if (info->arguments[0] == NULL || strcmp(info->arguments[0], "~") == 0)
            chdir(getenv("HOME"));
        
        // Go to the folder  
        else
            chdir(info->arguments[0]);
        
        return 1;
    }

    return 0;
}

//3
void nano(char* filename){
    if(filename != NULL)
        execlp("nano", "nano", filename, NULL);
    else
        perror("Must Have file name ");
}

//4
void cat(parseInfo *info) {
    if (info->arguments[0] == NULL) {
        fprintf(stderr, "Error: missing file name\n");
        return;
    }

    if (info->arguments[1] != NULL && strcmp(info->arguments[1], ">") == 0) {
        FILE *file;
        char temp[MAX_ARGS];
        // Check if there is a file name
        if (!info->arguments[2]) {
            printf("\033[0;31mThis command requires a file name\033[0m\n");
            exit(1);
        }

        file = fopen(info->arguments[2], "w");

        // Check if the file is open
        if (!file) {
            printf("\033[0;31mCould not open the file\033[0m\n");
            exit(1);
        }

        // Insert string to the file
        while (fgets(temp, MAX_ARGS, stdin)) fprintf(file, "%s", temp);
        fclose(file);
        exit(1);
    } else {
        execlp("cat", "cat", info->arguments[0], NULL);
    }
}


//5
void wc(char **arguments) {
    if (arguments[0] == NULL) {
        execl("/usr/bin/wc", "wc", NULL);
    } else if (strcmp(arguments[0], "-l") == 0) {
        execl("/usr/bin/wc", "wc", "-l", arguments[1], NULL);
    } else if (strcmp(arguments[0], "-c") == 0) {
        execl("/usr/bin/wc", "wc", "-c", arguments[1], NULL);
    } else if (strcmp(arguments[0], "-w") == 0) {
        execl("/usr/bin/wc", "wc", "-w", arguments[1], NULL);
    }
}

//6
void cp(char **arguments) {
    if (arguments[0] == NULL || arguments[1] == NULL) {
        fprintf(stderr, "Usage: cp <file1> <file2>\n");
        return;
    }
    execl("/bin/cp", "cp", arguments[0], arguments[1], NULL);
}

//7
void executePipe(parseInfo* info){
 // Setup
    parseInfo *p1, *p2;
    int fds_arr[2], childPid;

    // Look for '|' character
    for (int i = 0; info->arguments[i] != NULL; i++) {
        if (!strcmp(info->arguments[i], "|")) {
            // Setup for 2 separated tokens arrays
            p1 = (parseInfo *)malloc(sizeof(parseInfo));
            p2 = (parseInfo *)malloc(sizeof(parseInfo));
            p1->arguments = &info->arguments[0];
            p2->arguments = &info->arguments[i + 1];

            // Null-terminate the first part of the arguments
            info->arguments[i] = NULL;

            // Create pipe
            pipe(fds_arr);
            childPid = fork();
            if (childPid == 0) {
                close(fds_arr[0]); // Close read end
                dup2(fds_arr[1], 1); // Redirect stdout to the pipe
                executeCommand(p1);
                exit(EXIT_SUCCESS);
            } else {
                wait(NULL);
                close(fds_arr[1]); // Close write end
                dup2(fds_arr[0], 0); // Redirect stdin from the pipe
                if (!ifcd(p2)) {
                    executeCommand(p2);
                }
                exit(EXIT_SUCCESS);
            }

            free(p1);
            free(p2);

            break; // Exit the loop
        }
    }
}

//8
void clear() {
    execl("/usr/bin/clear", "clear", NULL);
}

//9
void grep(char **arguments) {
    if (strcmp(arguments[0], "-c") == 0) {
        char *line[] = { "grep", arguments[0], arguments[1], arguments[2], NULL };
        execvp("grep", line);
    } else {
        char *line[] = { "grep", arguments[0], arguments[1], NULL };
        execvp("grep", line);
    }
}


//10
void ls(char **arguments) {
    if(arguments[0] == NULL){
        execlp("ls","ls", NULL);
    }else{
        execlp("ls","ls","-l", NULL);
    }
}


int isPipe(char **arguments) {
    int flag = 0;
    for (int i = 0; arguments[i] != NULL; i++) {
        if (strcmp(arguments[i], "|") == 0) {
            flag = 1;
            break;
        }
    }
    if (flag && strcmp(arguments[0], "|") == 0) {
        flag = 0;
    }
    return flag;
}


void printCurrentPath()
{
    char curr_path[MAX_ARGS];
    getcwd(curr_path, MAX_ARGS);
    printf("\033[0;35m%s \033[0m: \033[0;36m%s\033[0m:",getenv("USER"),curr_path);
}

void printTree(char* path, int depth){

    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;

    if ((dir = opendir(path)) == NULL) {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue; // skip the current directory and the parent directory

        for (int i = 0; i < depth; i++) {
            printf("  "); // print proper indentation based on depth
        }
        printf("%s\n", entry->d_name); // print the name of the file or directory

        char new_path[MAX_ARGS];
        snprintf(new_path, sizeof(new_path), "%s/%s", path, entry->d_name); // concatenate the path with the current entry
        lstat(new_path, &file_stat); // get the file or directory stats

        if (S_ISDIR(file_stat.st_mode)) { // check if the entry is a directory
            printTree(new_path, depth + 1); // recursive call to print the contents of the directory
        }
    }

    closedir(dir);
}
