/**
 * @file hash.h
 * @brief Вычисление SHA-256 для файлов.
 */


#ifndef HASH_H
#define HASH_H
/**
 * @brief Вычисляет SHA-256 хэш файла и выводит его в stdout.
 * 
 * Формат вывода: `<display_path> : <sha256_hex>`
 * 
 * @param path         Реальный путь к файлу для чтения.
 * @param display_path Путь для вывода (относительно корня обхода).
 * @return 0 при успехе, -1 при ошибке.
 */
int hash_file(const char *path, const char *display_path);

#endif