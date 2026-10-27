/**
 * @file manifest.h
 * @brief Генерация манифеста с контролем целостности.
 */
#ifndef MANIFEST_H
#define MANIFEST_H

#include <stdio.h>

/**
 * @brief Генерирует манифест (с опциональным контролем целостности).
 * 
 * @param dir            Каталог для обхода.
 * @param exclude_name   Исключить файлы/каталоги с этим именем (или NULL).
 * @param exclude_pattern Исключить по glob-паттерну (или NULL).
 * @param check_integrity Если 1, добавит строку "--- integrity: <hash>".
 * @return 0 при успехе, 1 при ошибке.
 */
int generate_manifest(const char *dir, const char *exclude_name, const char *exclude_pattern, int check_integrity);
/**
 * @brief Проверяет целостность манифеста, читая его из stdin.
 * @return 0 если OK, 1 если поврежден, 2 если ошибка формата.
 */
int verify_manifest();

#endif