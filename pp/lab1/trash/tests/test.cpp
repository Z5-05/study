#include "stack_lf.cpp"
#include <cstdlib>
#include <iostream>
#include <stack>
#include <limits.h>
#include <string>
#include "threadPooling.h"
#pragma GCC diagnostic ignored "-Wreturn-type"
using namespace std;

// namespace internal_tests {
//     void check_methods() {
//         int i, n = 10;
//         int elems[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//         std::optional<int> value1;
//         int value2;

//         Stack_lf<int> stack1;
//         std::stack<int> stack2;
//         for (i = 0; i < n; ++i)
//         {
//             stack1.push(elems[i]);
//             stack2.push(elems[i]);
//             value1 = stack1.top();
//             value2 = stack2.top();
//             stack1.pop();
//             stack2.pop();
//             if (value1 != value1.value_or(INT_MIN))
//                 fail("values are not equal!");
//         }
//     }

//     void all_tests() {
//         check_methods();
//         check_empty_stack();
//     }
// }


Stack_lf<int> stack1;

int main() {
    ThreadPool pool(4);  
    
    clock_t tm = clock();
    for (int i = 0; i < 200000; ++i) { 
        
        pool.enqueue([i] { 
            // cout << "Task " << i << " is running on thread "
            //      << this_thread::get_id() << endl; 
            // Simulate some work 
            stack1.push(i);
            if (rand() % 4 == 0) {
                stack1.pop();
            }
            
        }); 
    }
    float diff = (clock() - tm) / 1000.0L;
    
    cout << diff;
    cout << stack1.top().value_or(11);
}


