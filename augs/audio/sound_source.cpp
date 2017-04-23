#include "sound_source.h"
#include "sound_buffer.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <AL/efx.h>

#include "augs/al_log.h"
#include "augs/math/vec2.h"
#include "augs/math/si_scaling.h"

#define TRACE_PARAMETERS 0
#define TRACE_CONSTRUCTORS_DESTRUCTORS 0
#define Y_IS_Z 1

#if TRACE_CONSTRUCTORS_DESTRUCTORS
int g_num_sources = 0;
#endif

namespace augs {
	sound_source::sound_source() {
		AL_CHECK(alGenSources(1, &id));
#if TRACE_CONSTRUCTORS_DESTRUCTORS
		++g_num_sources;
		LOG("alGenSources: %x (now %x sources)", id, g_num_sources);
#endif
		AL_CHECK(alSourcef(id, AL_PITCH, 1));
		AL_CHECK(alSourcef(id, AL_GAIN, 1));
		AL_CHECK(alSourcei(id, AL_LOOPING, AL_FALSE));

		initialized = true;
	}

	void sound_source::destroy() {
		if (initialized) {
#if TRACE_CONSTRUCTORS_DESTRUCTORS
			--g_num_sources;
			LOG("alDeleteSources: %x (now %x sources)", id, g_num_sources);
#endif
			AL_CHECK(alDeleteSources(1, &id));
			initialized = false;
			attached_buffer = nullptr;
		}
	}

	sound_source::~sound_source() {
		destroy();
	}

	sound_source::sound_source(sound_source&& b) {
		*this = std::move(b);
	}

	sound_source& sound_source::operator=(sound_source&& b) {
		std::swap(initialized, b.initialized);
		std::swap(id, b.id);
		std::swap(attached_buffer, b.attached_buffer);
		return *this;
	}

	ALuint sound_source::get_id() const {
		return id;
	}

	sound_source::operator ALuint() const {
		return get_id();
	}

	void sound_source::play() const {
		AL_CHECK(alSourcePlay(id));
	}
	
	void sound_source::seek_to(const float seconds) const {
		AL_CHECK(alSourcef(id, AL_SEC_OFFSET, seconds));
	}
	
	float sound_source::get_time_in_seconds() const {
		float seconds = 0.f;
		AL_CHECK(alGetSourcef(id, AL_SEC_OFFSET, &seconds));
		return seconds;
	}

	void sound_source::stop() const {
		AL_CHECK(alSourceStop(id));
	}
	
	void sound_source::set_looping(const bool loop) const {
		AL_CHECK(alSourcei(id, AL_LOOPING, loop));
#if TRACE_PARAMETERS
		LOG_NVPS(loop);
#endif
	}

	void sound_source::set_pitch(const float pitch) const {
		AL_CHECK(alSourcef(id, AL_PITCH, pitch));
#if TRACE_PARAMETERS
		LOG_NVPS(pitch);
#endif
	}

	void sound_source::set_gain(const float gain) const {
		AL_CHECK(alSourcef(id, AL_GAIN, gain));
#if TRACE_PARAMETERS
		LOG_NVPS(gain);
#endif
	}

	void sound_source::set_air_absorption_factor(const float absorption) const {
		AL_CHECK(alSourcef(id, AL_AIR_ABSORPTION_FACTOR, absorption));
#if TRACE_PARAMETERS
		LOG_NVPS(absorption);
#endif
	}

	void sound_source::set_velocity(const si_scaling si, vec2 v) const {
		v = si.get_meters(v);
#if Y_IS_Z
		AL_CHECK(alSource3f(id, AL_VELOCITY, v.x, 0.f, v.y));
#else
		AL_CHECK(alSource3f(id, AL_VELOCITY, v.x, v.y, 0.f));
#endif
#if TRACE_PARAMETERS
		LOG_NVPS(v);
#endif
	}

	void sound_source::set_position(const si_scaling si, vec2 pos) const {
		pos = si.get_meters(pos);

#if Y_IS_Z
		AL_CHECK(alSource3f(id, AL_POSITION, pos.x, 0.f, pos.y));
#else
		AL_CHECK(alSource3f(id, AL_POSITION, pos.x, pos.y, 0.f));
#endif
#if TRACE_PARAMETERS
		LOG_NVPS(pos);
#endif
	}

	void sound_source::set_max_distance(const si_scaling si, const float distance) const {
		const auto passed_distance = si.get_meters(distance);

		AL_CHECK(alSourcef(id, AL_MAX_DISTANCE, passed_distance));
#if TRACE_PARAMETERS
		LOG_NVPS(passed_distance);
#endif
	}

	void sound_source::set_reference_distance(const si_scaling si, const float distance) const {
		const auto passed_distance = si.get_meters(distance);

		AL_CHECK(alSourcef(id, AL_REFERENCE_DISTANCE, passed_distance));
#if TRACE_PARAMETERS
		LOG_NVPS(passed_distance);
#endif
	}

	void sound_source::set_direct_channels(const bool flag) const {
		AL_CHECK(alSourcei(id, AL_DIRECT_CHANNELS_SOFT, flag));

#if TRACE_PARAMETERS
		LOG_NVPS(flag);
#endif
	}

	float sound_source::get_gain() const {
		float gain = 0.f;
		AL_CHECK(alGetSourcef(id, AL_GAIN, &gain));
		return gain;
	}

	float sound_source::get_pitch() const {
		float pitch = 0.f;
		AL_CHECK(alGetSourcef(id, AL_PITCH, &pitch));
		return pitch;
	}

	bool sound_source::is_playing() const {
		ALenum state = 0xdeadbeef;
		AL_CHECK(alGetSourcei(id, AL_SOURCE_STATE, &state));
		return state == AL_PLAYING;
	}

	void sound_source::bind_buffer(const single_sound_buffer& buf) {
		attached_buffer = &buf;
		AL_CHECK(alSourcei(id, AL_BUFFER, buf.get_id()));
#if TRACE_PARAMETERS
		LOG_NVPS(buf.get_id());
#endif
	}

	void sound_source::unbind_buffer() {
		attached_buffer = nullptr;
		AL_CHECK(alSourcei(id, AL_BUFFER, NULL));
	}
	
	const single_sound_buffer* sound_source::get_bound_buffer() const {
		return attached_buffer;
	}

	void set_listener_position(const si_scaling si, vec2 pos) {
		pos = si.get_meters(pos);

#if Y_IS_Z
		AL_CHECK(alListener3f(AL_POSITION, pos.x, 0.f, pos.y));
#else
		AL_CHECK(alListener3f(AL_POSITION, pos.x, pos.y, 0.f));
#endif
#if TRACE_PARAMETERS
		LOG_NVPS(pos);
#endif
	}

	void set_listener_velocity(const si_scaling si, vec2 v) {
		v = si.get_meters(v);

#if Y_IS_Z
		AL_CHECK(alListener3f(AL_VELOCITY, v.x, 0.f, v.y));
#else
		AL_CHECK(alListener3f(AL_VELOCITY, v.x, v.y, 0.f));
#endif

#if TRACE_PARAMETERS
		LOG_NVPS(v);
#endif
	}

	void set_listener_orientation(const std::array<float, 6> data) {
		AL_CHECK(alListenerfv(AL_ORIENTATION, data.data()));
	}
}
