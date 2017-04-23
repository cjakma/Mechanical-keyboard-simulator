#pragma once
#include <random>
#include <utility>
#include <vector>

#include "augs/misc/minmax.h"
#include "augs/math/declare_math.h"

template <class generator_type>
struct basic_randomization {
	generator_type generator;

	basic_randomization(const size_t seed = 0);

	int randval(
		const int min, 
		const int max
	);

	unsigned randval(
		const unsigned min, 
		const unsigned max
	);

	float randval(
		const float min, 
		const float max
	);

	float randval(const float minmax);
	
	std::vector<float> make_random_intervals(
		const size_t n, 
		const float maximum
	);

	std::vector<float> make_random_intervals(
		const size_t n, 
		const float maximum, 
		const float variation_multiplier
	);

	template<class A, class B>
	auto randval(const std::pair<A, B> p) {
		return randval(p.first, p.second);
	}

	template<class T>
	vec2t<T> random_point_in_ring(
		const T min_radius,
		const T max_radius
	) {
		return randval(min_radius, max_radius) * vec2().set_from_degrees(
			randval(
				static_cast<T>(0), 
				static_cast<T>(360)
			));
	}

	template<class T>
	vec2t<T> random_point_in_circle(
		const T max_radius
	) {
		return random_point_in_ring(static_cast<T>(0), max_radius);
	}

	template<class T>
	vec2t<T> randval(
		const vec2t<T> min_a, 
		const vec2t<T> max_a
	) {
		return { 
			randval(min_a.x, max_a.x), 
			randval(min_a.y, max_a.y) 
		};
	}

	template<class T>
	T randval(const augs::minmax<T> m) {
		return randval(m.first, m.second);
	}
};

typedef basic_randomization<std::mt19937> randomization;
typedef basic_randomization<std::minstd_rand0> fast_randomization;
