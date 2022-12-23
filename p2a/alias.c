#include "alias.h"

t_amap* amap_init()
{
    t_amap* map = malloc(sizeof(t_amap));
    if (map == NULL) exit(1);

    map->keys = NULL;
    map->args = NULL;
    map->values = NULL;
    map->size = 0;
    return map;
}

char* amap_find_value(const t_amap* map, const char* key)
{
    for (unsigned int i = 0; i < map->size; i++)
    {
        unsigned int key_size = strlen(key) + 1;
        if (strncmp(key, map->keys[i], key_size) == 0)
            return strndup(map->values[i], strlen(map->values[i]));
    }
    return NULL;
}

t_arg* amap_find_arg(const t_amap* map, const char* key)
{
    for (unsigned int i = 0; i < map->size; i++)
    {
        unsigned int key_size = strlen(key) + 1;
        if (strncmp(key, map->keys[i], key_size) == 0)
            return arg_clone(map->args[i]);
    }
    return NULL;
}

void amap_print_all(const t_amap* map)
{
    for (unsigned int i = 0; i < map->size; i++)
        info("%s %s\n", map->keys[i], map->values[i]);
}

void amap_print(const t_amap* map, const char* key)
{
    char* value = amap_find_value(map, key);
    if (value != NULL)
    {
        info("%s %s\n", key, value);
        free(value);
    }
}

void amap_add(t_amap* map, const char* key, char** const argv, unsigned int argc)
{
    for (unsigned int i = 0; i < 3; i ++)
    {
        if (strncmp(key, fns[i], strlen(key) + 1) == 0)
        {
            error("alias: Too dangerous to alias that.\n");
            return;
        }
    }

    for (unsigned int i = 0; i < map->size; i++)
    {
        if (strncmp(key, map->keys[i], strlen(key) + 1) == 0)
        {
            free(map->values[i]);
            map->values[i] = argv_to_str(argv, argc);

            arg_free(map->args[i]);
            map->args[i] = arg_init();
            for (unsigned int j = 0; j < argc; j++)
                arg_add(map->args[i], argv[j]);
            return;
        }
    }

    unsigned int tail = map->size++;
    map->keys = realloc(map->keys, map->size * sizeof(char*));
    if (map->keys == NULL) exit(1);

    map->values = realloc(map->values, map->size * sizeof(char*));
    if (map->values == NULL) exit(1);

    map->args = realloc(map->args, map->size * sizeof(t_arg*));
    if (map->args == NULL) exit(1);

    unsigned int key_size = strlen(key) + 1;
    map->keys[tail] = strndup(key, key_size);
    map->values[tail] = argv_to_str(argv, argc);
    map->args[tail] = arg_init();
    for (unsigned int i = 0; i < argc; i++)
        arg_add(map->args[tail], argv[i]);
}


void amap_remove(t_amap* map, const char* key)
{
    char* value = amap_find_value(map, key);

    if (value != NULL)
    {
        free(value);
        unsigned int size = map->size - 1;
        char** keys = malloc(size * sizeof(char*));
        if (keys == NULL) exit(1);

        char** values = malloc(size * sizeof(char*));
        if (values == NULL) exit(1);

        t_arg** args = malloc(size * sizeof(t_arg*));
        if (args == NULL) exit(1);

        for (unsigned int i = 0, j = 0; i < map->size; i++)
        {
            if (strncmp(key, map->keys[i], strlen(key) + 1) == 0)
                continue;

            unsigned int key_size = strlen(map->keys[i]) + 1;
            unsigned int val_size = strlen(map->values[i]) + 1;
            keys[j] = strndup(map->keys[i], key_size);
            values[j] = strndup(map->values[i], val_size);
            args[j] = arg_clone(map->args[i]);
            j++;

            free(map->keys[i]);
            free(map->values[i]);
            arg_free(map->args[i]);
        }

        free(map->keys);
        map->keys = keys;
        free(map->values);
        map->values = values;
        free(map->args);
        map->args = args;

        map->size--;
    }
}

void amap_free(t_amap* map)
{
    for (unsigned int i = 0; i < map->size; i++)
    {
        free(map->keys[i]);
        free(map->values[i]);
        arg_free(map->args[i]);
    }

    if (map->keys != NULL) free(map->keys);
    if (map->values != NULL) free(map->values);
    if (map->args != NULL) free(map->args);
    free(map);
    map = NULL;
}
