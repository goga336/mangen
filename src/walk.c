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
    DIR *d = opendir(dir);
    if(d == NULL){
        fprintf(stderr, "mangen: %s: %s\n", dir, strerror(errno));
        return -1;
    }
    struct dirent *entry;
   
    while ((entry = readdir(d)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        if (exclude_file != NULL && strcmp(entry->d_name, exclude_file) == 0)
            continue;

        if (exclude_pattern != NULL && regexp_match(exclude_pattern, entry->d_name))
            continue;
        char real_path[PATH_MAX];
        char rel_path[PATH_MAX];

        snprintf(real_path, sizeof(real_path), "%s/%s", dir, entry->d_name);
        if (rel_prefix[0] == '\0')
            snprintf(rel_path, sizeof(rel_path), "%s", entry->d_name);
        else
            snprintf(rel_path, sizeof(rel_path), "%s/%s", rel_prefix, entry->d_name);
         
        struct stat st;
        if (lstat(real_path, &st) != 0) {
            fprintf(stderr, "mangen: %s: %s\n", real_path, strerror(errno));
            continue;
        }
        if (S_ISDIR(st.st_mode))
            recursive_walk(real_path, rel_path, exclude_file, exclude_pattern);          /* рекурсия */
        else if (S_ISREG(st.st_mode))
            hash_file(real_path,rel_path);
    }
    closedir(d);
    return 0;

}