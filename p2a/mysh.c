#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "alias.h"
#include "parse.h"
#include "utils.h"

// Builtin function names (also names that cannot be aliased)
const char* fns[] = { "alias", "unalias", "exit", NULL };

// Forward declaration
void execute(const t_arg* arg, t_amap* map);

int main(int argc, char* argv[])
{
    // Invalid number of arguments
    if (argc > 2)
    {
        error("Usage: mysh [batch-file]\n");
        return 1;
    }

    // Check if input is a valid file, and open it if so
    FILE* fptr = stdin;
    if (argc == 2)
    {
        fptr = fopen(argv[1], "r");
        if (fptr == NULL)
        {
            error("Error: Cannot open file %s.\n", argv[1]);
            return 1;
        }
    }

    // Initialize
    t_arg* arg = arg_init();
    t_amap* map = amap_init();
    char line[kBufferSize] = { 0 };
    do
    {
        arg_free(arg);
        arg = arg_init();

        // Print prompt only in interactive mode
        if (fptr == stdin) info("mysh> ");
        if (fgets(line, kBufferSize, fptr) == NULL) break;
        // Echo command only in batch mode
        if (fptr != stdin) info("%s", line);

        // Overwrite new line character for easy parsing
        line[strlen(line) - 1] = 0;
        parse(line, arg);
        // Ignore empty command
        if (arg->argc == 0) continue;
        // Exit shell when command is exactly "exit"
        if (strncmp("exit", arg->argv[0], 5) == 0) break;
        // Execute non-empty command
        execute(arg, map);
    } while (1);

    // Cleanup
    arg_free(arg);
    amap_free(map);
    if (fptr != stdin)
        fclose(fptr);
    return 0;
}

void execute(const t_arg* arg, t_amap* map)
{
    char* exe = arg->argv[0];
    unsigned int argc = arg->argc;

    if (strncmp("alias", exe, 6) == 0)
    {
        // Print all aliases
        if (argc == 1)
            amap_print_all(map);
        // Print specific alias
        else if(argc == 2)
            amap_print(map, arg->argv[1]);
        // Add alias
        else
            amap_add(map, arg->argv[1], &arg->argv[2], argc - 2);
    }
    else if (strncmp("unalias", exe, 8) == 0)
    {
        if (argc != 2)
            error("unalias: Incorrect number of arguments.\n");
        else
            amap_remove(map, arg->argv[1]);
    }
    else
    {
        int pid = fork();
        if (pid < 0)
            error("Error: Failed to fork.\n");
        // In parent proc
        else if (pid > 0)
            waitpid(pid, NULL, 0);
        // In child proc
        else
        {
            // Set up redirection
            if (arg->redir_file != NULL)
            {
                FILE* fp = fopen(arg->redir_file, "w");
                if (fp == NULL)
                {
                    error("Cannot write to file %s.\n", arg->redir_file);
                    _exit(0);
                }
                dup2(fileno(fp), STDOUT_FILENO);
            }

            // Look for alias first
            t_arg* alias_arg = amap_find_arg(map, exe);
            // Is not an alias, then execute directly
            if (alias_arg == NULL)
            {
                execv(exe, arg->argv);
                error("%s: Command not found.\n", exe);
                _exit(0);
            }
            // Is an alias, then concat argv before executing
            else
            {
                t_arg* comb = arg_cat(alias_arg, arg);
                execv(comb->argv[0], comb->argv);
                error("%s: Command not found.\n", comb->argv[0]);
                arg_free(comb);
                _exit(0);
            }
        }
    }
}
