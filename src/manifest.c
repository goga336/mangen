/**
 * @file manifest.c
 * @brief Реализация генерации манифеста.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../inc/manifest.h"
#include "../inc/walk.h"
#include "rfc6234/sha.h"

int generate_manifest(const char *dir, const char *exclude_name, 
                      const char *exclude_pattern, int check_integrity)
{
    // Если флаг -i не передан, то делаем обход и выходим
    if (!check_integrity) {
        return recursive_walk(dir, "", exclude_name, exclude_pattern);
    }

    // Создание временного файла
    FILE *tmp = tmpfile();
    if (!tmp) {
        fprintf(stderr, "mangen: cannot create temporary file\n");
        return 1;
    }

    // Сохранение текущего stdout и перенаправление его в tmp
    fflush(stdout);
    int saved_stdout = dup(fileno(stdout));
    dup2(fileno(tmp), fileno(stdout));

    // Запуск обхода
    int walk_res = recursive_walk(dir, "", exclude_name, exclude_pattern);

    // Возврат stdout обратно на экран
    fflush(stdout);
    dup2(saved_stdout, fileno(stdout));
    close(saved_stdout);

    if (walk_res != 0) {
        fclose(tmp);
        return 1;
    }

    // tmp содержит весь наш манифест, считаем его SHA-256
    rewind(tmp);
    SHA256Context ctx;
    SHA256Reset(&ctx);

    char buf[8192];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), tmp)) > 0) {
        SHA256Input(&ctx, (uint8_t*)buf, (unsigned int)n);
    }

    uint8_t digest[SHA256HashSize];
    SHA256Result(&ctx, digest);

    // Печать накопленного
    rewind(tmp);
    while ((n = fread(buf, 1, sizeof(buf), tmp)) > 0) {
        fwrite(buf, 1, n, stdout);
    }
    fclose(tmp);

    // Печать строки контроля целостности
    printf("--- integrity: ");
    for (int i = 0; i < SHA256HashSize; i++) {
        printf("%02x", digest[i]);
    }
    printf("\n");

    return 0;
}


int verify_manifest(void)
{
    char line[4096];
    char expected_hash[65] = {0}; // 64 символа хэша + \0
    int found_integrity = 0;

    SHA256Context ctx;
    SHA256Reset(&ctx);

    // Чтение stdin построчно
    while (fgets(line, sizeof(line), stdin)) {
        // Проверка на строку с integrity
        if (strncmp(line, "--- integrity: ", 15) == 0) {
            found_integrity = 1;
            // Копируем хэш (64 символа) из строки
            strncpy(expected_hash, line + 15, 64);
            expected_hash[64] = '\0';
            expected_hash[64] = '\0';
            // Убираем \n в конце
            char *newline = strchr(expected_hash, '\n');
            if (newline) 
                *newline = '\0';
            
            break; 
        } else {
            SHA256Input(&ctx, (uint8_t*)line, (unsigned int)strlen(line));
        }
    }

    if (!found_integrity) {
        fprintf(stderr, "mangen: verify error: no integrity line found\n");
        return 2;
    }

    // Подсчет хэша
    uint8_t digest[SHA256HashSize];
    SHA256Result(&ctx, digest);

    // Конвертация в hex-строку
    char actual_hash[65];
    for (int i = 0; i < SHA256HashSize; i++) {
        sprintf(actual_hash + (i * 2), "%02x", digest[i]);
    }
    actual_hash[64] = '\0';

    if (strcmp(expected_hash, actual_hash) == 0) {
        fprintf(stderr, "mangen: integrity check OK\n");
        return 0; // Целостность соблюдена
    } else {
        fprintf(stderr, "mangen: integrity check FAILED\n");
        return 1; // Ошибка целостности
    }
}