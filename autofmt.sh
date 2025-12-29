#!/bin/sh
FILES=$(find src include -type f \( -name "*.c" -o -name "*.h" \))

if [ -z "$FILES" ]; then
    exit 0
fi

for f in $FILES; do
    echo "[+] Formatting $f"
    clang-format -i "$f"
done
