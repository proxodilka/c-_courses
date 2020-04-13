#include <iostream>
#include <map>

using namespace std;

class TL;
map<TL*, int> d_counter;

class TL {
public:

	~TL(){
		cout << "destructor are called at " << this << "\n";
		d_counter[this]++;
	}
};

// struct A;

// struct B {
// 	B(){
// 		cout << "default B\n";
// 	}

// 	explicit B(A a){
// 		cout << "B(a)\n";
// 	}

// 	B operator=(A a){
// 		cout << "B = a\n";
// 		return B();
// 	}
// };

// struct A {
// 	operator B(){
// 		cout << "cast\n";
// 		return B();
// 	}
// };

struct Test{
	std::int64_t x = 2;
	// Test(){std::cout << "Constructor at " << this << "\n";}
	// ~Test(){std::cout << "Destructor at " << this << "\n";}
	//~Test(int arg){std::cout << "Custom destructor at " << this << " with " << arg << " param\n";}
};

#include <vector>

int main() {
	std::size_t N = 100000000;
	//Test t;

	Test* arr = static_cast<Test*>(operator new[](sizeof(Test)*N));
	std::cout << "allocated " << N*sizeof(Test)/(1024*1024) << "mbytes\n";
	for (int i=0; i<N; i++){
		new (arr+i) Test();
	}
	std::cout << "filled\n";
	system("pause");
	std::cout << "deallocating...\n";
	operator delete(arr);
	std::cout << "deallocated\n";
	system("pause");
}
