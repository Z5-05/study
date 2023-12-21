template <typename T>
struct Node
{
    T value;
    Node<T>* next;

    explicit Node(const T& value) : value(value), next(nullptr) {}
	explicit Node(const T& value, Node<T>* next) : value(value), next(next) {}
};