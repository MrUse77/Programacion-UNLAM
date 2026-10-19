#!/bin/bash
# Genera compile_commands.json con una entrada real por cada .c del repo.
# Paths absolutos: portables entre maquinas.
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUT="$SCRIPT_DIR/compile_commands.json"

INCLUDE_DIRS=(
  "$SCRIPT_DIR/includes"
)

FLAGS=(-std=c89 -Wall -Wextra)
for dir in "${INCLUDE_DIRS[@]}"; do
  FLAGS+=("-I$dir")
done

printf '[\n' > "$OUT"
first=1
while IFS= read -r -d '' file; do
  [[ $first -eq 1 ]] || printf ',\n' >> "$OUT"
  first=0
  printf '  {\n' >> "$OUT"
  printf '    "directory": "%s",\n' "$SCRIPT_DIR" >> "$OUT"
  printf '    "file": "%s",\n' "$file" >> "$OUT"
  printf '    "command": "%s %s -c %s"\n' "$(command -v clang || echo clang)" "${FLAGS[*]}" "$file" >> "$OUT"
  printf '  }' >> "$OUT"
done < <(find "$SCRIPT_DIR" -name '*.c' -not -path '*/.*' -print0 | sort -z)
printf '\n]\n' >> "$OUT"

echo "compile_commands.json generado: $(grep -c '"file"' "$OUT") archivos"
