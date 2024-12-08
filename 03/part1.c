#include "stdio.h"
#include "stdlib.h"

#define IO_BUF_SIZE 1024

int main(int argc, char *argv[])
{
    char *filepath = "./in.txt";
    if (argc > 1)
    {
        filepath = argv[1];
    }

    FILE *file = fopen(filepath, "r");
    if (!file)
    {
        printf("Couldn't open file with path '%s'\n", filepath);
    }

    size_t bufSize = IO_BUF_SIZE;
    char *in = malloc(sizeof(char) * bufSize);
    size_t cursor = 0;
    int amountRead;
    while ((amountRead = fread(in + cursor, sizeof(char), bufSize - cursor, file)) > 0)
    {
        cursor += amountRead;
        if (cursor >= bufSize)
        {
            bufSize *= 2;
            in = realloc(in, sizeof(char) * bufSize);
        }
    }

    long result = 0;
    int a, b, s;
    char end;
    for (size_t i = 0; i < cursor; i++)
    {
        s = sscanf(in + i, "mul(%d,%d%c", &a, &b, &end);
        if (s == 3 && end == ')')
        {
            printf("%d %d\n", a, b);
            result += a * b;
        }
        else if (s < 0)
        {
            break;
        }
    }

    printf("Sum of muls: %ld\n", result);
    return 0;
}
