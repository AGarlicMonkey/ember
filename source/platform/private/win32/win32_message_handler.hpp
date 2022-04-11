#pragma once

#include <windows.h>

namespace ember::platform {
    LRESULT CALLBACK process_message(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
}