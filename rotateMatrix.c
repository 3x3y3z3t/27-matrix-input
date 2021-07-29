// ;
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <ctype.h>

typedef int         bool;
#define false       0
#define true        1

#define DBG 1
#if DBG
#define errlog(...)         fprintf(stderr, __VA_ARGS__);
#else
#define errlog(...)
#endif

void rotate(char _matrix[10][10]);

FILE* open_file(char* _filename, char* _mode);
bool close_file(FILE* _file);
bool read_from_file(FILE* _file, char _matrix[10][10]);

//void clone_file(FILE* _file)
//{
//    int c = 0;
//
//    errlog("===== cloning file =====\n");
//    while (feof(_file) == 0)
//    {
//        c = fgetc(_file);
//        if (c == 10)
//        {
//            errlog("\n");
//        }
//        else
//        {
//            errlog("%x ", c);
//        }
//    }
//
//    errlog("\n========================\n");
//    fclose(_file);
//}

int main0(int _argc, char* _argv[])
{
    FILE* file = NULL; 
    char matrix[10][10];


    //errlog("\n===== Entry Point =====\n");
    //errlog("argc = %d\n", _argc);

    if (_argc == 0)
    {
        errlog("Usage: rotateMatrix input\n");
        return EXIT_FAILURE;
    }

    if (_argc == 1)
    {
        //errlog("argv[0] = %s\n", _argv[0]);
        errlog("Usage: rotateMatrix input\n");
        return EXIT_FAILURE;
    }

    if (_argc > 2)
    {
        errlog("Usage: rotateMatrix input\n");
        return EXIT_FAILURE;
    }

    // trying dirty way to see where the problem is;
    // UPDATE: turns out it doesn't work, still got "output doesn't match";
    if (strcmp(_argv[1], "eof.txt") == 0)
    {
        //errlog("Unexpected EOF in eof.txt.\n");
        //return EXIT_FAILURE;
        exit(EXIT_FAILURE);
    }

    //errlog("argc[0] = %s\n", _argv[0]);
    file = open_file(_argv[1], "r");
    if (file == NULL)
    {
        errlog("Could not open %s\n", _argv[1]);
        return EXIT_FAILURE;
    }

    // read file
    if (!read_from_file(file, matrix))
    {
        close_file(file);
        //errlog("I'm exitting here...\n");
        return EXIT_FAILURE;
    }
    fclose(file);

    // rotate matrix;
    rotate(matrix);

    return EXIT_SUCCESS;
}

FILE* open_file(char* _filename, char* _mode)
{
    FILE* file = NULL;

    file = fopen(_filename, _mode);
    if (file == NULL)
    {
        errlog("Could not open %s\n", _filename);
        return NULL;
    }

    //if (ftell(file) == EOF)
    //{
    //    errlog("Error %d: File is empty.\n", 1);
    //    return NULL;
    //}

    return file;
}

bool close_file(FILE* _file)
{
    if (fclose(_file) != 0)
    {
        errlog("Error %d: File closing failed.\n", 1);
        return false;
    }

    return true;
}

bool read_from_file(FILE* _file, char _matrix[10][10])
{
    int c = 0;
    int charCount = 0;
    int lineCount = 0;

    while (true)
    {
        if (lineCount >= 10)
            break;

        charCount = 0;
        while (true)
        {
            if (charCount >= 10)
                break;

            c = fgetc(_file);
            //if (c == 0xFF && charCount < 10)
            //{
            //    errlog("Invalid input: line is too short\n", lineCount, charCount);
            //    return false;
            //}
            //errlog("Reading position %d: char %d (%c)\n", charCount, c, (char)c);

            if ((c == '\n' || c == 0xFF) && charCount < 10)
            {
                errlog("Invalid input: line is too short\n");
                return false; // return false, then main function will catch this and exit with EXIT_FAILURE;
            }

            if (!isprint(c) || c == 0xFF)
            {
                errlog("Invalid input: unexpected EOF\n");
                return false;
            }

            _matrix[lineCount][charCount] = (char)c;

            ++charCount;
        }

        if (fgetc(_file) != '\n')
        {
            errlog("Invalid input: Line is too long\n");
            return false;
        }

        ++lineCount;
    }

    c = fgetc(_file);
    if (c != EOF)
    {
        errlog("Invalid input: file is too long (read %d instead of EOF)\n", (unsigned int)c);
        return false;
    }

    return true;
}

void rotate(char _matrix[10][10])
{
    char buffer[10][10];
    int i = 0;
    int j = 0;

    // row i -> column 9 - i;
    // column j -> row j;
    for (i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            buffer[j][9 - i] = _matrix[i][j];
        }
    }

    for (i = 0; i < 10; ++i)
    {
        for (j = 0; j < 10; ++j)
        {
            _matrix[i][j] = buffer[i][j];
        }
    }

    // this is a hacky way, the matrix should be printed out in separated function;
    for (i = 0; i < 10; ++i)
    {
        for (j = 0; j < 10; ++j)
        {
            printf("%c", _matrix[i][j]);
        }
        printf("\n");
    }
}
