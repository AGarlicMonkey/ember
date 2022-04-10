#include <new>

namespace ember::memory {
    template<typename object_t, typename... arg_t>
    object_t *construct(arg_t &&...args) {
        std::byte *const memory{ alloc(sizeof(object_t), alignof(object_t)) };
        object_t *object{ new(memory) object_t{ std::forward<arg_t>(args)... } };

        return object;
    }

    template<typename object_t>
    void destruct(object_t *&object) {
        object->~object_t();
        auto *memory{ reinterpret_cast<std::byte *>(object) };
        free(memory);

        object = nullptr;
    }
}