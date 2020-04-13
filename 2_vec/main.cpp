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

struct A;

struct B {
	B(){
		cout << "default B\n";
	}

	explicit B(A a){
		cout << "B(a)\n";
	}

	B operator=(A a){
		cout << "B = a\n";
		return B();
	}
};

struct A {
	operator B(){
		cout << "cast\n";
		return B();
	}
};

#include <vector>

int main() {
	A a;
	B b;
	b = (B)a;
}
