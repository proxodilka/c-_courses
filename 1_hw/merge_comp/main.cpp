#include <iostream>
//#include "Solution.h"
#include <vector>
#include <fstream>
#include <any>
#include <algorithm>

std::ofstream log_("log.txt");

class Less {
public:
	int x;
	Less(int x = 0) :x(x) {} 
};

class Less5:public Less {
public:
	Less5(int x = rand() % 5): Less(x){}

	bool operator<(const Less5& other) {
		return this->x < other.x;
	}
};

class Less10:public Less {
public:
	Less10(int x = rand() % 10): Less(x) {}

	bool operator<(const Less10& other) {
		return this->x < other.x;
	}
};

class Less15N :public Less {
public:
	Less15N(int x = rand() % 15 + 5) : Less(x) {}

	bool operator<(const Less15N& other) {
		return this->x < other.x;
	}
};

std::ostream& operator<<(std::ostream& out, const Less& obj) {
	out << obj.x;
	return out;
}

bool operator<(const Less5& a, const Less10& b) {
	return a.x < b.x;
}

template<typename T, typename U>
struct has_less {
private:
	template<typename ...>
	static char f(...) { return 0; }

	template<typename C, typename P>
	static decltype(C() < P(), int()) f(int x) { return 0; }

public:
	static const bool value = sizeof(f<T, U>(2)) == sizeof(int);
};

class Comparator {
public:
	template<typename T, typename U>
	std::enable_if_t<has_less<T, U>::value, bool> operator()(T& a, U& b) { // a < b?
		return a < b;
	}

	template<typename T, typename U>
	std::enable_if_t<!has_less<T, U>::value, bool> operator()(T& a, U& b) {
		return static_cast<void*>(&a) < static_cast<void*>(&b);
	}
};

template<typename Comparator, typename Container1, typename Container2>
void merge(Container1& first, Container2& second, std::vector<std::any>& result, Comparator comparator = Comparator()) {
	auto it1 = first.begin(), end1 = first.end();
	auto it2 = second.begin(), end2 = second.end();

	while (it1 != end1 && it2 != end2) {
		if (comparator(*it1, *it2)) {
			result.push_back(*it1);
			it1++;
		}
		else {
			result.push_back(*it2);
			it2++;
		}
	}

	while (it1 != end1) {
		result.push_back(*it1);
		it1++;
	}

	while (it2 != end2) {
		result.push_back(*it2);
		it2++;
	}
}

template<typename First, typename Second>
void print_any(std::vector<std::any>& vals, std::ostream& out = std::cout) {
	for (auto& x : vals) {
		try {
			std::any_cast<First>(x);
		}
		catch (std::bad_any_cast e) {
			out << std::any_cast<Second>(x) << " ";
		}
		try {
			std::any_cast<Second>(x);
		}
		catch (std::bad_any_cast e) {
			out << std::any_cast<First>(x) << " ";
		}
		if (std::is_same<First, Second>::value) {
			out << std::any_cast<First>(x) << " ";
		}
	}
	out << "\n";
}

template<typename Container>
void log_random_values(Container& container) {
	for (auto& x : container) {
		log_ << x << "(" << &x << ")" << " ";
	}
	log_ << "\n";
}

template<typename Container>
void prepare(Container& container) {
	std::sort(container.begin(), container.end());
	log_random_values(container);
}

//template <typename T = void  >
//struct less
//{
//	bool operator()(const int& t1, const int& t2)
//	{
//		return t1 < t2;
//	}
//};

//template <>
// struct less//<void>
// {
// 	template <typename T, typename U>
// 	bool operator()(const T& t, const U& u)
// 	{
// 		return t < u;
// 	}
// };

// struct A {};

// less<int>(3, A());

















int main() {
	std::vector<int> iv1 = { -5, 2, 5, 10, 13, 14 }, iv2 = { 0, 3, 15 };
	std::vector<Less5> l5v1(6), l5v2(3);
	std::vector<Less10> l10v1(3), l10v2(4);
	std::vector<Less15N> l15v1(5), l15v2(3);
	
	prepare(iv1);
	prepare(iv2);
	prepare(l5v1);
	prepare(l5v2);
	prepare(l10v1);
	prepare(l10v2);
	prepare(l15v1);
	prepare(l15v2);

	std::vector<std::vector<std::any>> results(4);

	// 0: i-i	(E)
	// 1: l5-l10(E)
	// 2: i-l15	(NE)
	// 3: l15-l5(NE)
	merge<Comparator>(iv1, iv2, results[0]);
	merge<Comparator>(l5v1, l10v2, results[1]);
	merge<Comparator>(iv1, l15v1, results[2]);
	merge<Comparator>(l15v2, l5v1, results[3]);

	print_any<int, int>(results[0]);
	print_any<Less5, Less10>(results[1]);
	print_any<int, Less15N>(results[2]);
	print_any<Less15N, Less5>(results[3]);

}