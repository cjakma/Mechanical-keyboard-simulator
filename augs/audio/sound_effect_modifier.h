#pragma once
#include "augs/padding_byte.h"

namespace augs {
	struct sound_effect_modifier {
		// GEN INTROSPECTOR struct augs::sound_effect_modifier
		float gain = 1.f;
		float pitch = 1.f;
		float max_distance = 1920.f * 3.f;
		float reference_distance = 0.f;
		short repetitions = 1;
		bool fade_on_exit = true;
		padding_byte pad;
		// END GEN INTROSPECTOR
	};
}