#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define TRUE 1
#define OPEN_ERROR -1
#define READ_ERROR -1
#define WRITE_ERROR -1
#define CLOSE_ERROR -1
#define LSEEK_ERROR (off_t)-1
#define GET_LINE_ERROR -1
#define READ_LINE_ERROR NULL
#define PRINT_LINE_ERROR -1
#define GET_NUMBER_ERROR -1
#define FILL_TABLE_ERROR -1
#define ALLOC_ERROR NULL
#define STOP_NUMBER 0
#define TABLE_SIZE 256
#define CONSOLE_INPUT_SIZE 100
#define FAIL -1
#define SUCCESS 0
#define DECIMAL_SYSTEM 0
#define INIT_CHECK 0
#define BUFFER_SIZE 1024


int fillTable(int file_descriptor, size_t* line_lengths, off_t* file_offsets)
{
    char* read_buffer = (char*)malloc(sizeof(char) * BUFFER_SIZE);

    if (read_buffer == ALLOC_ERROR)
    {
        perror("Can't allocate memory for buffer");
        return FILL_TABLE_ERROR;
    }

    int actual_buffer_size =  1;

    int current_line_index = 1;
    int file_offset_index = 1;
    int current_offset = 0;
    
    while (actual_buffer_size > 0)
    {
        actual_buffer_size = read(file_descriptor, read_buffer, BUFFER_SIZE);
        if (actual_buffer_size == READ_ERROR)
        {
            free(read_buffer);
            perror("Can't read current text");
            return FILL_TABLE_ERROR;
        }
        
        for (size_t i = 0; i < actual_buffer_size; i++)
        {
            line_lengths[current_line_index]++;
            current_offset++;
            if (read_buffer[i] == '\n')
            {
                file_offsets[file_offset_index] = (off_t)(current_offset - line_lengths[current_line_index]);
                current_line_index++;
                file_offset_index++;
            }
        }
        
    }

    free(read_buffer);
    return (current_line_index);
}

long long getNumber()
{
        char text_for_user[23] = "Enter number of line: ";
        long long line_number;
        char console_input[CONSOLE_INPUT_SIZE]; 

        int write_check = INIT_CHECK;
        write_check = write(STDOUT_FILENO, text_for_user, 23);
        if (write_check == WRITE_ERROR)
        {
            perror("Can't print message for user");
            return GET_NUMBER_ERROR;
        }
        
        int read_check = INIT_CHECK;
        read_check = read(STDIN_FILENO, console_input, CONSOLE_INPUT_SIZE);
        if (read_check == READ_ERROR)
        {
            perror("Can't read current text");
            return GET_NUMBER_ERROR;
        }

        char* endptr = NULL;
        line_number = strtoll(console_input, &endptr, DECIMAL_SYSTEM);
        
        return line_number;
}

char* readLineFromFile(int file_descriptor, int line_length, off_t offset)
{
    char* line = NULL;
    line = (char*)malloc(sizeof(char) * line_length);

        if (line == ALLOC_ERROR)
        {
            perror("Can't allocate memory with realloc");
            free(line);
            return READ_LINE_ERROR;
        }

        off_t lseek_check  = INIT_CHECK;
        lseek_check = lseek(file_descriptor, offset, SEEK_SET);

        if (lseek_check == LSEEK_ERROR)
        {
            perror("Seek error");
            free(line);
            return READ_LINE_ERROR;
        }
    
        int read_check = INIT_CHECK;
        read_check = read(file_descriptor, line, line_length);

        if (read_check == READ_ERROR)
        {
            perror("Can't read current text");
            free(line);
            return READ_LINE_ERROR;
        }

        return line;
}

int printLine(char* line, int line_length)
{
    int write_check = INIT_CHECK;
    write(STDOUT_FILENO, line, line_length);
    if (write_check == WRITE_ERROR)
    {
        perror("Can't print line");
        free(line);
        return PRINT_LINE_ERROR;
    }

    return SUCCESS;
}

int getLines(int file_descriptor, size_t* line_lengths, off_t* file_offsets, int number_of_lines)
{
    char* line = NULL;
    off_t offset = 0;
    int line_length = 0;
    long long line_number = 0;
    
    while(TRUE)
    {
        line_number = getNumber();

        if (line_number == GET_NUMBER_ERROR)
        {
            printf("%s", "Can't get number from user");
            return GET_LINE_ERROR;
        }
        
        if (line_number == LLONG_MAX || line_number == LLONG_MIN)
        {
            perror("Invalid line number");
            continue;
        }
        
        if (line_number == STOP_NUMBER)
        {
            break;
        }
        if (line_number > number_of_lines || line_number < 0)
        {
            printf("%s", "Invalid line number: No such line in file\n");
            continue;
        }

        if (line_lengths[line_number] != 0)
        {
            offset = file_offsets[line_number];
            line_length = line_lengths[line_number];

            line = readLineFromFile(file_descriptor, line_length, offset);

            if (line == READ_LINE_ERROR)
            {
                printf("%s", "Can't read line");
                return GET_LINE_ERROR;
            }

            int print_line_check = INIT_CHECK;
            print_line_check = printLine(line, line_length);
            if (print_line_check == PRINT_LINE_ERROR)
            {
                free(line);
                printf("%s", "Can't print line");
                return GET_LINE_ERROR;
            }
            free(line);
        }
        printf("\n");  
    }
    return SUCCESS;
}

 int main(int argc, char* argv[])
 {
    off_t file_offsets[TABLE_SIZE]  = {0};
    size_t line_lengths[TABLE_SIZE]  = {0};

    int file_descriptor = INIT_CHECK;
    
    int number_of_lines = INIT_CHECK;

    if (argc != 2)
    {
        printf("Usage: a.out f1\n");
        return FAIL;
    }
    
    file_descriptor = open(argv[1], O_RDONLY);

    if (file_descriptor == OPEN_ERROR)
    {
        perror("Can't open file");
        return FAIL;
    }

    number_of_lines = fillTable(file_descriptor, line_lengths, file_offsets);
    
    if (number_of_lines == FILL_TABLE_ERROR)
    {
        printf("Error with filling the table");
        return FAIL;
    }

    int get_lines_check = INIT_CHECK;
    
    get_lines_check = getLines(file_descriptor, line_lengths, file_offsets, number_of_lines);
    
    if (get_lines_check == GET_LINE_ERROR)
    {
        printf("Error with printing lines");
        return FAIL;
    }

    int close_check = INIT_CHECK;
    close_check = close(file_descriptor);

    if (close_check == CLOSE_ERROR)
    {
        perror("Error with closing the file");
        return FAIL;
    }

    return SUCCESS;
 }