/**
 * @file main.c
 * @brief Точка входа в утилиту mangen. Парсинг аргументов командной строки.
 * 
 * 
 * Утилита командной строки для генерации манифеста каталога.
 * Обходит указанную директорию рекурсивно и выводит для каждого файла
 * его относительный путь и SHA-256 хэш.
 */

#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <limits.h>

#include "../inc/walk.h"


#define NAME "mangen"

#ifndef GIT_COMMIT
#define GIT_COMMIT "unknown"
#endif

#ifndef GIT_AUTHOR
#define GIT_AUTHOR "unknown"
#endif

static void print_version()
{

    printf("mangen v1.0\n");
    printf("Author: %s\n", GIT_AUTHOR);
    printf("Git commit: %s\n", GIT_COMMIT);
            
    
}        
            
        

/**
 * @brief Выводит справку по использованию утилиты в stdout.
 */
void helper(){
    printf(
        "Usage: mangen [DIR_PATH] [OPTIONS]\n"
        "\n"
        "Generate a manifest of files in DIR_PATH (default: current directory).\n"
        "Each line: <relative_path> : <sha256_hex>\n"
        "\n"
        "Options:\n"
        "  -h                display this help and exit\n"
        "  -e NAME           exclude files and directories named NAME\n"
        "  -E PATTERN        exclude files/dirs matching PATTERN ('.' and '*')\n"
        "  -v                display this git commit hash and exit\n"
    );
}

/**
 * @brief Главная функция программы.
 */
int main(int argc, char *argv[]) {
    const char *dir = NULL;
    const char *exclude_name = NULL;
    const char *exclude_pattern = NULL;
    for (int i = 1; i < argc; ++i) {
        const char *a = argv[i];
        if(strcmp(a, "-h") == 0){
            helper();
            return 0;
        }
        else if (strcmp(a, "-e") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "mangen: -e requires an argument\n");
                return 2;
            }
            exclude_name = argv[++i];
        }
        else if (strcmp(a, "-E") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "mangen: -E requires an argument\n");
                return 2;
            }
            exclude_pattern = argv[++i];
        }
        else if (strcmp(a, "-v") == 0) {
            print_version();
            return 0;
        }
        else if (*a == '-') {
            fprintf(stderr, "%s: invalid option '%s'\"\n"
            "Try %s -h for more information.\n", NAME, a, NAME);
            return 2;
        }
        else{
            if (dir != NULL) {
                fprintf(stderr, "error: too many arguments\n");
                return 2;
            }
            dir = a;
        }
        
        
    }
    if(dir == NULL){
            dir=".";
        }

    struct stat st;
    if (stat(dir, &st) != 0) {
    fprintf(stderr, "mangen: %s: %s\n", dir, strerror(errno));
    return 1;
    }
    if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "mangen: %s: not a directory\n", dir);
        return 1;
    }

    //hash_file(dir);
    if(recursive_walk(dir, "", exclude_name, exclude_pattern))
        return 1;
    else
        return 0;

}