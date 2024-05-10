#include "wish.h"

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
        fflush(stdout);

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
    } else if (pid == 0) {
        execvp(args[0], args);
        print_error();
        exit(1);
    } else {
        wait(NULL);
    }
}

void print_error() {
    write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
}
