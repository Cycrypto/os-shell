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
void execute_command(char *command, int isbg); // 백그라운드 플래그 추가
void interactive_mode();
void batch_mode(const char* filename);
void command_cd(char* directory); // cd command
void command_path(char** path); // path command
void command_exit(); // exit command
int count_background(const char *input); // & 기호 개수 카운트
void split_command(const char *input, char *token[], int tok); // & 기호 기준 명령어 분할
#endif
