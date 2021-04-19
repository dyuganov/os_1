#pragma once

#include <sys/types.h>

#define FILE_OPEN_FAIL (-1)
#define TABLE_SIZE (250)
#define TABLE_STANDART_VAL (-1)
#define READ_END (0)
#define END_LINE_CHAR ('\n')
#define LSEEK_FAIL ((off_t)-1)
#define FILE_ARG_IDX (1)
#define FILE_CLOSE_FAIL (-1)
#define READ_CNT (1)
#define TIME_NOT_OUT (1)

int isCorrectLineNum(size_t lineNumber, size_t linesNum);
void initTable(void* table, int value);
size_t findLongestStrSize(const size_t* lineLength);
int openFile(const char* fileName);
int closeFile(int fileDescriptor);
size_t createOffsetTable(off_t* offsets, size_t* lineLength, int fileDescriptor);
void printStringsToUser(int fileDescriptor, const off_t* offsets, const size_t* lineLength, size_t linesNum);