# gcc -g engine.c && valgrind --tool=callgrind ./a.out
gcc -g src/engine.c && valgrind ./a.out
