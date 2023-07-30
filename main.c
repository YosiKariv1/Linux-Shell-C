///////////// HW2 - OS /////////////
///// Yosi Kariv - 205507460 /////
///// Eldar Abramobich - 316462795 ///
////////////////////////////////////

#include "shell.h"
#include "shell.c"

int main(int argc, char **argv) { 
    int childPid; 
    char *cmdLine;
    parseInfo *info;
    printf("\033[0;36m################# Running custom shell ####################\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\033[0m\n");
    
    while(1) {
        printCurrentPath();
        cmdLine = readline(" >> ");
        info = parse(cmdLine);
        
        if (strcmp(info->command, "exit") == 0) {
            free(cmdLine);
            free(info);
            break; 
        }

        if(!isPipe(info->arguments) && ifcd(info))
            continue;

        childPid = fork();
        if (childPid == 0) {
            // child code
            executeCommand(info); // calls
            exit(0); 
        } else if (childPid > 0) {
            //parent code
            waitpid(childPid, NULL, 0);
        } else {
            perror("fork");
        }
        
        // Free cmdLine and parseInfo resources
        free(cmdLine);
        free(info);
    }
    return 0;
}
