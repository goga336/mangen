# Проверка -E '*.txt'
out=$("$MANGEN" "$TMPDIR/test" -E '*.txt' | sort)
expected=""

if [ "$out" = "$expected" ]; then
    echo "OK:   флаг -E исключает по паттерну '*.txt'"
    PASS=$((PASS+1))
else
    echo "FAIL: флаг -E исключает по паттерну '*.txt'"
    echo "  Ожидалось пусто, получено:"
    echo "$out" | sed 's/^/    /'
    FAIL=$((FAIL+1))
fi

out=$("$MANGEN" "$TMPDIR/test" -E 'skip.tx.' | sort)
expected="$(printf "a.txt : %s\nsub/b.txt : %s" "$HASH_A" "$HASH_B")"

if [ "$out" = "$expected" ]; then
    echo "OK:   флаг -E исключает по паттерну 'skip.tx.'"
    PASS=$((PASS+1))
else
    echo "FAIL: флаг -E исключает по паттерну 'skip.tx.'"
    FAIL=$((FAIL+1))
fi

