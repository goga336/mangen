#!/bin/bash
# tests/run_tests.sh

# скрипт для запуска end-to-end тестов

cd "$(dirname "$0")/.." || exit 1

# Подключаем общие переменные и функции
source tests/common.sh

TEST_NUM=0
section() {
    TEST_NUM=$((TEST_NUM + 1))
    echo ""
    echo "Тест $TEST_NUM: $1"
    echo "--------------------------------------------------"
}

# Запускаем все тесты
section "Проверка вывода справки и информации о версии"
source tests/test_help.sh

section "Проверка базового обхода каталогов, хэширования и фильтрации файлов"
source tests/test_basic.sh

section "Проверка обработки ошибок, неверных аргументов и кодов возврата"
source tests/test_errors.sh

section "Проверка фильтрации файлов по регулярным выражениям"
source tests/test_regular.sh

section "Проверка контроля целостности манифеста при передаче данных"
source tests/test_integrity.sh

echo "--------------------------------------------------"
echo "Тестов пройдено: $PASS"
echo "Тестов провалено: $FAIL"
echo "--------------------------------------------------"

if [ "$FAIL" -ne 0 ]; then
    exit 1
fi
exit 0