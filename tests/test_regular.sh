# Проверка -E '*.txt' (исключает ВСЕ txt файлы, включая вложенные)
out=$("$MANGEN" "$TMPDIR/test" -E '*.txt' | sort)
expected=""

if [ "$out" = "$expected" ]; then
    echo "✅ OK:   флаг -E исключает по паттерну '*.txt'"
    PASS=$((PASS+1))
else
    echo "❌ FAIL: флаг -E исключает по паттерну '*.txt'"
    echo "  Ожидалось пусто, получено:"
    echo "$out" | sed 's/^/    /'
    FAIL=$((FAIL+1))
fi

# Проверка -E 'skip.???' (точка = один символ, вопросительный знак тут не поддерживается, используем точку)
# В задании сказано: точка = один символ.
# Попробуем исключить a.txt через "?.txt" (если бы поддерживался ?)
# Но в задании ? нет. Только точка и звездочка.
# Проверим точку: "sk.p" совпадет с "skip"? Нет. "sk.p" совпадет с "sk.p".
# Давай проверим "skip.tx." (последняя точка = любой символ, совпадет с "skip.txt")
out=$("$MANGEN" "$TMPDIR/test" -E 'skip.tx.' | sort)
expected="$(printf "a.txt : %s\nsub/b.txt : %s" "$HASH_A" "$HASH_B")"

if [ "$out" = "$expected" ]; then
    echo "✅ OK:   флаг -E исключает по паттерну 'skip.tx.'"
    PASS=$((PASS+1))
else
    echo "❌ FAIL: флаг -E исключает по паттерну 'skip.tx.'"
    FAIL=$((FAIL+1))
fi

