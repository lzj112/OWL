#include <thread>         // std::thread
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

void foo()
{
    // do stuff...
    for(int i = 0; i < 10; ++i)
    {
        cout << "foo()  i: " << i << "\n";
    }
}
void bar(int x)
{
    // do stuff...
    for(int j = x; j < 20; ++j)
    {
        cout << "bar()  j: " << j << "\n";
    }
}
int main()
{
    std::cout << "main, foo and bar now execute concurrently...\n\n";
    std::thread first(foo);     // spawn new thread that calls foo()
    std::thread second(bar, 10);  // spawn new thread that calls bar(0)
    
    // synchronize threads:
    first.join();                // pauses until first finishes
    second.join();               // pauses until second finishes
    std::cout << "foo and bar completed.\n\n";
    std::cin.get();
    return 0;
}