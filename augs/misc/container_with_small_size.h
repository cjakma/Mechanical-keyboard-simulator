#pragma once
#include <limits>
#include "augs/ensure.h"

namespace augs {
	template <class T, class size_type>
	class container_with_small_size {
		T container;
	public:
		template <class... Args>
		container_with_small_size(Args&&... args) : container(std::forward<Args>(args)...) {
			overflow_check();
		}

		T& operator*() {
			return container;
		}

		const T& operator*() const {
			return container;
		}

		T* operator->() {
			return &container;
		}

		bool overflowed() const {
			return container.size() > std::numeric_limits<size_type>::max();
		}

		bool is_full() const {
			return container.size() >= std::numeric_limits<size_type>::max();
		}

		void overflow_check() {
			if (overflowed()) {
				LOG("Warning! %x has overflowed - clearing the container.", typeid(T).name());
				clear();
			}
		}

		const T* operator->() const {
			return &container;
		}

		template <class... Args>
		decltype(auto) insert(Args&&... args) {
			const auto result = container.insert(std::forward<Args>(args)...);
			overflow_check();
			return result;
		}

		template <class... Args>
		decltype(auto) find(Args&&... args) {
			return container.find(std::forward<Args>(args)...);
		}

		template <class... Args>
		decltype(auto) push_back(Args&&... args) {
			container.push_back(std::forward<Args>(args)...);
			overflow_check();
		}
		
		template <class... Args>
		decltype(auto) erase(Args&&... args) {
			return container.erase(std::forward<Args>(args)...);
		}

		template <class... Args>
		decltype(auto) operator[](Args&&... args) {
			return container.operator[](std::forward<Args>(args)...);
		}

		template <class... Args>
		decltype(auto) operator[](Args&&... args) const {
			return container.operator[](std::forward<Args>(args)...);
		}

		decltype(auto) begin() {
			return container.begin();
		}

		decltype(auto) end() {
			return container.end();
		}

		decltype(auto) begin() const {
			return container.begin();
		}

		decltype(auto) end() const {
			return container.end();
		}

		decltype(auto) size() const {
			return container.size();
		}

		void clear() {
			container.clear();
		}
	};

	template<class A, class T, class size_type>
	void read_object(
		A& ar,
		container_with_small_size<T, size_type>& storage
	) {
		read(ar, *storage, size_type());
	}

	template<class A, class T, class size_type>
	void write_object(
		A& ar,
		const container_with_small_size<T, size_type>& storage
	) {
		write(ar, *storage, size_type());
	}
}

template <
	class A, 
	class B, 
	class size_type_A, 
	class size_type_B
>
bool operator==(
	const augs::container_with_small_size<A, size_type_A>& a,
	const augs::container_with_small_size<B, size_type_B>& b
) {
	return *a == *b;
}