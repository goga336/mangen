/**
 * @file regexp.c
 * @brief Реализация поиска по маске.
 */
#include <stdlib.h>
#include "../inc/regexp.h"

int regexp_match(const char *pattern, const char *name)
{
    // Указатели для отката, если встретим '*'
    const char *star_pattern = NULL;
    const char *star_name = NULL;

    // Прохождение по строке
    while (*name) {
        if (*pattern == '.' || *pattern == *name) {
            pattern++;
            name++;
        } else if (*pattern == '*') { // Встретили *, сохраняется позиция и выполняется переход к следующему символу паттерна
            star_pattern = pattern++;
            star_name = name;
        } else if (star_pattern != NULL) { // // При несовпадении выполняется возврат к сохраненной позиции символа *
            pattern = star_pattern + 1;
            name = ++star_name;
        } else {
            return 0;
        }
    }

    // Пропуск оставшихся символов *
    while (*pattern == '*') {
        pattern++;
    }

    // Возвращается 1, если паттерн обработан, иначе 0
    return *pattern == '\0';
}