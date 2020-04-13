#pragma once

namespace my {

	template <typename T>
	class vector {
	public:
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
			for (std::size_t i = 0; i < il.size(); i++) {
				push_back(il[i]);
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
			reserve(new_size);
			for (std::size_t i = my_size; i < new_size; i++) {
				push_back(value);
			}
			my_size = new_size;
		}

		void resize(std::size_t new_size, const T& value) {
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
			_try_to_reserve();
			new (my_data + (my_size++)) T(value);
		}

		void push_back(T&& value) {
			_try_to_reserve();
			new (my_data + (my_size++)) T(std::move(value));
		}

		T* begin() {
			return my_data;
		}

		T* end() {
			return my_data + my_size;
		}

	private:
		T* my_data;
		std::size_t my_size;
		std::size_t my_cap;

		void _destruct_objs(T* ptr, std::size_t amount = my_size){
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
} // namespace my