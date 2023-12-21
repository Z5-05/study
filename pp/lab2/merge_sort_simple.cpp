#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include "utils.cpp"
#include <time.h>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[])
{	
	int array_size = atoi(argv[1]);
	float diff_time;

	int* array = (int*)malloc(sizeof(int) * array_size);
    
	srand(clock());
	for (int i = 0; i < array_size; i++)
		array[i] = rand();

	clock_t time = clock();
    merge_sort(array, 0, array_size-1);
	diff_time = (clock() - time) / 1000.0L;

	int last = 0;
	for (int i = 0; i < array_size; i++)
	{
		if (array[i] < last)
		{
			throw std::exception();
		}
		last = array[i];
	}
	free(array);
	cout << array_size << ",merge_sort_simple," << diff_time << endl;
	return 0;
}