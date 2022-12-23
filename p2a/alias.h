#ifndef ALIAS_HEADER_H
#define ALIAS_HEADER_H

#include <stdlib.h>
#include <string.h>

#include "arg.h"
#include "utils.h"

/**
 * Builtin functions' names for this shell (e.g. alias, unalias, exit).
 *
 * The external `fns` are builtin function names which should never be aliased.
 * It must be NULL terminated, e.g. `{"alias", NULL}`. Otherwise, it may lead
 * to undefined behaviors when using related functions.
 */
extern const char* fns[];

/** @struct t_amap
 * This structure contains the mapping relations between aliases and values.
 *
 * @var t_amap::keys
 * The keys (aliases).
 * @var t_amap::args
 * The commands represented by the key.
 * @var t_amap::values
 * The stringified argument structures.
 * @var t_amap::size
 * The number of keys stored.
 */
typedef struct alias_map
{
	char** keys;
	t_arg** args;
	char** values;
	unsigned int size;
} t_amap;

/**
 * Initializes an alias map structure (dynamically allocated).
 *
 * @return A dynamically allocated `t_amap` pointer.
 */
t_amap* amap_init();

/**
 * Finds the value (stringified argument structure) of the corresponding key.
 *
 * This function searches for the key in the map. If found, it returns a deep
 * copy of the corresponding value. Otherwise, it returns NULL. Importantly,
 * the return value is dynamically allocated.
 *
 * @param map The alias map structure that contains the information.
 * @param key The key to search for in the map.
 * @return The corresponding value.
 */
char* amap_find_value(const t_amap* map, const char* key);

/**
 * Finds the argument structure of the corresponding key.
 *
 * This function searches for the key in the map. If found, it returns a deep
 * copy of the corresponding argument structure. Otherwise, it returns NULL.
 * Importantly, the return value is dynamically allocated.
 *
 * @param map The alias map structure that contains the information.
 * @param key The key to search for in the map.
 * @return The corresponding argument structure.
 */
t_arg* amap_find_arg(const t_amap* map, const char* key);

/**
 * Prints all the aliases stored in the map.
 *
 * This function formats all the aliases and values in the form of
 * `<name> <value>\n` and prints to the standard output.
 *
 * @param map The alias map structure that contains the information to be
 *            printed.
 */
void amap_print_all(const t_amap* map);

/**
 * Prints the specific alias-value pair in the map.
 *
 * This function searches for the given key in the map. If found, it will print
 * the alias-value pair to the standard output in the format of 
 * `<name> <value>\n`. Otherwise, it does nothing.
 *
 * @param map The alias map structure that contains the information to be
 *            printed.
 * @param key The key to be searched for in the map.
 */
void amap_print(const t_amap* map, const char* key);

/**
 * Adds the key-arg (value) pair to the map.
 *
 * This function first checks if the given key is allowed. If allowed, it adds
 * the key-arg pair to the map. Otherwise, it will print an error message to
 * the standard error and do nothing else. If the given key is already present
 * in the map, it will simply replace the old value/argument with the new one.
 * This function also increments the size by one.
 *
 * @param map The alias map structure that contains the information.
 * @param key The key to be added to the map.
 * @param argv The associated argument vector.
 * @param argc The number of arguments in the argument vector.
 */
void amap_add(t_amap* map, const char* key, char** const argv, unsigned int argc);

/**
 * Removes the key-arg (value) pair from the map.
 *
 * This function searches for the given key. If found, it will remove (free)
 * the entry from the map and decrement the size by one. Otherwise, it does
 * nothing.
 *
 * @param map The alias map structure that contains the information.
 * @param key The key to be removed from the map.
 */
void amap_remove(t_amap* map, const char* key);

/**
 * Frees the entire map.
 *
 * @param map The alias map structure to be freed.
 */
void amap_free(t_amap* map);

#endif
