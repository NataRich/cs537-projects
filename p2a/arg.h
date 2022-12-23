#ifndef ARG_HEADER_H
#define ARG_HEADER_H

#include <stdlib.h>
#include <string.h>

/** @struct t_arg
 * This structure contains essential elements parsed from the command line 
 * input.
 *
 * @var t_arg::argv
 * The argument vector that stores both the executable and options.
 * @var t_arg::argc
 * The argument count that counts the number of elements in `argv`.
 * @var t_arg::redir_file
 * The file (name) to which the output is redirected.
 */
typedef struct argument
{
	char** argv;
	unsigned int argc;
	char* redir_file;
} t_arg;

/**
 * Initializes an argument structure (dynamically allocated).
 *
 * @return A dynamically allocated `t_arg` pointer.
 */
t_arg* arg_init();

/**
 * Clones an argument structure (dynamically allocated).
 *
 * This function deep-copies the given argument structure.
 *
 * @param arg The argument structure to be cloned.
 * @return A dynamically allocated deep copy of `t_arg` pointer.
 */
t_arg* arg_clone(const t_arg* arg);

/**
 * Concatenates the argument vectors of `arg1` and `arg2` to produce a new
 * argument structure (dynamically allocated). Importantly, this function does
 * NOT copy the redirection file from any given argument structure, and it will
 * ignore the first argument of `arg2` (i.e., the executable of `arg2`).
 *
 * @param arg1 The argument structure at the front.
 * @param arg2 The argument structure at the back.
 * @return A dynamically allocated deep copy of both `t_arg` pointers.
 */
t_arg* arg_cat(const t_arg* arg1, const t_arg* arg2);

/**
 * Adds a new element to the argument vector.
 *
 * This function not only appends the new element at the end of the argument
 * vector but increments the argument count by one.
 *
 * @param arg The argument structure to which the new element is added.
 * @param e The new element to be added.
 */
void arg_add(t_arg* arg, const char* e);

/**
 * Adds the redirection file to the argument structure.
 *
 * @param arg The argument structure to which the redirection file is added.
 * @param redir_file The redirection file to be added.
 */
void arg_add_redir_file(t_arg* arg, const char* redir_file);

/**
 * Frees the dynamically allocated `t_arg` pointer.
 *
 * @param arg The argument structure to be freed.
 */
void arg_free(t_arg* arg);

/**
 * Converts the argument vector to a string.
 *
 * This function will concatenate all the elements in the given argument vector
 * with one whitespace added between two consecutive elements and return the
 * resultant string. Importantly, the resultant string is dynmically allocated.
 *
 * @param argv The argument vector to be converted.
 * @param argc The number of arguments in the argument vector.
 * @return The string version of the given argument vector.
 */
char* argv_to_str(char** const argv, unsigned int argc);

#endif
