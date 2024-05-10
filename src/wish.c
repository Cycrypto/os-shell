#include "wish.h"

int main(void){
    char input[MAX_INPUT_SIZE];

    while(1){
        printf("%s", PROMPT);
        fflush(stdout);

        if(fgets(input, sizeof(input), stdin) == NULL){
            exit(0);
        }

        input[strcspn(input, "\n")] = '\0';
        execute_command(input);
    }
    return 0;
}

void print_error(){
    write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
}

void execute_command(char *command){    // 외부 명령어 실행시 사용
    char *args[MAX_INPUT_SIZE / 2];
    int argc = 0;
    char *token = strtok(command, " \t");
    while(token != NULL){
        args[argc++] = token;
        token = strtok(NULL, " \t");
    }
    args[argc] = NULL;
    
    if (argc > 0 && strcmp(args[0], "exit") == 0)
        exit(0);

    pid_t pid = fork();
    if (pid < 0){
        print_error();  // 프로세스를 생성하지 못한 경우
    }
    else if(pid == 0){  //명령어 검색 후 명령어가 없는 경우
        execvp(args[0], args);
        print_error();
        exit(1);
    }
    else{
        wait(NULL); // 자식 프로세스 대기
    }
}