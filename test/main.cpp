#include <iostream>

int main(){
    return [](){std::cout << "Hello world!\n"; return 0;}();
}