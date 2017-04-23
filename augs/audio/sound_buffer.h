#pragma once
#include <vector>

typedef unsigned int ALuint;

class assets_manager;

namespace augs {
	class single_sound_buffer {
		double computed_length_in_seconds = 0.0;
		bool initialized = false;
		ALuint id = 0;

	public:
		struct data_type {
			std::vector<int16_t> samples;
			int frequency = 0;
			int channels = 0;

			double compute_length_in_seconds() const;
			int get_format() const;
		};

		single_sound_buffer() = default;
		~single_sound_buffer();

		single_sound_buffer& operator=(single_sound_buffer&&);
		single_sound_buffer(single_sound_buffer&&);

		single_sound_buffer(const single_sound_buffer&) = delete;
		single_sound_buffer& operator=(const single_sound_buffer&) = delete;

		void set_data(const data_type&);
		// data_type get_data() const;
		bool is_set() const;

		double get_length_in_seconds() const;

		ALuint get_id() const;
		operator ALuint() const;
	};

	struct sound_buffer_logical_meta {
		// GEN INTROSPECTOR struct augs::sound_buffer_logical_meta
		float max_duration_in_seconds = 0.f;
		unsigned num_of_variations = 0xdeadbeef;
		// END GEN INTROSPECTOR
	};

	class sound_buffer {
		struct variation {
			int original_channels = 0;
			single_sound_buffer mono;
			single_sound_buffer stereo;

			void set_data(const single_sound_buffer::data_type&, const bool generate_mono);

			single_sound_buffer& request_original();
			single_sound_buffer& request_mono();
			single_sound_buffer& request_stereo();
			const single_sound_buffer& request_original() const;
			const single_sound_buffer& request_mono() const;
			const single_sound_buffer& request_stereo() const;
		};

		std::vector<variation> variations;
	
	public:
		void from_file(const std::string filename, const bool generate_mono = true);

		size_t get_num_variations() const;
		variation& get_variation(const size_t);
		const variation& get_variation(const size_t) const;

		ALuint get_id() const;
		operator ALuint() const;

		sound_buffer_logical_meta get_logical_meta(const assets_manager& manager) const;
	};

	single_sound_buffer::data_type get_sound_samples_from_file(const std::string);
	std::vector<int16_t> mix_stereo_to_mono(const std::vector<int16_t>&);
}