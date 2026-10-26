#!/bin/bash
# tests/run_tests.sh
# Главный скрипт для запуска end-to-end тестов.

cd "$(dirname "$0")/.." || exit 1

# Подключаем общие переменные и функции
source tests/common.sh

# Запускаем все тестовые модули
source tests/test_help.sh
source tests/test_basic.sh
source tests/test_errors.sh
source tests/test_regular.sh

# --- ИТОГИ ---
echo "----------------------------------------"
echo "Тесты пройдены: $PASS"
echo "Тесты провалены: $FAIL"
echo "----------------------------------------"

if [ "$FAIL" -ne 0 ]; then
    exit 1
fi
exit 0