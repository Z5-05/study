#include "stack_lf.cpp"
#include <cstdlib>
#include <iostream>
#include <stack>
#include <limits.h>
#include <string>

void fail(const char *message) {
    std::cerr << "Fail:\n";
    std::cerr << message << "\n";
    std::cout << "-1 bad output\n"; // to get WA
    exit(0);
};

namespace internal_tests {
    void check_methods() {
        int i, n = 10;
        int elems[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::optional<int> value1;
        int value2;

        Stack_lf<int> stack1;
        std::stack<int> stack2;
        for (i = 0; i < n; ++i)
        {
            stack1.push(elems[i]);
            stack2.push(elems[i]);
            value1 = stack1.top();
            value2 = stack2.top();
            stack1.pop();
            stack2.pop();
            if (value1 != value1.value_or(INT_MIN))
                fail("values are not equal!");
        }
    }

    void check_empty_stack() {
       int i, n = 10;
       int elems[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
       bool flag;
       Stack_lf<int> stack;

       for (i = 0; i < n; ++i)
        stack.push(elems[i]);
       
       for (i = 0; i < n; ++i)
       {
            flag = stack.isEmpty();
            if (!flag)
                fail("Non empty stack returns null!");
            stack.pop();
       }
       if (!flag)
            fail("Empty stack returns not null!");
    }

    void all_tests() {
        check_methods();
        check_empty_stack();
    }
}

int main() {
    internal_tests::all_tests();
    return 0;
}