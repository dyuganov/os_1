#include "FileStrSearch.h"

int isCorrectLineNum(const size_t lineNumber, const size_t linesNum){
    return (lineNumber > 0) && (lineNumber <= TABLE_SIZE) && (lineNumber <= linesNum);
}

void initTable(void* table, const int value){
    memset(table, value, TABLE_SIZE);
}

bool isLseekError(const off_t lseekResult){
    if(lseekResult == LSEEK_FAIL) {
        perror("Lseek fail");
        return true;
    }
    return false;
}

bool isOpenError(const int fileDescriptor){
    if(fileDescriptor == FILE_OPEN_FAIL) {
        perror("Open file fail");
        return true;
    }
    return false;
}

bool isCloseError(const int closeResult){
    if(closeResult == FILE_CLOSE_FAIL) {
        perror("Close file fail");
        return true;
    }
    return false;
}

bool isReadError(const ssize_t readResult){
    if(readResult == READ_FAIL) {
        perror("Read fail");
        return true;
    }
    return false;
}

bool isReadEnd(const ssize_t readResult){
    if(readResult == READ_END) return true;
    return false;
}

int openFile(const char* fileName){
    int fileDescriptor = FILE_OPEN_FAIL;
    fileDescriptor = open(fileName, O_RDONLY);
    if(isOpenError(fileDescriptor)) return FILE_OPEN_FAIL;
    return fileDescriptor;
}

int closeFile(int fileDescriptor){
    int closeResult = close(fileDescriptor);
    if(isCloseError(closeResult)) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

bool isLineEnd(const char symbol){
    if(symbol == END_LINE_CHAR || symbol == EOF) return true;
    return false;
}

bool isBigFile(const size_t linesNum){
    if(linesNum >= TABLE_SIZE) {
        perror("File is too big");
        return true;
    }
    return false;
}

size_t createOffsetTable(off_t* offsets, size_t* lineLength, const int fileDescriptor){
    size_t linesNum = 0;
    size_t currLineLength = 0;
    char currChar = 0;
    offsets[0] = lseek(fileDescriptor, 0L, SEEK_CUR);
    while (NUM_NOT_NULL){
        ssize_t readResult = read(fileDescriptor, &currChar, READ_CNT);
        if(isReadError(readResult)) return NO_LINES;
        if(isReadEnd(readResult)) break;
        if(isLineEnd(currChar)){
            currLineLength += 1;
            lineLength[linesNum] = currLineLength;
            linesNum += 1;
            long lseekResult = lseek(fileDescriptor, 0L, SEEK_CUR);
            if(isLseekError(lseekResult)) return NO_LINES;
            offsets[linesNum] = lseekResult;
            currLineLength = 0;
        } else{
            ++currLineLength;
        }
        if(isBigFile(linesNum)) return NO_LINES;
    }
    return linesNum;
}

size_t findLongestStrSize(const size_t* lineLength, const size_t linesNum){
    long long max = 0;
    for (size_t i = 0; i < linesNum; ++i){
        if(lineLength[i] > max) max = lineLength[i];
    }
    return max;
}

bool isStop(const long long lineNumber){
    return (lineNumber == 0);
}

bool isDigit(char symbol){
    if(symbol >= '0' && symbol <= '9') return true;
    return false;
}

bool isNumber(const char* number, const size_t size){
    for(size_t i = 0; i < size; ++i){
        if(!isDigit(number[i])) return false;
    }
    return true;
}

bool isFgetsError(const char* fgetsResult){
    if(fgetsResult == NULL) return true;
    return false;
}

long long getLineNum(){
    long long result = -1;
    char lineNumber[LINE_NUM_SIZE];
    char* fgetsResult = fgets(lineNumber, LINE_NUM_SIZE, stdin);
    if(isFgetsError(fgetsResult)) return result;

    char* endPtr = NULL;
    size_t linesNumLength = strnlen(lineNumber, LINE_NUM_SIZE) - 1;
    if(isNumber(lineNumber, linesNumLength)) result = strtoll(lineNumber, &endPtr, LLINT);
    return result;
}

void printStringsToUser(const int fileDescriptor, const off_t* offsets, const size_t* lineLength, const size_t linesNum){
    if(linesNum < 1) return;
    const size_t strBufSize = findLongestStrSize(lineLength, linesNum);
    char currStrBuf[strBufSize];
    printf("Enter string number\n");
    while(1){
        long long lineNumber = getLineNum();
        if(isStop(lineNumber)) return;
        if(isCorrectLineNum(lineNumber, linesNum)){
            memset(currStrBuf, 0, strBufSize);
            int lseekResult = lseek(fileDescriptor, offsets[lineNumber-1], SEEK_SET);
            if(isLseekError(lseekResult)) return;
            int readResult = read(fileDescriptor, currStrBuf, lineLength[lineNumber - 1]);
            if(isReadError(readResult)) return;
            printf("%s", currStrBuf);
        }
        else{
            printf("Wrong value\n");
        }
    }
}
