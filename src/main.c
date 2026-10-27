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

//#include "../inc/walk.h"
#include "../inc/manifest.h"


#define NAME "mangen"

#ifndef GIT_COMMIT
#define GIT_COMMIT "unknown"
#endif

#ifndef GIT_AUTHOR
#define GIT_AUTHOR "unknown"
#endif

// Вывод версии при флаге (-v)
static void print_version()
{

    printf("mangen v1.0\n");
    printf("Author: %s\n", GIT_AUTHOR);
    printf("Git commit: %s\n", GIT_COMMIT);
            
    
}        
            
        

/**
 * @brief Выводит справку по использованию утилиты в stdout.
 */

// Вывод справки при флаге (-h)
void helper(){
    printf(
        "Usage: mangen [OPTION]... [DIR_PATH]\n"
        "Generate a SHA-256 manifest of files in DIR_PATH (default: current directory).\n"
        "\n"
        "With no DIR_PATH, generates the manifest for the current working directory.\n"
        "Each line of output follows the pattern: <relative_path> : <sha256_hex>\n"
        "\n"
        "Options:\n"
        "  -h                display this help and exit\n"
        "  -v                display git commit hash (version) and exit\n"
        "  -e NAME           exclude all files and directories named NAME\n"
        "  -E PATTERN        exclude files/dirs matching PATTERN\n"
        "                    ('.' matches any single char, '*' matches any sequence)\n"
        "  -i                add integrity check line at the end\n"
        "  --verify          verify manifest integrity from stdin\n"
        "\n"
        "Report bugs to: https://github.com/goga336/mangen/issues\n"
        "\n"
        "Exit status:\n"
        " 0  if OK,\n"
        " 1  if minor problems (e.g., cannot access subdirectory),\n"
        " 2  if serious trouble (e.g., cannot access command-line argument).\n"
    );
}

/**
 * @brief Главная функция программы.
 */
int main(int argc, char *argv[]) {
    const char *dir = NULL;
    const char *exclude_name = NULL;
    const char *exclude_pattern = NULL;
    int check_integrity = 0;
    int verify = 0;

    for (int i = 1; i < argc; ++i) {
        const char *a = argv[i]; // Текущий аргумент
        if(strcmp(a, "-h") == 0){
            helper(); // Вызов вывода справки
            return 0;
        }
        else if (strcmp(a, "-e") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "mangen: -e requires an argument\n");
                return 2;
            }
            exclude_name = argv[++i]; // Принимаем следующий аргумент полсе флага -e
        }
        else if (strcmp(a, "-E") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "mangen: -E requires an argument\n");
                return 2;
            }
            exclude_pattern = argv[++i]; // Принимаем следующий аргумент полсе флага -E
        }
        else if (strcmp(a, "-v") == 0) {
            print_version(); // Вызов вывода версии
            return 0;
        }
        else if (strcmp(a, "-i") == 0) {
            check_integrity = 1; // Поднятие флага добавления хэша всего манифеста
        }
        else if (strcmp(a, "--verify") == 0) {
            verify = 1; // // Поднятие флага проверки манифеста
        }
        else if (*a == '-') { // Если встретились другие флаги, то ошибка
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
    // Проверка на режим верификации
    if (verify == 1) {
        return verify_manifest();
    }
    // Если путь не указан, то используется текущий каталог
    if(dir == NULL){
            dir=".";
    }

    // Проверка на существование пути
    struct stat st;
    if (stat(dir, &st) != 0) {
        fprintf(stderr, "mangen: %s: %s\n", dir, strerror(errno));
        return 1;
    }
    if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "mangen: %s: not a directory\n", dir);
        return 1;
    }
    
    // Генерация манифеста
    if (generate_manifest(dir, exclude_name, exclude_pattern, check_integrity) != 0)
        return 1;
    else
        return 0;

}