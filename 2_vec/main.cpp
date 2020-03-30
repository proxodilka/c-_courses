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

int main() {
	//TESTS: буду[нет]
	TL* arr = new TL[10];
	delete[](arr);
	

	// std::cout << v.at(3) << std::endl;
	// std::cout << v[3] << std::endl;

}
