#!/bin/bash
# tests/test_integrity.sh

# гинерация манифеста с флагом -i
"$MANGEN" "$TMPDIR/test" -i > "$TMPDIR/manifest.txt"

# проверка, что в конце есть строка --- integrity
if tail -n 1 "$TMPDIR/manifest.txt" | grep -q "^--- integrity: "; then
    echo "OK:   манифест -i содержит строку integrity"
    PASS=$((PASS+1))
else
    echo "FAIL: манифест -i НЕ содержит строку integrity"
    FAIL=$((FAIL+1))
fi

# проверка успешной валидации
"$MANGEN" --verify < "$TMPDIR/manifest.txt" > "$TMPDIR/out.txt" 2>&1
rc=$?

if [ $rc -eq 0 ] && grep -q "integrity check OK" "$TMPDIR/out.txt"; then
    echo "OK:   верификация целого манифеста возвращает код 0"
    PASS=$((PASS+1))
else
    echo "FAIL: верификация целого манифеста"
    echo "  Код возврата: $rc"
    echo "  Вывод: $(cat "$TMPDIR/out.txt")"
    FAIL=$((FAIL+1))
fi