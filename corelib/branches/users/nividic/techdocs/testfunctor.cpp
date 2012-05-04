
#include <iostream>

template<class T>
struct functor
{
    static T value()
    {
        std::cout << "FUNCTOR function\n";
        return T();
    }
};

int main(int argc, const char **argv)
{
    functor<double>::value();

    return 0;
}
