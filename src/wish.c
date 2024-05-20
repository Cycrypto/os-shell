#include "wish.h"

//이 브랜치에서 구현할 것
//- 내장명령 cd와 path : 대화형모드
//- 리다이렉션 기능
int main(int argc, char** argv) {
    if (argc > 1) { // 인자가 주어진 경우 배치 모드
        batch_mode(argv[1]); // 첫 번째 인자(파일 이름)를 배치 모드에 전달
    } else { // 인자가 없으면 대화형 모드
        interactive_mode(); // 대화형 모드 실행
    }

    return 0;
}

// 대화형 모드 구현 함수
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

        execute_command(input); // 명령어 실행
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

        execute_command(line); // 명령어 실행
    }

    fclose(file); // 파일 닫기
}

void execute_command(char* command) {
    char* args[10];
    int argc = 0;

    char* token = strtok(command, " \t");
    while (token != NULL) {
        args[argc++] = token;
        token = strtok(NULL, " \t");
    }

    args[argc] = NULL; 

    pid_t pid = fork();
    if (pid < 0) {
        print_error();
    } else if (pid == 0) { //여기에 cd, path 구현
        if (strcmp(args[0], "cd") == 0) {
            if (argc != 2) {
                print_error();
            } else {
                command_cd(args[1]);
            }
        }
        else if (strcmp(args[0], "path") == 0) {
            command_path(args[1]);
        }
        else {
            execvp(args[0], args); //다른 프로그램을 실행한다. 
            print_error();
            exit(1);
        }
    } else {
        wait(NULL);
    }
}

void command_cd(char* directory) {
    printf("function command_cd() executed, args : %s\n", directory);
}

void command_path(char* paths) {
    printf("function command_path() executed, args : %s\n", paths);
}

void print_error() {
    write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
}
