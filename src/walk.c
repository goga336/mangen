/**
 * @file walk.c
 * @brief Реализация рекурсивного обхода директорий.
 */
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include "../inc/walk.h"
#include "../inc/hash.h"
#include "../inc/regexp.h"
/**
 * @copydoc recursive_walk
 */

int recursive_walk(const char *dir,  const char *rel_prefix, const char *exclude_file, const char *exclude_pattern){
    DIR *d = opendir(dir); // Открытие каталога
    if(d == NULL){
        fprintf(stderr, "mangen: %s: %s\n", dir, strerror(errno));
        return -1;
    }
    struct dirent *entry;
   // Проход по всем записям каталога
    while ((entry = readdir(d)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) // Пропуск ссылок на текущий и родительский каталог
            continue;
        if (exclude_file != NULL && strcmp(entry->d_name, exclude_file) == 0) // Пропуск, если имя совпадает с исключаемым файлом -e
            continue;

        if (exclude_pattern != NULL && regexp_match(exclude_pattern, entry->d_name)) // Пропуск, если имя совпадает с регулярным выражением -E
            continue;
        char real_path[PATH_MAX];
        char rel_path[PATH_MAX];

         // Формирование полного пути для доступа к файлу
        snprintf(real_path, sizeof(real_path), "%s/%s", dir, entry->d_name);

        // Формирование относительного пути
        if (rel_prefix[0] == '\0')
            snprintf(rel_path, sizeof(rel_path), "%s", entry->d_name);
        else
            snprintf(rel_path, sizeof(rel_path), "%s/%s", rel_prefix, entry->d_name);
        
            // Получение информации о файле
        struct stat st;
        if (lstat(real_path, &st) != 0) {
            fprintf(stderr, "mangen: %s: %s\n", real_path, strerror(errno));
            continue;
        }
        // Если это каталог, то выполняется рекурсивный вызов
        if (S_ISDIR(st.st_mode))
            recursive_walk(real_path, rel_path, exclude_file, exclude_pattern);          /* рекурсия */
        else if (S_ISREG(st.st_mode)) // // Если это обычный файл, то вычисляется и выводится его хэш
            hash_file(real_path,rel_path);
    }
    closedir(d); // Закрытие каталога
    return 0;

}