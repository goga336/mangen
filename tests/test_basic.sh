#!/bin/bash
# tests/test_basic.sh

# Дефолтный путь
out=$(cd "$TMPDIR/test" && "$OLDPWD/$MANGEN" | sort)
expected="$(printf "a.txt : %s\nskip.txt : %s\nsub/b.txt : %s" "$HASH_A" "$HASH_SKIP" "$HASH_B")"

if [ "$out" = "$expected" ]; then
    echo "OK:   генерация манифеста для текущей директории"
    PASS=$((PASS+1))
else
    echo "FAIL: генерация манифеста для текущей директории"
    echo "  Ожидалось:"
    echo "$expected" | sed 's/^/    /'
    echo "  Получено:"
    echo "$out" | sed 's/^/    /'
    FAIL=$((FAIL+1))
fi

# Передача пути и исключение файла -e
out=$("$MANGEN" "$TMPDIR/test" -e skip.txt | sort)
expected="$(printf "a.txt : %s\nsub/b.txt : %s" "$HASH_A" "$HASH_B")"

if [ "$out" = "$expected" ]; then
    echo "OK:   флаг -e исключает файл"
    PASS=$((PASS+1))
else
    echo "FAIL: флаг -e исключает файл"
    FAIL=$((FAIL+1))
fi

# Исключение директории -e
out=$("$MANGEN" "$TMPDIR/test" -e sub | sort)
expected="$(printf "a.txt : %s\nskip.txt : %s" "$HASH_A" "$HASH_SKIP")"

if [ "$out" = "$expected" ]; then
    echo "OK:   флаг -e исключает директорию"
    PASS=$((PASS+1))
else
    echo "FAIL: флаг -e исключает директорию"
    FAIL=$((FAIL+1))
fi