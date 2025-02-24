#include <iostream>
#include <random>

inline int add(int a, int b) { return a + b; }
inline int subtract(int a, int b) { return a - b; }

template <int (*f)(int, int)>
int apply(int a, int b)
{
    return f(a, b);
}

int main()
{
    std::random_device randomDevice;
    int a = randomDevice() % 100;
    int b = randomDevice() % 100;

    int s = add(a, b);
    int d = subtract(a, b);

    std::cout << s << std::endl;
    std::cout << d << std::endl;
    return 0;
}