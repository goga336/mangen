#!/bin/bash
# tests/common.sh
# Общие переменные и функции для всех тестов.

MANGEN="./mangen"
PASS=0
FAIL=0

# Создаем временную директорию для тестов
export TMPDIR=$(mktemp -d)
trap 'rm -rf "$TMPDIR"' EXIT

# Подготовка тестовых данных
mkdir -p "$TMPDIR/test/sub"
echo -n "Kaspersky" > "$TMPDIR/test/a.txt"
echo -n "KasperskyOS" > "$TMPDIR/test/sub/b.txt"
echo -n "skip"  > "$TMPDIR/test/skip.txt"

# Хэши для проверки, получены через echo -n "Kaspersky" | sha256sum
export HASH_A="f1761a5e3856dceb3e14d4555af92d3d1ac47604841f69fc72328b53ab45ca56"
export HASH_B="23c594d0b335379c13d1d6d25af1b72b341a8e8c7df5836e69b007684cc880ba"
export HASH_SKIP="42e93b9bb77d8a73e8412111b8f3d6befab66bf48fdcdefa80bb111819aa0cb1"

# Функция проверки вывода и кода возврата
check() {
    local desc="$1"
    local expected_rc="$2"
    local expected_stdout="$3"
    shift 3
    
    local out
    out=$("$@" 2>/dev/null)
    local rc=$?
    
    if [ "$rc" -ne "$expected_rc" ]; then
        echo "FAIL: $desc (ожидался код $expected_rc, получен $rc)"
        FAIL=$((FAIL+1))
        return
    fi
    
    if [ -n "$expected_stdout" ] && [ "$out" != "$expected_stdout" ]; then
        echo "FAIL: $desc (неверный вывод)"
        echo "  Ожидалось:"
        echo "$expected_stdout" | sed 's/^/    /'
        echo "  Получено:"
        echo "$out" | sed 's/^/    /'
        FAIL=$((FAIL+1))
        return
    fi
    
    echo "OK:   $desc"
    PASS=$((PASS+1))
}