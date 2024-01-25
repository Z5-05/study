clear

g++ -std=c++17 -o main -pthread main.cpp atomic_threadpool/lfqueue.c atomic_threadpool/at_thpool.c
chmod +x main
./main $1

rm -f main