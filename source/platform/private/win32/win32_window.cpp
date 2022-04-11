#include "ember/platform/window.hpp"

#include <ember/core/exception.hpp>
#include <ember/core/log.hpp>
#include <ember/memory/memory.hpp>
#include <new>
#include <windows.h>

EMBER_LOG_CATEGORY(EmberWindowWin32);

namespace ember::platform {
    class win32_window : public window {
        //VARIABLES
    private:
        HWND window_handle{ nullptr };

        //FUNCTIONS
    public:
        win32_window() = delete;
        win32_window(HWND window_handle)
            : window_handle{ window_handle } {
        }

        win32_window(win32_window const &other)     = delete;
        win32_window(win32_window &&other) noexcept = default;

        win32_window &operator=(win32_window const &other) = delete;
        win32_window &operator=(win32_window &&other) noexcept = default;

        ~win32_window() = default;

        bool is_open() const override {
            return static_cast<bool>(IsWindow(window_handle));
        }

        void close() override {
            CloseWindow(window_handle);
            window_handle = nullptr;
        }
    };

    LRESULT CALLBACK process_message(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
        return DefWindowProc(hwnd, msg, w_param, l_param);
    }

    memory::unique_ptr<window> open_window(window::descriptor const &descriptor) {
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

        HWND hwnd = CreateWindow(
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

        EMBER_THROW_IF_FAILED(hwnd != nullptr, exception{ "Failed to create Win32 window." });
        EMBER_LOG(EmberWindowWin32, log_level::info, "Created new window of X:{0} Y:{1}", descriptor.size.x, descriptor.size.y);

        return memory::make_unique<win32_window>(hwnd);
    }
}