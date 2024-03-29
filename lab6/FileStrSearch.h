#pragma once

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>

#ifdef __unix__
#include <sys/select.h>
#elif defined(_WIN32) || defined(WIN32)
#include <winsock.h>
#endif

#define FILE_OPEN_FAIL (-1)
#define FILE_CLOSE_FAIL (-1)
#define READ_FAIL (-1)
#define TABLE_SIZE (500)
#define TABLE_STANDART_VAL (-1)
#define READ_END (0)
#define NO_LINES (0)
#define END_LINE_CHAR ('\n')
#define LSEEK_FAIL ((off_t)-1)
#define FILE_ARG_IDX (1)
#define READ_CNT (1)
#define NUM_NOT_NULL (1)
#define NOT_FILE_END (1)
#define LLINT 0
#define LINE_NUM_SIZE 100
#define TIME_SEC (5)
#define TIME_USEC (0)
#define SELECT_ERROR (-1)
#define TIME_OVER (0)
#define SELECT_FAIL (-2)
#define FD_NOT_SET (-3)
#define READY_TO_GET_NUM (1)

int isCorrectLineNum(size_t lineNumber, size_t linesNum);
void initTable(void* table, int value);
size_t findLongestStrSize(const size_t* lineLength, size_t linesNum);
int openFile(const char* fileName);
int closeFile(int fileDescriptor);
size_t createOffsetTable(off_t* offsets, size_t* lineLength, int fileDescriptor);
void printStringsToUser(int fileDescriptor, const off_t* offsets, const size_t* lineLength, size_t linesNum);
long long getLineNum();
