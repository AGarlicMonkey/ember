#include "ember/platform/window.hpp"

#include <ember/core/exception.hpp>
#include <ember/core/log.hpp>
#include <ember/memory/memory.hpp>
#include <new>
#include <windows.h>

EMBER_LOG_CATEGORY(EmberWindowWin32);

namespace ember::platform {
    struct win32_handle {
        HWND hwnd{ nullptr };
    };
}

namespace ember::platform {
    LRESULT CALLBACK process_message(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
        return DefWindowProc(hwnd, msg, w_param, l_param);
    }

    window_handle open_window(window_descriptor const &descriptor) {
        win32_handle *const handle{ memory::construct<win32_handle>() };

        HINSTANCE instance{ GetModuleHandle(nullptr) };

        WNDCLASSEX window_class{
            .cbSize        = sizeof(WNDCLASSEX),
            .style         = CS_OWNDC,
            .lpfnWndProc   = process_message,
            .cbClsExtra    = 0,
            .cbWndExtra    = 0,
            .hInstance     = instance,
            .hIcon         = nullptr,
            .hCursor       = nullptr,
            .hbrBackground = nullptr,
            .lpszMenuName  = nullptr,
            .lpszClassName = /* WindowsWindow::className */ "ember_window_class",
        };
        RegisterClassEx(&window_class);

        DWORD window_style{ WS_VISIBLE };
        RECT window_rect{
            .left   = 0,
            .top    = 0,
            .right  = descriptor.size.x,
            .bottom = descriptor.size.y,
        };
        //HWND window_parent{ nullptr };
        LONG window_pos_xy{ 0 };

        //TODO: child windows
        // if() {
        //     window_style |= WS_CHILD;
        //     window_parent = ...;
        // } else {
        window_style |= WS_OVERLAPPEDWINDOW;
        AdjustWindowRect(&window_rect, window_style, FALSE);
        window_pos_xy = CW_USEDEFAULT;
        //}

        handle->hwnd = CreateWindow(
            window_class.lpszClassName,
            descriptor.title.c_str(),
            window_style,
            window_pos_xy,
            window_pos_xy,
            window_rect.right - window_rect.left,
            window_rect.bottom - window_rect.top,
            /* window_parent  */ nullptr,
            0,
            instance,
            /* this */ nullptr);

        EMBER_THROW_IF_FAILED(handle->hwnd != nullptr, exception{ "Failed to create Win32 window." });
        EMBER_LOG(EmberWindowWin32, log_level::info, "Created new window of X:{0} Y:{0}", descriptor.size.x, descriptor.size.y);

        return window_handle{
            .platform_handle = handle,
        };
    }

    bool is_window_open(window_handle const &handle) {
        if(handle.platform_handle == nullptr) {
            return false;
        }

        auto *win_handle{ reinterpret_cast<win32_handle *>(handle.platform_handle) };
        return static_cast<bool>(IsWindow(win_handle->hwnd));
    }

    void close_window(window_handle &handle) {
        if(handle.platform_handle == nullptr) {
            return;
        }

        auto *win_handle{ reinterpret_cast<win32_handle *>(handle.platform_handle) };
        CloseWindow(win_handle->hwnd);

        memory::destruct(win_handle);

        handle.platform_handle = nullptr;

        EMBER_LOG(EmberWindowWin32, log_level::trace, "Closed Win32 window");
    }

}