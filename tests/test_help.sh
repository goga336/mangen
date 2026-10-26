#!/bin/bash
# tests/test_help.sh

check "-h выводит помощь" 0 "$(cat <<EOF
Usage: mangen [DIR_PATH] [OPTIONS]

Generate a manifest of files in DIR_PATH (default: current directory).
Each line: <relative_path> : <sha256_hex>

Options:
  -h                Show this help and exit
  -e NAME           Exclude files and directories named NAME
  -E PATTERN        Exclude files/dirs matching PATTERN ('.' and '*')
EOF
)" "$MANGEN" -h