// ===================================================== //
// Please note this is a VERY incomplete implementation. //
// There is no debugger or any (meaningful) insight into //
// runtime behavior.                                     //
// Use an online interpreter for visuals.                //
// ===================================================== //

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define DEFAULT_TAPE_SIZE 1024
#define IO_BUF_SIZE 1024
#define TAPE_OUTPUT 32
#define ROW_LEN 8

typedef unsigned short tape_cell;
#define TAPE_CELL_FORMAT "%hu"

size_t jumpMatching(size_t instruction, char *prog, size_t progLen)
{
    int dir = 1;
    size_t count = 1;
    if (prog[instruction] == ']')
    {
        dir = -1;
        count = -1;
    }

    while (instruction < progLen && count != 0)
    {
        instruction += dir;

        switch (prog[instruction])
        {
        case '[':
            ++count;
            break;
        case ']':
            --count;
            break;
        }

        if (count == 0)
        {
            break;
        }
    }

    if (instruction >= progLen)
    {
        fprintf(stderr, "[ERR] Failed to jump to matching bracket.\n");
    }

    return instruction;
}

int brainfuck(char *prog, size_t progLen, size_t tapeSize)
{
    int status = 0;

    size_t pos = 0;
    tape_cell *tape = malloc(sizeof(tape_cell) * tapeSize);
    memset(tape, 0, sizeof(tape_cell) * tapeSize);

    // Current instruction position.
    size_t instruction = 0;

    printf("[INPUT/OUTPUT]\n");

    // Main loop
    while (1)
    {
        // Note that pos is unsigned so we don't need to check for going under
        if (pos >= tapeSize)
        {
            printf("\n[ERR] Overflowed tape bounds.\n");
            status = -10;
            break;
        }

        // If the instruction pointer has overflowed the program we terminate
        if (instruction >= progLen)
        {
            break;
        }

        // Matching on instruction
        switch (prog[instruction])
        {
        case '+':
            tape[pos] += 1;
            ++instruction;
            break;
        case '-':
            tape[pos] -= 1;
            ++instruction;
            break;
        case '>':
            ++pos;
            ++instruction;
            break;
        case '<':
            --pos;
            ++instruction;
            break;
        case '.':
            putchar(tape[pos]);
            ++instruction;
            break;
        case ',':
            tape[pos] = getchar();
            ++instruction;
            break;
        case '[':
            // This isn't really in the scope of the project, but you could compute all of the jumps
            // before execution and then just do a lookup instead of this linear algorithm.
            // jumpMatching is O(n) where n is the program length.
            // You could very easily make it O(1) because the program is constant.
            if (tape[pos] == 0)
            {
                instruction = jumpMatching(instruction, prog, progLen);
            }
            else
            {
                ++instruction;
            }
            break;
        case ']':
            if (tape[pos] != 0)
            {
                instruction = jumpMatching(instruction, prog, progLen);
            }
            else
            {
                ++instruction;
            }
            break;

        default:
            printf("[ERR] Unrecognized instruction '%c'.\n", prog[instruction]);
            status = -15;
            break;
        }

        if (status != 0)
        {
            break;
        }
    }

    printf("\n[END OF EXECUTION]\n");

    // Print out the first TAPE_OUTPUT values in the tape
    for (size_t i = 0; i < TAPE_OUTPUT; i++)
    {
        if (i != 0 && i % ROW_LEN == 0)
        {
            putchar('\n');
        }

        if (i >= tapeSize)
        {
            printf("[End of tape]");
            break;
        }

        printf(TAPE_CELL_FORMAT, tape[i]);
        putchar(' ');
    }

    return status;
}

int isBFChar(char c)
{
    switch (c)
    {
    case '>':
    case '<':
    case '+':
    case '-':
    case '[':
    case ']':
    case '.':
    case ',':
        return 1;

    default:
        return 0;
    }
}

int main(int argc, char *argv[])
{
    // Dropping the first argument (the path of this load module)
    argc -= 1;
    argv += 1;

    if (argc == 0 || argc > 2)
    {
        printf("Expected 1 or 2 command line arguments. (Got %d)\n", argc);
        printf("Usage: brainfuck.exe file.bf [tape-size (default=%d)]\n", DEFAULT_TAPE_SIZE);
        return 1;
    }

    char *filepath = argv[0];

    int tapeSize = DEFAULT_TAPE_SIZE;
    if (argc == 2)
    {
        tapeSize = atoi(argv[1]);
    }

    FILE *file = fopen(filepath, "r");
    if (file == NULL)
    {
        printf("Couldn't open file with path %s\n", filepath);
        return -1;
    }

    // Reading file into program buffer
    size_t progBufSize = IO_BUF_SIZE;
    char *prog = malloc(sizeof(char) * IO_BUF_SIZE);
    char *in = malloc(sizeof(char) * IO_BUF_SIZE);

    size_t cursor = 0;
    size_t amountRead;
    while ((amountRead = fread(in, sizeof(char), IO_BUF_SIZE, file)) > 0)
    {
        for (size_t i = 0; i < amountRead; i++)
        {
            char c = in[i];
            // Skip any non-brainfuck characters
            if (!isBFChar(c))
            {
                continue;
            }

            // If we've ran out of space, allocate more space.
            if (cursor >= progBufSize)
            {
                progBufSize *= 2;
                prog = realloc(prog, sizeof(char) * progBufSize);
            }

            prog[cursor] = c;
            ++cursor;
        }
    }

    printf("[PROGRAM (len=%ld)]\n", cursor);
    for (size_t i = 0; i < cursor; i++)
    {
        putchar(prog[i]);
    }

    putchar('\n');

    return brainfuck(prog, cursor, tapeSize);
}