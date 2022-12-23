#ifndef PARSE_HEADER_H
#define PARSE_HEADER_H

#include <stdlib.h>
#include <string.h>

#include "arg.h"
#include "utils.h"

/**
 * Builtin functions' names for this shell (e.g. alias, unalias, exit).
 *
 * The external `fns` must be NULL terminated, e.g. `{"alias", NULL}`.
 * Otherwise, it may lead to undefined behaviors when using related functions.
 */
extern const char* fns[];

/**
 * Gets the number of builtin functions.
 *
 * This function counts the number of strings until the first NULL mark. For
 * example, calling this function when `fns` is defined as `{"alias", NULL}`
 * will return 1.
 *
 * @return The number of builtin functions excluding the final NULL mark.
 */
unsigned int len_fns();

/**
 * Checks if the command requires redirection.
 *
 * This function checks if the command contains any redirection character,
 * i.e., '>'. It doesn't check if it is a valid redirection action.
 *
 * @param command The command line input.
 * @return 0 if the given command contains '>', -1 if it doesn't.
 */
int is_redirected(const char* command);

/**
 * Checks if the redirection command has a valid from.
 *
 * This function checks if the command has '>' character at the beginning or
 * the end of the string and if the number of '>' character is not 1. In above
 * situations, it returns -1. Otherwise, it returns 0. Importantly, even if it
 * returns 0, it is not guaranteed that the command is indeed a valid
 * redirection. It is only guaranteed that the command has a valid form of a
 * redirection.
 *
 * @param command The command line input.
 * @return 0 if the given command is of a valid form, -1 otherwise.
 */
int is_valid_redir(const char* command);

/**
 * Parses the command that doesn't involve redirection.
 *
 * This function treats only whitespaces as delimiters. It will store the
 * executable and options in the `t_arg` data strcuture.
 *
 * @param command The command line input.
 * @param arg Save the parsed parts of the command.
 */
void parse_normal(const char* command, t_arg* arg);

/**
 * Parses the command that involves redirection.
 *
 * This function treats both whitespaces and right arrows as delimiters. It
 * will store the executable, options and redirected files in the `t_arg` data
 * structure.
 *
 * @param command The command line input.
 * @param arg Save the parsed parts of the command.
 */
void parse_redirection(const char* command, t_arg* arg);

/**
 * Parses any command.
 *
 * @param command The command line input.
 * @param arg Save the parsed parts of the command.
 */
void parse(const char* command, t_arg* arg);

#endif
