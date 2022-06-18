find . -type d \( -name include -o -name highway \) \
-prune -false -o -iname *.h -o -iname *.c -o -iname *.cpp -o -iname *.hpp  \
| xargs clang-format -style=file -i -fallback-style=none
