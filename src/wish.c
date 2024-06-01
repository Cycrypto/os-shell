#include "wish.h"

int main(int argc, char** argv) {
    if (argc > 1) {
        batch_mode(argv[1]);
    } else {
        interactive_mode();
    }

    return 0;
}

void interactive_mode() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        printf("%s", PROMPT); // 프롬프트 출력
        fflush(stdout); // 스트림에 남아있는 데이터를 출력위치로 보내준다(출력 버퍼를 비운다.)

        if (fgets(input, sizeof(input), stdin) == NULL) {
            exit(0); // EOF일 경우 종료
        }

        input[strcspn(input, "\n")] = '\0'; // 개행 문자 제거

        if (strlen(input) == 0) { // 공백 입력 무시
            continue;
        }

        // background 명령어 체크
        char *token[10];
        int count = count_background(input);
        if (count > 0) {
            split_command(input, token, 10);
            for (int i = 0; i < count; i++) {
                execute_command(token[i], 1); // 백그라운드로 실행
            }
            execute_command(token[count], 0); // 일반 실행
        } else {
            execute_command(input, 0);
        }

    }
}

// 배치 모드 구현 함수
void batch_mode(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) { // 파일 열기 실패
        print_error();
        exit(1); // 오류로 종료
    }

    char line[MAX_INPUT_SIZE];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // 개행 문자 제거

        if (strlen(line) == 0) { // 공백 줄 무시
            continue;
        }

        // background 명령어 체크
        char *token[10];
        int count = count_background(line);
        if (count > 0) {
            split_command(line, token, 10);
            for (int i = 0; i < count; i++) {
                execute_command(token[i], 1); // 백그라운드로 실행
            }
	    execute_command(token[count], 0); // 일반 실행
        } else {
            execute_command(line, 0);
        }
    }

    fclose(file); // 파일 닫기
}

void execute_command(char* command, int isbg) {
    char* args[10];
    int argc = 0;

    char* token = strtok(command, " \t");
    while (token != NULL) {
        args[argc++] = token;
        token = strtok(NULL, " \t");
    }

    args[argc] = NULL;

    // 리다이렉션 처리
    char* output_file = NULL;
    for (int i = 0; i < argc; i++) {
        if (strcmp(args[i], ">") == 0) {
            if (i + 1 < argc) {
                output_file = args[i + 1];
                args[i] = NULL;
                argc = i; // 리다이렉션 연산자 이후의 인자를 제거
                break;
            } else {
                print_error();
                return;
            }
        }
    }

    if (argc == 0) {
        return;
    }

    // 내장 명령어 처리
    if (strcmp(args[0], "cd") == 0) {
        if (argc != 2) {
            print_error();
        } else {
            command_cd(args[1]);
        }
    } else if (strcmp(args[0], "path") == 0) {
        command_path(args + 1);
    } else if (strcmp(args[0], "exit") == 0) {
        command_exit();
    } else {
        pid_t pid = fork();
        if (pid < 0) {
            print_error();
        } else if (pid == 0) {
            if (output_file != NULL) {
                int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
                if (fd < 0) {
                    print_error();
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            execvp(args[0], args);
            print_error();
            exit(1);
        } else {
            if (isbg == 1) {
                printf("백그라운드 프로세스 PID: %d\n", pid);
            } else {
                waitpid(pid, NULL, 0); // 수정: wait에서 waitpid로 변경
            }
        }
    }
}

void command_cd(char* directory) {
    if (directory == NULL || strcmp(directory, "~") == 0) {
        directory = getenv("HOME");
    }
    if (chdir(directory) == 0) {
        char cwd[MAX_INPUT_SIZE];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Directory changed to %s\n", cwd);
        }
    } else {
        print_error();
    }
}

void command_path(char** paths) {
    if (paths[0] == NULL) {
        setenv("PATH", "", 1);
        printf("PATH cleared\n");
    } else {
        char new_path[MAX_INPUT_SIZE] = "";
        for (int i = 0; paths[i] != NULL; i++) {
            if (i > 0) {
                strcat(new_path, ":");
            }
            strcat(new_path, paths[i]);
        }
        setenv("PATH", new_path, 1);
        printf("PATH set to: %s\n", new_path); // 시각화 위한 출력 추가
    }
}

void command_exit() {
    printf("Exit shell.\n");
    exit(0);
}

void print_error() {
    write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
}

int count_background(const char *input) {
    int count = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '&') {
            count++;
        }
    }
    return count;
}

void split_command(char *input, char *token[], int tok) {
    char *cmd = strtok(input, "&");
    int i = 0;
    while (cmd != NULL && i < tok) {
        token[i++] = cmd; // 각 토큰을 복제하여 저장합니다.
        cmd = strtok(NULL, "&");
    }
    token[i] = NULL; // 토큰 배열의 끝을 나타내기 위해 NULL을 추가합니다.
}

