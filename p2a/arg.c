#include "arg.h"

t_arg* arg_init()
{
    t_arg* arg = malloc(sizeof(t_arg));
    if (arg == NULL) exit(1);

    arg->argv = malloc(sizeof(char*));
    if (arg->argv == NULL) exit(1);

    arg->argv[0] = NULL;
    arg->argc = 0;
    arg->redir_file = NULL;
    return arg;
}

t_arg* arg_clone(const t_arg* arg)
{
    t_arg* copy = arg_init();
    for (unsigned int i = 0; i < arg->argc; i++)
        arg_add(copy, arg->argv[i]);
    if (arg->redir_file != NULL)
        arg_add_redir_file(copy, arg->redir_file);
    return copy;
}

t_arg* arg_cat(const t_arg* arg1, const t_arg* arg2)
{
    t_arg* comb = arg_init();
    for (unsigned int i = 0; i < arg1->argc; i++)
        arg_add(comb, arg1->argv[i]);
    for (unsigned int i = 1; i < arg2->argc; i++)
        arg_add(comb, arg2->argv[i]);
    return comb;
}

void arg_add(t_arg* arg, const char* e)
{
    arg->argc++;
    arg->argv = realloc(arg->argv, (arg->argc + 1) * sizeof(char*));
    if (arg->argv == NULL) exit(1);

    arg->argv[arg->argc - 1] = strdup(e);
    arg->argv[arg->argc] = NULL;
}

void arg_add_redir_file(t_arg* arg, const char* redir_file)
{
    arg->redir_file = strdup(redir_file);
}

void arg_free(t_arg* arg)
{
    if (arg->redir_file != NULL)
        free(arg->redir_file);
    for (unsigned int i = 0; i < arg->argc; i++)
        free(arg->argv[i]);
    free(arg->argv);
    free(arg);
    arg = NULL;
}

char* argv_to_str(char** const argv, unsigned int argc)
{
    char* str = NULL;
    for (unsigned int i = 0; i < argc; i++)
    {
        unsigned int total_size = 0;
        if (str == NULL)
            total_size = strlen(argv[i]) + 1;
        else
            total_size = strlen(str) + strlen(argv[i]) + 1;

        // Additional space allocated for a whitespace
        if (i + 1 != argc)
            total_size += 1;

        if (str == NULL)
            str = calloc(total_size, sizeof(char));
        else
            str = realloc(str, total_size * sizeof(char));
        if (str == NULL) exit(1);

        strncat(str, argv[i], strlen(argv[i]));

        // Add a whitespace in between two arguments
        if (i + 1 != argc)
            strncat(str, " ", 2);
    }

    return str;
}
