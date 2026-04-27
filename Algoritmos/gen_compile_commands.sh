#!/bin/bash
# Genera compile_commands.json portable para clangd

# Obtiene el directorio donde está este script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

cat > "$SCRIPT_DIR/compile_commands.json" << EOF
[
  {
    "directory": "$SCRIPT_DIR",
    "command": "/usr/bin/gcc -ITDAs/inlcudes/ -Wall -Wextra -std=c11 -c dummy.c",
    "file": "dummy.c"
  }
]
EOF

echo "✓ compile_commands.json generado en $SCRIPT_DIR"
