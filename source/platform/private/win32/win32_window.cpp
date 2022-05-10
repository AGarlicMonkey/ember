#include "ember/platform/window.hpp"
#include "win32_input.hpp"

#include <ember/containers/queue.hpp>
#include <ember/core/exception.hpp>
#include <ember/core/log.hpp>
#include <ember/memory/memory.hpp>
#include <new>
#include <windows.h>

EMBER_LOG_CATEGORY(EmberWindowWin32);

using namespace ember::maths;

namespace {
    /**
     * @brief Maps keys like shift/control/alt to their left and right versions. Windows does not do this by default.
     * @return 
     */
    WPARAM map_left_right_keys(WPARAM w_param, LPARAM l_param) {
        WPARAM mapped_param{};

        auto const scancode{ static_cast<UINT>((l_param & 0x00ff0000) >> 16) };

        switch(w_param) {
            case VK_SHIFT:
            case VK_CONTROL:
            case VK_MENU:
                mapped_param = MapVirtualKey(scancode, MAPVK_VK_TO_VSC_EX);
                break;
            default:
                mapped_param = w_param;
                break;
        }

        return mapped_param;
    }
}

namespace ember::platform {
    class win32_window : public window {
        //VARIABLES
    private:
        HWND window_handle{ nullptr };

        static std::uint32_t constexpr max_event_queue_size{ 16 };//Keep an upper limit on cached events incase no one polls a window for a while.
        containers::queue<event> event_queue{};

        //FUNCTIONS
    public:
        win32_window() = delete;
        win32_window(std::string_view title, HINSTANCE instance, WNDCLASSEX const &window_class, DWORD const window_style, RECT const &window_rect, LONG const window_pos_xy) {
            //We do the actual API call for creating a window here so we can use 'this' as the l_param
            window_handle = CreateWindow(
                window_class.lpszClassName,
                title.data(),
                window_style,
                window_pos_xy,
                window_pos_xy,
                window_rect.right - window_rect.left,
                window_rect.bottom - window_rect.top,
                /* window_parent  */ nullptr,
                nullptr,
                instance,
                this);

            EMBER_THROW_IF_FAILED(window_handle != nullptr, exception{ "Failed to create Win32 window." });
        }

        win32_window(win32_window const &other)     = delete;
        win32_window(win32_window &&other) noexcept = default;

        win32_window &operator=(win32_window const &other) = delete;
        win32_window &operator=(win32_window &&other) noexcept = default;

        ~win32_window() = default;

        std::optional<event> get_event() override {
            //Flush messages for this window. These will get queued up inside handle_message
            MSG message;
            while(PeekMessage(&message, window_handle, 0, 0, PM_REMOVE)) {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }

            while(event_queue.size() > max_event_queue_size) {
                event_queue.pop();
            }

            if(!event_queue.empty()) {
                auto event{ event_queue.front() };
                event_queue.pop();

                return event;
            } else {
                return std::nullopt;
            }
        }

        bool is_open() const override {
            return static_cast<bool>(IsWindow(window_handle));
        }

        void close() override {
            CloseWindow(window_handle);
            window_handle = nullptr;
        }

        maths::vec2u get_size() const override {
            EMBER_CHECK(is_open());

            RECT window_rect{};
            GetClientRect(window_handle, &window_rect);

            auto const width{ static_cast<std::uint32_t>(window_rect.right) - static_cast<std::uint32_t>(window_rect.left) };
            auto const height{ static_cast<std::uint32_t>(window_rect.bottom) - static_cast<std::uint32_t>(window_rect.top) };

            return { width, height };
        }

        void *get_native_window() const override {
            return window_handle;
        }

        LRESULT handle_message(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
            POINTS const pt{ MAKEPOINTS(l_param) };
            vec2i const mouse_pos{ pt.x, pt.y };

            switch(message) {
                case WM_CLOSE:
                    //TODO
                    break;

                case WM_KILLFOCUS:
                    //TODO
                    break;

                case WM_SETFOCUS:
                    //TODO
                    break;

                    //Keyboard
                case WM_KEYDOWN:
                case WM_SYSKEYDOWN:
                    //TODO: Check for repeat
                    event_queue.push(key_event{ .type = key_event::type::pressed, .key = convert_key(map_left_right_keys(w_param, l_param)) });
                    break;

                case WM_KEYUP:
                case WM_SYSKEYUP:
                    event_queue.push(key_event{ .type = key_event::type::released, .key = convert_key(map_left_right_keys(w_param, l_param)) });
                    break;

                case WM_CHAR:
                    event_queue.push(char_event{ .character = static_cast<char>(w_param) });
                    break;

                    //Mouse
                case WM_MOUSEMOVE:
                    //TODO: on enter / leave & set / release capture
                    event_queue.push(mouse_event{ .type = mouse_event::type::move, .pos = mouse_pos });
                    break;

                case WM_LBUTTONDOWN:
                    event_queue.push(mouse_event{ .type = mouse_event::type::pressed, .button = mouse_button::left, .pos = mouse_pos });
                    break;

                case WM_LBUTTONUP:
                    event_queue.push(mouse_event{ .type = mouse_event::type::released, .button = mouse_button::left, .pos = mouse_pos });
                    break;

                case WM_RBUTTONDOWN:
                    event_queue.push(mouse_event{ .type = mouse_event::type::pressed, .button = mouse_button::right, .pos = mouse_pos });
                    break;

                case WM_RBUTTONUP:
                    event_queue.push(mouse_event{ .type = mouse_event::type::released, .button = mouse_button::right, .pos = mouse_pos });
                    break;

                case WM_XBUTTONDOWN:
                    event_queue.push(mouse_event{ .type = mouse_event::type::pressed, .button = convert_button(w_param), .pos = mouse_pos });
                    break;

                case WM_XBUTTONUP:
                    event_queue.push(mouse_event{ .type = mouse_event::type::released, .button = convert_button(w_param), .pos = mouse_pos });
                    break;

                case WM_MOUSEWHEEL:
                    if(GET_WHEEL_DELTA_WPARAM(w_param) > 0) {
                        event_queue.push(mouse_event{ .type = mouse_event::type::wheel_up, .pos = mouse_pos });
                    } else {
                        event_queue.push(mouse_event{ .type = mouse_event::type::wheel_down, .pos = mouse_pos });
                    }
                    break;

                    //Window
                case WM_SIZE:
                    //TODO
                    break;
            }

            return DefWindowProc(window_handle, message, w_param, l_param);
        }
    };
}

namespace {
    /**
     * @brief Forwards any win32 messages to it's bound ember::platform::win32_window.
     * @return 
     */
    LRESULT CALLBACK forward_message(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
        auto *const window{ reinterpret_cast<ember::platform::win32_window *>(GetWindowLongPtr(window_handle, GWLP_USERDATA)) };
        return window->handle_message(window_handle, message, w_param, l_param);
    }

    /**
     * @brief Binds a HWND to a ember::platform::win32_window. 
     * @return 
     */
    LRESULT CALLBACK setup_message_handler(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
        switch(message) {
            case WM_NCCREATE: {
                //Store our win32_window class into the hwnd pointer so we can retrieve it later.
                auto const *const create_struct{ reinterpret_cast<CREATESTRUCTW *>(l_param) };
                SetWindowLongPtr(window_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create_struct->lpCreateParams));//lpCreateParams contains the 'this' pointer passed when we create our own window

                //Switch over to the forwarding message handler
                SetWindowLongPtr(window_handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&forward_message));
            } break;
        }

        return DefWindowProc(window_handle, message, w_param, l_param);
    }
}

namespace ember::platform {
    memory::unique_ptr<window> open_window(window::descriptor const &descriptor) {
        HINSTANCE instance{ GetModuleHandle(nullptr) };

        WNDCLASSEX window_class{
            .cbSize        = sizeof(WNDCLASSEX),
            .style         = CS_OWNDC,
            .lpfnWndProc   = setup_message_handler,
            .cbClsExtra    = 0,
            .cbWndExtra    = 0,
            .hInstance     = instance,
            .hIcon         = nullptr,
            .hCursor       = nullptr,
            .hbrBackground = nullptr,
            .lpszMenuName  = nullptr,
            .lpszClassName = "ember_window_class",
        };
        RegisterClassEx(&window_class);

        DWORD window_style{ WS_VISIBLE };
        RECT window_rect{
            .left   = 0,
            .top    = 0,
            .right  = static_cast<std::int32_t>(descriptor.size.x),
            .bottom = static_cast<std::int32_t>(descriptor.size.y),
        };
        //HWND window_parent{ nullptr }; //TODO: child windows
        LONG window_pos_xy{ 0 };

        window_style |= WS_OVERLAPPEDWINDOW;
        AdjustWindowRect(&window_rect, window_style, FALSE);
        window_pos_xy = CW_USEDEFAULT;

        auto window{ memory::make_unique<win32_window>(descriptor.title, instance, window_class, window_style, window_rect, window_pos_xy) };

        EMBER_LOG(EmberWindowWin32, log_level::info, "Created new window of X:{0} Y:{1}", descriptor.size.x, descriptor.size.y);

        return window;
    }
}