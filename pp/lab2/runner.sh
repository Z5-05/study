clear

g++ -o merge_sort_base_threads merge_sort_base_threads.cpp -pthread
chmod +x merge_sort_base_threads
./merge_sort_base_threads 10000 4

g++ -o merge_sort_many_threads merge_sort_many_threads.cpp -pthread
chmod +x merge_sort_many_threads
./merge_sort_many_threads 10000

g++ -o merge_sort_simple merge_sort_simple.cpp  -pthread
chmod +x merge_sort_simple
./merge_sort_simple 10000

rm -f merge_sort_base_threads merge_sort_simple merge_sort_many_threads