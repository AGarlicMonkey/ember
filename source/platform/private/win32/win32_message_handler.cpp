#include "win32_message_handler.hpp"

namespace ember::platform {
    class win32_window;
}

namespace ember::platform {
    LRESULT CALLBACK process_message(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
        switch(msg) {
            case WM_NCCREATE: {
                //Sotre our win32_window class into the hwnd pointer so we can retrieve it later.
                auto const *const create_struct{ reinterpret_cast<CREATESTRUCTW *>(l_param) };
                auto *const window{ reinterpret_cast<win32_window *>(create_struct->lpCreateParams) };

                //TODO: Can this just be called inside open_window?
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
            } break;
        }

        return DefWindowProc(hwnd, msg, w_param, l_param);
    }
}