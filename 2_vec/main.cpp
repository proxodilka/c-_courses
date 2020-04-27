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

struct Test{
	std::int64_t x = 2;
	// Test(){std::cout << "Constructor at " << this << "\n";}
	// ~Test(){std::cout << "Destructor at " << this << "\n";}
	//~Test(int arg){std::cout << "Custom destructor at " << this << " with " << arg << " param\n";}
};

#include <vector>
#include "my_vector.h"

void primes(int n){
	for(int i=0; i*i<n; i++){
		
	}
}

template<typename Container>
void print_container(Container &container){
	for(auto& x: container){
		std::cout << x << " ";
	}
	std::cout << "\n";
}

int main() {
	my::vector<int> v = {1,2,3,4,5};
	auto it1 = v.cbegin();
	auto it = v.begin();
	*it = 5;
	//*it1 = 2;
	print_container(v);
}
