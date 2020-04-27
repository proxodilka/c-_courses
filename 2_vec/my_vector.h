#pragma once

namespace my {

	template<typename T>
	T dif(T a, T b){
		return a < b ? 0 : a - b;
	}

	std::size_t npos = -1;

	template <typename T>
	class vector {
	public:
		class iterator;
		class const_iterator;
		vector() : my_data(nullptr), my_size(0), my_cap(0) {}

		vector(std::size_t sz, const T& value) : vector() {
			_reserve(sz);
			for (std::size_t i = 0; i < my_size; i++) {
				push_back(value);
			}
		}

		vector(std::size_t sz, T&& value) : vector() {
			_reserve(sz);
			for (std::size_t i = 0; i < my_size; i++) {
				push_back(std::move(value));
			}
		}

		vector(const vector& other) : vector() {
			*this = other;
		}

		vector(vector&& other) : my_data(other.data), my_size(other.my_size), my_cap(other.my_cap) {
			_clear(other);
		}

		vector(const std::initializer_list<T>& il) : vector() {
			reserve(il.size());
			for(auto& x: il){
				push_back(std::move(x));
			}
		}

		vector& operator=(const vector& other) {
			clear();
			reserve(other.my_cap);
			for (std::size_t i = 0; i < other.my_size; i++) {
				push_back(other.my_data[i]);
			}
		}

		vector& operator=(vector&& other) {
			_clear(true);

			my_data = other.my_data;
			my_size = other.my_size;
			my_cap = other.my_cap;

			other._clear();
		}

		void clear() {
			_clear();
		}

		bool empty() const {
			return my_size == 0;
		}

		void reserve(std::size_t new_capacity) {
			if (new_capacity <= my_cap) {
				return;
			}
			_reserve(new_capacity);
		}

		void resize(std::size_t new_size) {
			T value;
			_destruct_objs(my_data + my_size - dif(my_size, new_size), dif(my_size, new_size));
			reserve(new_size);
			for (std::size_t i = my_size; i < new_size; i++) {
				push_back(value);
			}
			my_size = new_size;
		}

		void resize(std::size_t new_size, const T& value) {
			_destruct_objs(my_data + my_size - dif(my_size, new_size), dif(my_size, new_size));
			reserve(new_size);
			for (std::size_t i = my_size; i < new_size; i++) {
				push_back(value);
			}
			my_size = new_size;
		}

		~vector() {
			_clear(true);
		}

		std::size_t size() const { return my_size; }
		std::size_t capacity() const { return my_cap; }

		T& operator[](std::size_t index) { return my_data[index]; }

		T& at(std::size_t index) {
			if (index >= my_size) {
				throw std::out_of_range("Index out of range");
			}
			return my_data[index];
		}

		void push_back(const T& value) {
			_push_back(value);
		}

		void push_back(T&& value) {
			_push_back(std::move(value));
		}

		iterator begin() {
			return iterator(my_data);
		}

		iterator end() {
			return iterator(my_data + my_size);
		}

		const_iterator cbegin(){
			return begin();
		}

		const_iterator cend(){
			return end();
		}

	private:
		T* my_data;
		std::size_t my_size;
		std::size_t my_cap;

		template<typename U>
		void _push_back(U&& value){
			_try_to_reserve();
			new (my_data + (my_size++)) T(std::forward<U>(value));
		}

		void _destruct_objs(T* ptr, std::size_t amount = npos){
			if (amount == npos){
				amount = my_size;
			}
			for (std::size_t i = 0; i<amount; i++){
				ptr[i].~T();
			}
		}

		void _clear(bool force=false) {
			_destruct_objs(my_data);
			if (force) {
				operator delete(my_data);
				my_data = nullptr;
				my_cap = 0;
			}
			my_size = 0;
		}

		void _try_to_reserve() {
			if (my_size == my_cap) {
				reserve(my_cap == 0 ? 1 : my_cap * 2);
			}
		}

		_reserve(std::size_t new_capacity){
			T* tmp = static_cast<T*>(operator new(sizeof(T)*new_capacity));

			for (std::size_t i = 0; i < min(my_size, new_capacity); ++i) {
				new (tmp + i) T(std::move(my_data[i]));
			}

			// _clear() will set 'my_size' to 0, so we should save old value
			// in a temporary variable
			std::size_t size = my_size;
			_clear(true);
			my_data = tmp;
			my_cap = new_capacity;
			my_size = size;
		}

	};

	template<typename T>
	class vector<T>::iterator : public std::iterator<std::random_access_iterator_tag, T> {
		protected:
			friend vector;
			T* pointer;
			iterator(T* pointer) noexcept : pointer(pointer) {}
		public:
			T& operator*() { return *(this->pointer); }
			T& operator[](int offset) { return *(this->pointer + offset); }
			T* operator->(){ return pointer; }

			iterator operator+(int value) { return iterator(this->pointer + value); }
			iterator operator-(int value) { return iterator(this->pointer - value); }

			bool operator==(const iterator& it) { return it.pointer == this->pointer; }
			bool operator!=(const iterator& it) { return it.pointer != this->pointer; }
			bool operator<(const iterator& it) { return this->pointer < it.pointer; }
			bool operator>(const iterator& it) { return this->pointer > it.pointer; }
			bool operator<=(const iterator& it) { return this->pointer <= it.pointer; }
			bool operator>=(const iterator& it) { return this->pointer >= it.pointer; }

			iterator& operator++(int) { this->pointer++; return *this; }
			iterator& operator--(int) { this->pointer--; return *this; }
			iterator operator++() {
				iterator retValue(*this);
				this->pointer++;
				return retValue;
			}
			iterator operator--() {
				iterator retValue(*this);
				this->pointer--;
				return retValue;
			}
			iterator& operator+=(int value) { this->pointer += value; return *this; }
			iterator& operator-=(int value) { this->pointer -= value; return *this; }
	};

	template<typename T>
	class vector<T>::const_iterator : iterator {
		friend vector;
		const_iterator(T* pointer): iterator(pointer) {}

	public:
		const_iterator(const iterator& it) noexcept : iterator(it.pointer) {}

		const T& operator*() const { return *(this->pointer); }
		const T& operator[](int offset) const { return *(this->pointer + offset); }
		const T* operator->() const { return this->pointer; }
	};
} // namespace my