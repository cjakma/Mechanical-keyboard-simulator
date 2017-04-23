#pragma once
#include <utility>
#include "augs/templates/memcpy_safety.h"

namespace augs {
	template<class A, class B>
	class trivially_copyable_pair {
	public:
		static_assert(is_memcpy_safe_v<A>, "First type is not trivially copyable!");
		static_assert(is_memcpy_safe_v<B>, "Second type is not trivially copyable!");

		typedef std::pair<A, B> pair;
		
		// GEN INTROSPECTOR class augs::trivially_copyable_pair class A class B
		A first;
		B second;
		// END GEN INTROSPECTOR

		trivially_copyable_pair(const A& a = A(), const B& b = B()) : first(a), second(b) {}
		trivially_copyable_pair(const pair& p) : first(p.first), second(p.second) {}

		trivially_copyable_pair& operator=(const pair& p) {
			first = p.first;
			second = p.second;
			return *this;
		}

		template <class A1, class A2>
		void set(const A1& a, const A2& b) {
			first = static_cast<A>(a);
			second = static_cast<B>(b);
		}

		operator pair() const {
			return{ first, second };
		}

		bool operator<(const trivially_copyable_pair& b) const {
			return pair(*this) < pair(b);
		}

		bool operator==(const trivially_copyable_pair& b) const {
			return pair(*this) == pair(b);
		}
	};
}