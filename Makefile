# 컴파일러 및 컴파일 옵션 설정
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I include

# 컴파일 대상 파일
SRC = src/*
TARGET = bin/wish

# 기본 빌드 목표
all: $(TARGET)

# 실행 파일 생성 규칙
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# 클린 규칙
clean:
	rm -f $(TARGET)
