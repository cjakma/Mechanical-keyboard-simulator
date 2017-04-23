#pragma once
#include <string>

/** Opaque device handle */
typedef struct ALCdevice_struct ALCdevice;
/** Opaque context handle */
typedef struct ALCcontext_struct ALCcontext;

namespace augs {
	class audio_manager {
		ALCdevice* device = nullptr;
		ALCcontext* context = nullptr;
		
		audio_manager(const audio_manager&) = delete;
		audio_manager(audio_manager&&) = delete;
		audio_manager& operator=(const audio_manager&) = delete;
		audio_manager& operator=(audio_manager&&) = delete;

	public:
		static void generate_alsoft_ini(
			const bool hrtf_enabled,
			const unsigned max_number_of_sound_sources
		);

		audio_manager(const std::string output_device_name = "");
		~audio_manager();

		bool make_current();
	};
}