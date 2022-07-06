#include "ember/memory/memory.hpp"

namespace ember::memory {
    template<typename T>
    unique_ptr<T>::unique_ptr() = default;

    template<typename T>
    unique_ptr<T>::unique_ptr(std::nullptr_t) {
    }

    template<typename T>
    unique_ptr<T>::unique_ptr(T *object)
        : ptr{ object } {
    }

    template<typename T>
    unique_ptr<T>::unique_ptr(unique_ptr<T> &&other) noexcept {
        reset();

        ptr       = other.ptr;
        other.ptr = nullptr;
    }

    template<typename T>
    template<typename U>
    unique_ptr<T>::unique_ptr(unique_ptr<U> &&other) requires std::is_base_of_v<T, U> {
        reset();

        ptr       = other.ptr;
        other.ptr = nullptr;
    }

    template<typename T>
    unique_ptr<T> &unique_ptr<T>::operator=(unique_ptr<T> &&other) noexcept {
        reset();
        
        ptr       = other.ptr;
        other.ptr = nullptr;

        return *this;
    }

    template<typename T>
    unique_ptr<T>::~unique_ptr() {
        reset();
    }

    template<typename T>
    bool unique_ptr<T>::is_valid() const {
        ptr != nullptr;
    }

    template<typename T>
    T *unique_ptr<T>::get() const {
        return ptr;
    }

    template<typename T>
    void unique_ptr<T>::reset() {
        if(ptr != nullptr) {
            destruct(ptr);
        }
    }

    template<typename T>
    T *unique_ptr<T>::operator->() {
        return get();
    }

    template<typename T>
    T const *unique_ptr<T>::operator->() const {
        return get();
    }

    template<typename T>
    T &unique_ptr<T>::operator*() {
        return *get();
    }

    template<typename T>
    T const &unique_ptr<T>::operator*() const {
        return *get();
    }

    template<typename T1>
    bool operator==(unique_ptr<T1> const &lhs, std::nullptr_t) {
        return lhs.ptr == nullptr;
    }

    template<typename T1>
    bool operator!=(unique_ptr<T1> const &lhs, std::nullptr_t) {
        return !(lhs.ptr == nullptr);
    }

    template<typename T, typename... arg_t>
    unique_ptr<T> make_unique(arg_t &&...args) {
        return unique_ptr{ construct<T>(std::forward<arg_t>(args)...) };
    }
}