# HW2 - OS: Writing Your Own Shell

This repository contains the code and files for the second homework assignment in Operating Systems: Writing Your Own Shell.

## Authors

Yosi Kariv - 205507460

## Contact Information

For any questions or concerns, please reach out to the authors:

- Yosi Kariv: [yosikariv@email.com](mailto:yosikariv@email.com)

## Project Overview

The project aims to create a custom shell that supports basic functionality such as executing commands, changing directories, and implementing various commands like `pwd`, `cd`, `nano`, `cat`, `wc`, `cp`, `clear`, `grep`, `ls`, executing pipes, and more.

This assignment helps gain experience with advanced programming techniques such as process creation, termination, and overriding an existing process (fork, wait, exec, and additional system calls).

## Features

The custom shell supports the following features:

1. Print current working directory using `pwd`
2. Change the working directory using `cd <path>`
3. Create a file using `cat > <filename>` or `nano <filename>`
4. View the contents of a file using `cat <filename>`
5. Count words, lines, and characters in a file using `wc [options/flags] <filename>`
6. Copy files using `cp <file1> <file2>`
7. Support the pipe operator `|` between processes (only for two commands)
8. Clear the terminal using `clear`
9. Return lines with a specific word/string/pattern in a file using `grep [options/flags] [Pattern] <filename>`
10. List files and directories using `ls [options/flags]`, with support for output redirection, e.g., `ls -l > output_file`
11. Print all files in the form of a tree structure using `tree`
12. Terminate the shell process using `exit`

## Setup and Usage

1. Compile the code using a C compiler like GCC: `gcc main.c -ll`
2. Run the custom shell executable: `./a.out`
3. Enter commands as you would in a regular shell, such as `pwd`, `cd`, `nano`, `cat`, `wc`, `cp`, `clear`, `grep`, `ls`, `tree`, and more.

Note: This custom shell has a limited set of features compared to a full-featured shell like Bash or Zsh. The focus of this project is on the implementation of basic shell functionality.

## Helpful Resources

The following functions are some helpful functions for this project:

- fork, exec, execvp, wait, waitpid, dup, pipe, strncmp, strlen, malloc, free, getcwd, chdir, dup2, dup, open, close, readline, gets, fgets, getchar, 'dirent.h', opendir, readdir, closedir.
