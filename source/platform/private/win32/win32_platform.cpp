#include "ember/platform/platform.hpp"

#include <windows.h>

namespace ember::platform {
    void pump_messages() {
        MSG message;
        while(PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)){
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }
}