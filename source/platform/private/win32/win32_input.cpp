#include "win32_input.hpp"

#include <windows.h>

namespace ember::inline platform {
    mouse_button convert_button(std::uint8_t button_code) {
        switch(GET_KEYSTATE_WPARAM(button_code)) {
            case MK_LBUTTON:
                return mouse_button::_1;
            case MK_RBUTTON:
                return mouse_button::_2;
            case MK_MBUTTON:
                return mouse_button::_3;
            case MK_XBUTTON1:
                return mouse_button::_4;
            case MK_XBUTTON2:
                return mouse_button::_5;
            default:
                return mouse_button::undefined;
        }
    }

    key convert_key(std::uint8_t key_code) {
        switch(key_code) {
            // clang-format off
            case VK_SPACE:      return key::space;
            case VK_OEM_7:      return key::apostrophe;
            case VK_OEM_COMMA:  return key::comma;
            case VK_OEM_MINUS:  return key::minus;
            case VK_OEM_PERIOD: return key::period;
            case VK_OEM_2:      return key::slash;
            case '0':           return key::_0;
            case '1':           return key::_1;
            case '2':           return key::_2;
            case '3':           return key::_3;
            case '4':           return key::_4;
            case '5':           return key::_5;
            case '6':           return key::_6;
            case '7':           return key::_7;
            case '8':           return key::_8;
            case '9':           return key::_9;
            case VK_OEM_1:      return key::semicolon;
            case 'A':           return key::a;
            case 'B':           return key::b;
            case 'C':           return key::c;
            case 'D':           return key::d;
            case 'E':           return key::e;
            case 'F':           return key::f;
            case 'G':           return key::g;
            case 'H':           return key::h;
            case 'I':           return key::i;
            case 'J':           return key::j;
            case 'K':           return key::k;
            case 'L':           return key::l;
            case 'M':           return key::m;
            case 'N':           return key::n;
            case 'O':           return key::o;
            case 'P':           return key::p;
            case 'Q':           return key::q;
            case 'R':           return key::r;
            case 'S':           return key::s;
            case 'T':           return key::t;
            case 'U':           return key::u;
            case 'V':           return key::v;
            case 'W':           return key::w;
            case 'X':           return key::x;
            case 'Y':           return key::y;
            case 'Z':           return key::z;
            case VK_OEM_4:      return key::bracket_left;
            case VK_OEM_6:      return key::bracket_right;
            case VK_OEM_5:      return key::back_slash;
            case VK_OEM_3:      return key::grave_accent;
            case VK_ESCAPE:     return key::escape;
            case VK_ACCEPT:     return key::enter;
            case VK_TAB:        return key::tab;
            case VK_BACK:       return key::backspace;
            case VK_INSERT:     return key::insert;
            case VK_DELETE:     return key::del;
            case VK_RIGHT:      return key::right;
            case VK_LEFT:       return key::left;
            case VK_DOWN:       return key::down;
            case VK_UP:         return key::up;
            case VK_PRIOR:      return key::page_up;
            case VK_NEXT:       return key::page_down;
            case VK_HOME:       return key::home;
            case VK_END:        return key::end;
            case VK_CAPITAL:    return key::caps_lock;
            case VK_SCROLL:     return key::scroll_lock;
            case VK_NUMLOCK:    return key::num_lock;
            case VK_PRINT:      return key::print_screen;
            case VK_PAUSE:      return key::pause;
            case VK_F1:         return key::f1;
            case VK_F2:         return key::f2;
            case VK_F3:         return key::f3;
            case VK_F4:         return key::f4;
            case VK_F5:         return key::f5;
            case VK_F6:         return key::f6;
            case VK_F7:         return key::f7;
            case VK_F8:         return key::f8;
            case VK_F9:         return key::f9;
            case VK_F10:        return key::f10;
            case VK_F11:        return key::f11;
            case VK_F12:        return key::f12;
            case VK_F13:        return key::f13;
            case VK_F14:        return key::f14;
            case VK_F15:        return key::f15;
            case VK_F16:        return key::f16;
            case VK_F17:        return key::f17;
            case VK_F18:        return key::f18;
            case VK_F19:        return key::f19;
            case VK_F20:        return key::f20;
            case VK_F21:        return key::f21;
            case VK_F22:        return key::f22;
            case VK_F23:        return key::f23;
            case VK_F24:        return key::f24;
            case VK_NUMPAD0:    return key::num_pad_0;
            case VK_NUMPAD1:    return key::num_pad_1;
            case VK_NUMPAD2:    return key::num_pad_2;
            case VK_NUMPAD3:    return key::num_pad_3;
            case VK_NUMPAD4:    return key::num_pad_4;
            case VK_NUMPAD5:    return key::num_pad_5;
            case VK_NUMPAD6:    return key::num_pad_6;
            case VK_NUMPAD7:    return key::num_pad_7;
            case VK_NUMPAD8:    return key::num_pad_8;
            case VK_NUMPAD9:    return key::num_pad_9;
            case VK_DECIMAL:    return key::num_pad_decimal;
            case VK_DIVIDE:     return key::num_pad_divide;
            case VK_MULTIPLY:   return key::num_pad_multiply;
            case VK_SUBTRACT:   return key::num_pad_subtract;
            case VK_ADD:        return key::num_pad_add;
            case VK_LSHIFT:     return key::shift_left;
            case VK_RSHIFT:     return key::shift_right;
            case VK_LCONTROL:   return key::control_left;
            case VK_RCONTROL:   return key::control_right;
            case VK_LMENU:      return key::alt_left;
            case VK_RMENU:      return key::alt_right;
            case VK_LWIN:       return key::super_left;
            case VK_RWIN:       return key::super_right;
            default:            return key::undefined;
                // clang-format on
        }
    }
}