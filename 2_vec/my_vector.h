#pragma once

namespace my {

	template <typename T>
	class vector {
	public:
		vector() : my_data(nullptr), my_size(0), my_cap(0) {}

		vector(std::size_t sz, const T& value)
			: my_data(new T[sz]), my_size(sz), my_cap(sz)
		{
			for (std::size_t i = 0; i < my_size; i++) {
				my_data[i] = value;
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
			my_size = 0;
		}

		bool empty() const {
			return my_size == 0;
		}

		void resize(std::size_t new_size) {
			T value;
			for (std::size_t i = my_size; i < new_size; i++) {
				push_back(value);
			}
			my_size = new_size;
		}

		void resize(std::size_t new_size, const T& value) {
			for (std::size_t i = my_size; i < new_size; i++) {
				push_back(value);
			}
			my_size = new_size;
		}

		~vector() {
			delete[] my_data;
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
			my_data[my_size++] = value;
		}

		void push_back(T&& value) {
			_try_to_reserve();
			my_data[my_size++] = std::move(value);
		}

		void reserve(std::size_t new_capacity) {
			if (new_capacity <= my_cap) {
				return;
			}

			T* tmp = new T[new_capacity];

			for (std::size_t i = 0; i < my_size; ++i) {
				tmp[i] = std::move(my_data[i]);
			}

			delete[] my_data;
			my_data = tmp;
			my_cap = new_capacity;
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

		void _clear(bool force=false) {
			if (force) {
				delete[] my_data;
			}
			my_data = nullptr;
			my_size = 0;
			my_cap = 0;
		}

		void _try_to_reserve() {
			if (my_size == my_cap) {
				reserve(my_cap == 0 ? 1 : my_cap * 2);
			}
		}

	};
} // namespace my