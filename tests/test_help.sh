#!/bin/bash
# tests/test_help.sh

check "-h выводит помощь" 0 "$(cat <<EOF
Usage: mangen [OPTION]... [DIR_PATH]
Generate a SHA-256 manifest of files in DIR_PATH (default: current directory).

With no DIR_PATH, generates the manifest for the current working directory.
Each line of output follows the pattern: <relative_path> : <sha256_hex>

Options:
  -h                display this help and exit
  -v                display git commit hash (version) and exit
  -e NAME           exclude all files and directories named NAME
  -E PATTERN        exclude files/dirs matching PATTERN
                    ('.' matches any single char, '*' matches any sequence)
  -i                add integrity check line at the end
  --verify          verify manifest integrity from stdin

Report bugs to: https://github.com/goga336/mangen/issues

Exit status:
 0  if OK,
 1  if minor problems (e.g., cannot access subdirectory),
 2  if serious trouble (e.g., cannot access command-line argument).
EOF
)" "$MANGEN" -h