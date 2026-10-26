#!/bin/bash
# tests/test_errors.sh

check "неизвестный флаг возвращает код 2" 2 "" "$MANGEN" -x
check "флаг -e без аргумента возвращает код 2" 2 "" "$MANGEN" -e
check "несуществующая директория возвращает код 1" 1 "" "$MANGEN" "$TMPDIR/nonexistent"
check "передан файл вместо директории возвращает код 1" 1 "" "$MANGEN" "$TMPDIR/test/a.txt"