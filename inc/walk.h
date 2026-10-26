/**
 * @file walk.h
 * @brief Рекурсивный обход директорий.
 */
#ifndef WALK_H
#define WALK_H

/**
 * @brief Рекурсивно обходит каталог и хэширует все найденные файлы.
 * 
 * @param dir          Реальный путь к каталогу для обхода.
 * @param rel_prefix   Относительный путь для формирования вывода.
 * @param exclude_name Имя файла или каталога для исключения (может быть NULL).
 * @return 0 при успехе, -1 при фатальной ошибке открытия корневой директории.
 */
int recursive_walk(const char *dir, const char *rel_prefix, const char *exclude_name, const char *exclude_pattern);

#endif