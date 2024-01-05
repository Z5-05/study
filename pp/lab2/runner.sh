clear

# g++ -o merge_sort_base_threads merge_sort_base_threads.cpp -pthread
# chmod +x merge_sort_base_threads
# ./merge_sort_base_threads 10000000

g++ -o merge_sort_binary_search_threads merge_sort_binary_search_threads.cpp
chmod +x merge_sort_binary_search_threads
./merge_sort_binary_search_threads 1000000

g++ -o merge_sort_many_threads merge_sort_many_threads.cpp -pthread
chmod +x merge_sort_many_threads
./merge_sort_many_threads 1000000

g++ -o merge_sort_simple merge_sort_simple.cpp
chmod +x merge_sort_simple
./merge_sort_simple 1000000

rm -f merge_sort_base_threads merge_sort_simple merge_sort_many_threads merge_sort_binary_search_threads