#include "parse.h"

unsigned int len_fns()
{
    unsigned int count = 0, index = 0;
    while (fns[index++] != NULL && ++count) {}
    return count;
}

int is_redirected(const char* command)
{
    unsigned int len_cmd = strlen(command) + 1;
    for (unsigned int i = 0; i < len_cmd; i++)
        if (command[i] == '>')
            return 0;
    return -1;
}

int is_valid_redir(const char* command)
{
    unsigned int arrow_count = 0;
    unsigned int size = strlen(command) + 1;

    // The minimum size for a valid redirection command is 4.
    if (size < 4) return -1;

    // The first and last characters must not be '>'.
    if (command[0] == '>' || command[size - 2] == '>')
        return -1;

    for (unsigned int i = 0; i < size; i++)
        if (command[i] == '>')
            arrow_count++;
    return arrow_count != 1 ? -1 : 0;
}

void parse_normal(const char* command, t_arg* arg)
{
    char* dup_cmd = strdup(command);
    char* record = dup_cmd;
    char* token;
    while ((token = strtok_r(dup_cmd, " \t", &dup_cmd)))
        arg_add(arg, token);
    free(record);
}

void parse_redirection(const char* command, t_arg* arg)
{
    char* dup_cmd = strdup(command);
    char* record = dup_cmd;  // record for free() later
    char* exe_cmd = strtok_r(dup_cmd, ">", &dup_cmd);
    char* redir = strtok_r(dup_cmd, ">", &dup_cmd);
    char* redir_file = strtok_r(redir, " \t", &redir);
    char* token;
    int n_invalid = 0;

    while ((token = strtok_r(redir, " \t", &redir)) && ++n_invalid) {}

    char* exe_test = strdup(exe_cmd);
    char* exe_record = exe_test;  // record for free() later
    // Check for all whitespaces/tabs case
    token = strtok_r(exe_test, " \t", &exe_test);
    if (token == NULL || redir_file == NULL || n_invalid != 0)
        error("Redirection misformatted.\n");
    else
    {
        parse_normal(exe_cmd, arg);
        arg_add_redir_file(arg, redir_file);
    }
    free(record);
    free(exe_record);
}

void parse(const char* command, t_arg* arg)
{
    char* dup_cmd = strdup(command);
    char* record = dup_cmd;
    // Check for all whitespaces/tabs case
    char* exe = strtok_r(dup_cmd, " \t", &dup_cmd);
    if (exe == NULL)
    {
        free(record);
        return;
    }
    free(record);

    if (is_redirected(command) == 0)
    {
        if (is_valid_redir(command) == 0)
            parse_redirection(command, arg);
        else
            error("Redirection misformatted.\n");
    }
    else
        parse_normal(command, arg);
}
