#include "ember/memory/memory.hpp"

namespace ember::memory {
    namespace internal {
        template<typename type_t>
        void default_deleter::operator()(type_t *object) {
            destruct(object);
        }
    }

    template<typename type_t, typename deleter_t>
    unique_ptr<type_t, deleter_t>::unique_ptr() = default;

    template<typename type_t, typename deleter_t>
    unique_ptr<type_t, deleter_t>::unique_ptr(std::nullptr_t) {
    }

    template<typename type_t, typename deleter_t>
    unique_ptr<type_t, deleter_t>::unique_ptr(type_t *object)
        : ptr{ object } {
    }

    template<typename type_t, typename deleter_t>
    unique_ptr<type_t, deleter_t>::unique_ptr(type_t *object, deleter_t deleter)
        : ptr{ object }
        , deleter{ std::move(deleter) } {
    }

    template<typename type_t, typename deleter_t>
    unique_ptr<type_t, deleter_t>::unique_ptr(unique_ptr<type_t, deleter_t> &&other) noexcept {
        reset();

        ptr       = other.ptr;
        deleter   = other.deleter;

        other.ptr = nullptr;
    }

    template<typename type_t, typename deleter_t>
    template<typename other_type_t>
    unique_ptr<type_t, deleter_t>::unique_ptr(unique_ptr<other_type_t, deleter_t> &&other) requires std::is_base_of_v<type_t, other_type_t> {
        reset();

        ptr       = other.ptr;
        deleter   = other.deleter;

        other.ptr = nullptr;
    }

    template<typename type_t, typename deleter_t>
    unique_ptr<type_t, deleter_t> &unique_ptr<type_t, deleter_t>::operator=(unique_ptr<type_t, deleter_t> &&other) noexcept {
        reset();

        ptr       = other.ptr;
        deleter   = other.deleter;

        other.ptr = nullptr;

        return *this;
    }

    template<typename type_t, typename deleter_t>
    unique_ptr<type_t, deleter_t>::~unique_ptr() {
        reset();
    }

    template<typename type_t, typename deleter_t>
    bool unique_ptr<type_t, deleter_t>::is_valid() const {
        ptr != nullptr;
    }

    template<typename type_t, typename deleter_t>
    type_t *unique_ptr<type_t, deleter_t>::get() const {
        return ptr;
    }

    template<typename type_t, typename deleter_t>
    void unique_ptr<type_t, deleter_t>::reset() {
        if(ptr != nullptr) {
            deleter(ptr);
            ptr = nullptr;
        }
    }

    template<typename type_t, typename deleter_t>
    type_t *unique_ptr<type_t, deleter_t>::operator->() {
        return get();
    }

    template<typename type_t, typename deleter_t>
    type_t const *unique_ptr<type_t, deleter_t>::operator->() const {
        return get();
    }

    template<typename type_t, typename deleter_t>
    type_t &unique_ptr<type_t, deleter_t>::operator*() {
        return *get();
    }

    template<typename type_t, typename deleter_t>
    type_t const &unique_ptr<type_t, deleter_t>::operator*() const {
        return *get();
    }

    template<typename type_t_1, typename deleter_t>
    bool operator==(unique_ptr<type_t_1, deleter_t> const &lhs, std::nullptr_t) {
        return lhs.ptr == nullptr;
    }

    template<typename type_t_1, typename deleter_t>
    bool operator!=(unique_ptr<type_t_1, deleter_t> const &lhs, std::nullptr_t) {
        return !(lhs.ptr == nullptr);
    }

    template<typename type_t, typename... args_t>
    unique_ptr<type_t> make_unique(args_t &&...args) {
        return unique_ptr{ construct<type_t>(std::forward<args_t>(args)...) };
    }
}