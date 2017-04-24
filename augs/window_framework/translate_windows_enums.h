#pragma once
#include "augs/window_framework/event.h"

augs::window::event::message translate_enum(unsigned int);
augs::window::event::keys::key translate_virtual_key(const unsigned int);
augs::window::event::keys::key translate_key_with_lparam(const unsigned int lParam, unsigned int);
