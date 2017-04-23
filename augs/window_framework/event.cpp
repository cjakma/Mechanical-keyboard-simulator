#include "event.h"
#include "augs/ensure.h"

namespace augs {
	namespace window {
		namespace event {
			change::change() {
				std::memset(this, 0, sizeof(change));
			}

			key_change change::get_key_change() const {
				switch (msg) {
				case message::ltripleclick: return key_change::PRESSED;
				case message::keydown: return key_change::PRESSED;
				case message::keyup:  return key_change::RELEASED;
				case message::ldoubleclick:  return key_change::PRESSED;
				case message::mdoubleclick: return key_change::PRESSED;
				case message::rdoubleclick: return key_change::PRESSED;
				case message::ldown: return key_change::PRESSED;
				case message::lup: return key_change::RELEASED;
				case message::mdown: return key_change::PRESSED;
				case message::mup: return key_change::RELEASED;
				case message::xdown: return key_change::PRESSED;
				case message::xup: return key_change::RELEASED;
				case message::rdown: return key_change::PRESSED;
				case message::rup: return key_change::RELEASED;
				default: return key_change::NO_CHANGE; break;
				}
			}
			
			bool change::operator==(const change& c) const {
				return !std::memcmp(this, &c, sizeof(change));
			}
			
			bool change::uses_mouse() const {
				return msg == message::mousemotion
					|| (get_key_change() != key_change::NO_CHANGE && msg != message::keydown && msg != message::keyup)
					;
			}

			bool change::was_any_key_pressed() const {
				return get_key_change() == key_change::PRESSED;
			}

			bool change::was_any_key_released() const {
				return get_key_change() == key_change::RELEASED;
			}

			bool change::was_key_pressed(const keys::key k) const {
				return was_any_key_pressed() && key.key == k;
			}
			
			bool change::was_key_released(const keys::key k) const {
				return was_any_key_released() && key.key == k;
			}

			void state::apply(const change& dt) {
				const auto ch = dt.get_key_change();

				if (ch == key_change::PRESSED) {
					keys.set(static_cast<size_t>(dt.key.key), true);
				}
				else if (ch == key_change::RELEASED) {
					keys.set(static_cast<size_t>(dt.key.key), false);
				}
				else if (dt.msg == message::mousemotion) {
					mouse.pos += dt.mouse.rel;
					mouse.pos.clamp_from_zero_to(vec2i{ screen_size.x - 1, screen_size.y - 1 });

					if (!get_mouse_key(0)) {
						mouse.ldrag.x = mouse.pos.x;
						mouse.ldrag.y = mouse.pos.y;
					}
					
					if (!get_mouse_key(1)) {
						mouse.rdrag.x = mouse.pos.x;
						mouse.rdrag.y = mouse.pos.y;
					}
				}
			}

			bool state::get_mouse_key(const unsigned n) const {
				switch (n) {
				case 0: return keys.test(static_cast<size_t>(keys::key::LMOUSE));
				case 1: return keys.test(static_cast<size_t>(keys::key::RMOUSE));
				case 2: return keys.test(static_cast<size_t>(keys::key::MMOUSE));
				case 3: return keys.test(static_cast<size_t>(keys::key::MOUSE4));
				case 4: return keys.test(static_cast<size_t>(keys::key::MOUSE5));
				default: ensure(false); return false;
				}
			}
			
			bool state::is_set(const keys::key k) const {
				return keys.test(static_cast<size_t>(k));
			}

			void state::unset_keys() {
				keys.reset();
			}

			namespace keys {
				bool is_numpad_key(const key k) {
					if(static_cast<int>(k) >= static_cast<int>(key::NUMPAD0) && static_cast<int>(k) <= static_cast<int>(key::NUMPAD9)) return true;
					return false;
				}

				std::wstring key_to_wstring(const key k) {
					switch (k) {
						case key::INVALID: return L"INVALID"; break;
						case key::LMOUSE: return L"Left Mouse Button"; break;
						case key::RMOUSE: return L"Right Mouse Button"; break;
						case key::MMOUSE: return L"Middle Mouse Button"; break;
						case key::MOUSE4: return L"Mouse Button 4"; break;
						case key::MOUSE5: return L"Mouse Button 5"; break;
						case key::CANCEL: return L"Cancel"; break;
						case key::BACKSPACE: return L"Backspace"; break;
						case key::TAB: return L"Tab"; break;
						case key::CLEAR: return L"Clear"; break;
						case key::ENTER: return L"Enter"; break;
						case key::SHIFT: return L"Shift"; break;
						case key::CTRL: return L"Control"; break;
						case key::ALT: return L"Alt"; break;
						case key::PAUSE: return L"Pause"; break;
						case key::CAPSLOCK: return L"Caps Lock"; break;
						case key::ESC: return L"Escape"; break;
						case key::SPACE: return L"Space"; break;
						case key::PAGEUP: return L"Page Up"; break;
						case key::PAGEDOWN: return L"Page Down"; break;
						case key::END: return L"End"; break;
						case key::HOME: return L"Home"; break;
						case key::LEFT: return L"Left Arrow"; break;
						case key::UP: return L"Up Arrow"; break;
						case key::RIGHT: return L"Right Arrow"; break;
						case key::DOWN: return L"Down Arrow"; break;
						case key::SELECT: return L"Select"; break;
						case key::PRINT: return L"Print"; break;
						case key::EXECUTE: return L"Execute"; break;
						case key::PRINTSCREEN: return L"Print Screen"; break;
						case key::INSERT: return L"Insert"; break;
						case key::DEL: return L"Del"; break;
						case key::HELP: return L"Help"; break;
						case key::LWIN: return L"Left Windows Key"; break;
						case key::RWIN: return L"Right Windows Key"; break;
						case key::APPS: return L"Apps"; break;
						case key::SLEEP: return L"Sleep"; break;
						case key::NUMPAD0: return L"Numpad 0"; break;
						case key::NUMPAD1: return L"Numpad 1"; break;
						case key::NUMPAD2: return L"Numpad 2"; break;
						case key::NUMPAD3: return L"Numpad 3"; break;
						case key::NUMPAD4: return L"Numpad 4"; break;
						case key::NUMPAD5: return L"Numpad 5"; break;
						case key::NUMPAD6: return L"Numpad 6"; break;
						case key::NUMPAD7: return L"Numpad 7"; break;
						case key::NUMPAD8: return L"Numpad 8"; break;
						case key::NUMPAD9: return L"Numpad 9"; break;
						case key::MULTIPLY: return L"Multiply"; break;
						case key::ADD: return L"Add"; break;
						case key::SEPARATOR: return L"Separator"; break;
						case key::SUBTRACT: return L"Subtract"; break;
						case key::DECIMAL: return L"Decimal"; break;
						case key::DIVIDE: return L"Divide"; break;
						case key::F1: return L"F1"; break;
						case key::F2: return L"F2"; break;
						case key::F3: return L"F3"; break;
						case key::F4: return L"F4"; break;
						case key::F5: return L"F5"; break;
						case key::F6: return L"F6"; break;
						case key::F7: return L"F7"; break;
						case key::F8: return L"F8"; break;
						case key::F9: return L"F9"; break;
						case key::F10: return L"F10"; break;
						case key::F11: return L"F11"; break;
						case key::F12: return L"F12"; break;
						case key::F13: return L"F13"; break;
						case key::F14: return L"F14"; break;
						case key::F15: return L"F15"; break;
						case key::F16: return L"F16"; break;
						case key::F17: return L"F17"; break;
						case key::F18: return L"F18"; break;
						case key::F19: return L"F19"; break;
						case key::F20: return L"F20"; break;
						case key::F21: return L"F21"; break;
						case key::F22: return L"F22"; break;
						case key::F23: return L"F23"; break;
						case key::F24: return L"F24"; break;
						case key::A: return L"A"; break;
						case key::B: return L"B"; break;
						case key::C: return L"C"; break;
						case key::D: return L"D"; break;
						case key::E: return L"E"; break;
						case key::F: return L"F"; break;
						case key::G: return L"G"; break;
						case key::H: return L"H"; break;
						case key::I: return L"I"; break;
						case key::J: return L"J"; break;
						case key::K: return L"K"; break;
						case key::L: return L"L"; break;
						case key::M: return L"M"; break;
						case key::N: return L"N"; break;
						case key::O: return L"O"; break;
						case key::P: return L"P"; break;
						case key::Q: return L"Q"; break;
						case key::R: return L"R"; break;
						case key::S: return L"S"; break;
						case key::T: return L"T"; break;
						case key::U: return L"U"; break;
						case key::V: return L"V"; break;
						case key::W: return L"W"; break;
						case key::X: return L"X"; break;
						case key::Y: return L"Y"; break;
						case key::Z: return L"Z"; break;
						case key::_0: return L"0"; break;
						case key::_1: return L"1"; break;
						case key::_2: return L"2"; break;
						case key::_3: return L"3"; break;
						case key::_4: return L"4"; break;
						case key::_5: return L"5"; break;
						case key::_6: return L"6"; break;
						case key::_7: return L"7"; break;
						case key::_8: return L"8"; break;
						case key::_9: return L"9"; break;
						case key::NUMLOCK: return L"Num Lock"; break;
						case key::SCROLL: return L"Scroll"; break;
						case key::LSHIFT: return L"Left Shift"; break;
						case key::RSHIFT: return L"Right Shift"; break;
						case key::LCTRL: return L"Left Control"; break;
						case key::RCTRL: return L"Right Control"; break;
						case key::LALT: return L"Left Alt"; break;
						case key::RALT: return L"Right Alt"; break;
						case key::DASH: return L"Dash"; break;
						default: return L"Invalid key"; break;
					}
				}
				
				key wstring_to_key(const std::wstring& wstr) {
					for (key i = key::INVALID; i < key::COUNT; i = key(int(i) + 1)) {
						if(key_to_wstring(i) == wstr) {
							return i;
						}
					}

					return key::INVALID;
				}
			}
		}
	}
}