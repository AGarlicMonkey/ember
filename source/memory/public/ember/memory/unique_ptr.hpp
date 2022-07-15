#pragma once

#include <concepts>
#include <cstddef>

namespace ember::inline memory {
    namespace internal {
        struct default_deleter {
            template<typename type_t>
            void operator()(type_t *object);
        };
    }

    /**
     * @brief This class ensures that only 1 pointer can point to an object at any one time.
     * @details unique_ptr assumes that any object given to it has been allocated from the 
     * global memory pool. So when it goes out of scope or resets it will try to delete
     * the object from that pool. This can be circumnavigated by using custom deleters.
     * @tparam type_t 
     * @tparam deleter_t 
     */
    template<typename type_t, typename deleter_t = internal::default_deleter>
    class unique_ptr {
        template<typename other_type_t, typename deleter_t>
        friend class unique_ptr;

        //VARIABLES
    private:
        type_t *ptr{ nullptr };
        deleter_t deleter{};

        //FUNCTIONS
    public:
        unique_ptr();
        unique_ptr(std::nullptr_t);
        explicit unique_ptr(type_t *object);
        explicit unique_ptr(type_t *object, deleter_t deleter);

        unique_ptr(unique_ptr const &other) = delete;
        unique_ptr(unique_ptr &&other) noexcept;
        template<typename other_type_t>
        unique_ptr(unique_ptr<other_type_t, deleter_t> &&other) requires std::is_base_of_v<type_t, other_type_t>;

        unique_ptr &operator=(unique_ptr const &other) = delete;
        unique_ptr &operator=(unique_ptr &&other) noexcept;

        ~unique_ptr();

        bool is_valid() const;

        type_t *get() const;

        void reset();

        type_t *operator->();
        type_t const *operator->() const;

        auto &operator*() requires(!std::is_void_v<type_t>);
        auto const &operator*() const requires(!std::is_void_v<type_t>);

        template<typename type_t_1, typename deleter_t>
        friend bool operator==(unique_ptr<type_t_1, deleter_t> const &lhs, std::nullptr_t);
        template<typename type_t_1, typename deleter_t>
        friend bool operator!=(unique_ptr<type_t_1, deleter_t> const &lhs, std::nullptr_t);
    };
}

namespace ember::inline memory {
    /**
     * @brief Makes a unique_ptr to type_t, allocating it from the global memory pool.
     * @tparam type_t Type the unique_ptr will point to
     * @tparam arg_t 
     * @param args 
     * @return 
     */
    template<typename type_t, typename... args_t>
    unique_ptr<type_t> make_unique(args_t &&...args);
}

#include "unique_ptr.inl"