#pragma once
#include <type_traits>
#include "augs/templates/introspection_traits.h"
#include "augs/templates/conditional_call.h"

namespace augs {
	struct introspection_access;

	template <
		class F, 
		class Instance, 
		class... Instances
	>
	void introspect(
		F&& callback,
		Instance&& t,
		Instances&&... tn
	) {
		augs::introspection_access::introspect_body(
			static_cast<std::decay_t<Instance>*>(nullptr), 
			std::forward<F>(callback),
			std::forward<Instance>(t), 
			std::forward<Instances>(tn)...
		);
	}

	/*
		Because the introspected members will always be passed to the callback as some kind of reference,
		it makes sense to remove the references from the arguments to predicates 
		to simplify the implementation of "should recurse" and "call valid" predicates.
	
		It is a separate function also for the reason that the compiler has troubles unpacking the parameter packs in lambdas,
		when the pattern is more complex.
	*/

	template <
		template <class...> class pred,
		class... Args
	>
	inline constexpr bool eval() {
		return pred<std::remove_reference_t<Args>...>::value;
	}

	/*
		Explanation of call_valid_predicate and should_recurse_predicate:

		if should_recurse_predicate returns true on a given set of types
			invoke the callback upon the types
		if should_recurse_predicate returns true on a given set of types and none of them is an introspective leaf
			recurse (static_assert ensures that introspectors exist at this point)

		I pass the arguments for the lambda via conditional_call's Args&& right under the lambda itself, 
		instead of letting them being captured by [&] - compiler is faulty when it comes to capturing parameter packs.
	*/

	template <
		template <class...> class call_valid_predicate,
		template <class...> class should_recurse_predicate,
		bool stop_recursion_if_valid,
		unsigned current_depth,
		class F,
		class G,
		class H,
		class... Instances
	>
	void introspect_recursive_with_prologues(
		F&& member_callback,
		G&& recursion_prologue,
		H&& recursion_epilogue,
		Instances&&... introspected_instances
	) {
		introspect(
			[&](
				auto&& label, 
				auto&&... args
			) {
				conditional_call<
					eval<call_valid_predicate, decltype(args)...>()
				> () (
					[&member_callback](auto&& passed_label, auto&&... passed_args) {
						member_callback(
							std::forward<decltype(passed_label)>(passed_label),
							std::forward<decltype(passed_args)>(passed_args)...
						);
					},
					std::forward<decltype(label)>(label),
					std::forward<decltype(args)>(args)...
				);

				conditional_call<
					eval<should_recurse_predicate, decltype(args)...>()
					&& eval<at_least_one_is_not_introspective_leaf, decltype(args)...>()
					&& !(eval<call_valid_predicate, decltype(args)...>() && stop_recursion_if_valid)
				>()(
					[&member_callback, &recursion_prologue, &recursion_epilogue](auto&& passed_label, auto&&... passed_args) {
						static_assert(eval<have_introspects, decltype(passed_args)...>(), "Recursion requested on type(s) without introspectors!");
						
						recursion_prologue(
							current_depth,
							std::forward<decltype(passed_label)>(passed_label),
							std::forward<decltype(passed_args)>(passed_args)...
						);

						introspect_recursive_with_prologues <
							call_valid_predicate,
							should_recurse_predicate,
							stop_recursion_if_valid,
							current_depth + 1u
						> (
							std::forward<F>(member_callback),
							std::forward<G>(recursion_prologue),
							std::forward<H>(recursion_epilogue),
							std::forward<decltype(passed_args)>(passed_args)...
						);

						recursion_epilogue(
							current_depth,
							std::forward<decltype(passed_label)>(passed_label),
							std::forward<decltype(passed_args)>(passed_args)...
						);
					},
					std::forward<decltype(label)>(label),
					std::forward<decltype(args)>(args)...
				);
			},
			std::forward<Instances>(introspected_instances)...
		);
	}

	template <
		template <class...> class call_valid_predicate,
		template <class...> class should_recurse_predicate,
		bool stop_recursion_if_valid,
		class F,
		class... Instances
	>
	void introspect_recursive(
		F&& member_callback,
		Instances&&... introspected_instances
	) {
		introspect_recursive_with_prologues<
			call_valid_predicate,
			should_recurse_predicate,
			stop_recursion_if_valid,
			0u
		>(
			std::forward<F>(member_callback),
			no_prologue(),
			no_epilogue(),
			std::forward<Instances>(introspected_instances)...
		);
	}
}