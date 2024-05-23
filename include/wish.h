#ifndef WISH_H
#define WISH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_INPUT_SIZE 1024
#define ERROR_MESSAGE "An error has occurred\n"
#define PROMPT "wish> "

void print_error();
void execute_command(char *command);
void interactive_mode();
void batch_mode(const char* filename);
void command_cd(char* directory);//cd �������
void command_path(char** path);//path �������
#endif
