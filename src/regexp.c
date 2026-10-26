/**
 * @file regexp.c
 * @brief Реализация поиска по маске.
 */
#include <stdlib.h>
#include "../inc/regexp.h"

int regexp_match(const char *pattern, const char *name)
{
    
    const char *star_pattern = NULL;
    const char *star_name = NULL;

    while (*name) {
        if (*pattern == '.' || *pattern == *name) {
            pattern++;
            name++;
        } else if (*pattern == '*') {
            star_pattern = pattern++;
            star_name = name;
        } else if (star_pattern != NULL) {
            pattern = star_pattern + 1;
            name = ++star_name;
        } else {
            return 0;
        }
    }

    while (*pattern == '*') {
        pattern++;
    }

    return *pattern == '\0';
}