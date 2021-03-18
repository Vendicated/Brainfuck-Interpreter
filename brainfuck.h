#include <stdio.h>
#include <stdlib.h>

typedef struct BRAINFUCK
{
    char *ptr;
    const char *cursor;
} BRAINFUCK;

static struct BRAINFUCK handle;

BRAINFUCK *brainfuck_init()
{
    char arr[30000] = {0};
    handle.ptr = arr;
    return &handle;
}

void brainfuck_loads(BRAINFUCK *handle, char *content)
{
    handle->cursor = content;
}

int brainfuck_loadf(BRAINFUCK *handle, FILE *fp)
{
    char *content = NULL;
    int cont_size, read_size, res = 0;

    fseek(fp, 0, SEEK_END);
    cont_size = ftell(fp);
    rewind(fp);

    content = (char *)malloc(sizeof(char) * (cont_size + 1));

    read_size = fread(content, sizeof(char), cont_size, fp);

    if (read_size != cont_size)
    {
        // Something went wrong while reading file
        free(content);
        content = NULL;
        res = 1;
    }

    handle->cursor = content;

    return res;
}

int brainfuck_validate(BRAINFUCK *handle)
{
    // Save pointers
    const char *cursor = handle->cursor;
    char *ptr = handle->ptr;

    // Set to 0
    handle->cursor = 0;
    handle->ptr = 0;

    int depth = 0;
    while (*cursor)
    {
        if (*cursor == '[')
            depth++;
        else if (*cursor == ']')
            depth--;

        if (depth < 0)
            break;

        cursor++;
    }

    handle->cursor = cursor;
    handle->ptr = ptr;

    return depth == 0 ? 0 : 1;
}

void _brainfuck_go_to_closing_bracket(BRAINFUCK *handle)
{
    while (1)
    {
        handle->cursor++;
        if (*handle->cursor == '[')
            _brainfuck_go_to_closing_bracket(handle);
        else if (*handle->cursor == ']')
            break;
    }
}

void _brainfuck_go_to_opening_bracket(BRAINFUCK *handle)
{
    int depth = 0;
    do
    {
        if (*handle->cursor == ']')
            depth++;
        else if (*handle->cursor == '[')
            depth--;
        handle->cursor--;
    } while (depth);
}

void brainfuck_evaluate(BRAINFUCK *handle)
{
    while (*handle->cursor)
    {
        switch (*handle->cursor)
        {
        case '>':
            handle->ptr++;
            break;
        case '<':
            handle->ptr--;
            break;
        case '+':
            (*handle->ptr)++;
            break;
        case '-':
            (*handle->ptr)--;
            break;
        case '[':
            if (*handle->ptr == '\0')
                _brainfuck_go_to_closing_bracket(handle);
            break;
        case ']':
            _brainfuck_go_to_opening_bracket(handle);
            break;
        case '.':
            putchar(*handle->ptr);
            break;
        case ',':
            *handle->ptr = getchar();
            break;
        }
    }

    handle->cursor++;
}