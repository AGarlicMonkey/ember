#pragma once

#include <cstddef>
#include <concepts>

namespace ember::memory {
    /**
     * @brief Allocates an object of type T from the global memory pool. This
     * class ensures that only 1 pointer can point to an object at any one time.
     * @tparam T 
     */
    template<typename T>
    class unique_ptr {
        template<typename U>
        friend class unique_ptr;

        //VARIABLES
    private:
        T *ptr{ nullptr };

        //FUNCTIONS
    public:
        unique_ptr();
        unique_ptr(std::nullptr_t);
        explicit unique_ptr(T *object);

        unique_ptr(unique_ptr const &other) = delete;
        unique_ptr(unique_ptr &&other) noexcept;
        template<typename U>
        unique_ptr(unique_ptr<U> &&other) requires std::is_base_of_v<T, U>;

        unique_ptr &operator=(unique_ptr const &other) = delete;
        unique_ptr &operator=(unique_ptr &&other) noexcept;

        ~unique_ptr();

        bool is_valid() const;

        T *get() const;

        void reset();

        T *operator->();
        T const *operator->() const;

        T &operator*();
        T const &operator*() const;

        template<typename T1>
        friend bool operator==(unique_ptr<T1> const &lhs, std::nullptr_t);
        template<typename T1>
        friend bool operator!=(unique_ptr<T1> const &lhs, std::nullptr_t);
    };
}

namespace ember::memory {
    template<typename T, typename... arg_t>
    unique_ptr<T> make_unique(arg_t &&...args);
}

#include "unique_ptr.inl"