#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>

namespace ember::containers::internal {
    template<typename array_type>
    class const_array_iterator {
        //TYPES
    public:
        using iterator_concept  = std::contiguous_iterator_tag;
        using iterator_category = std::random_access_iterator_tag;

        using value_type     = typename array_type::value_type;
        using pointer_type   = typename array_type::const_pointer_type;
        using reference_type = typename array_type::const_reference_type;

        //VARIABLES
    private:
        array_type const *array{ nullptr };
        pointer_type ptr{ nullptr };

        //FUNCTIONS
    public:
        const_array_iterator();
        const_array_iterator(array_type const *array, typename array_type::pointer_type start);

        const_array_iterator(const_array_iterator const &other);
        const_array_iterator(const_array_iterator &&other) noexcept;

        const_array_iterator &operator=(const_array_iterator const &other);
        const_array_iterator &operator=(const_array_iterator &&other) noexcept;

        ~const_array_iterator();

        pointer_type operator->() const;
        reference_type operator*() const;

        const_array_iterator &operator++();
        const_array_iterator &operator--();

        template<typename array_type_1>
        friend const_array_iterator<array_type_1> operator+(const_array_iterator<array_type_1> const &lhs, std::size_t const num);
        template<typename array_type_1>
        friend const_array_iterator<array_type_1> operator-(const_array_iterator<array_type_1> const &lhs, std::size_t const num);

        template<typename array_type_1>
        friend bool operator==(const_array_iterator<array_type_1> const &lhs, const_array_iterator<array_type_1> const &rhs) noexcept;
        template<typename array_type_1>
        friend bool operator!=(const_array_iterator<array_type_1> const &lhs, const_array_iterator<array_type_1> const &rhs) noexcept;

        template<typename array_type_1>
        friend bool operator<(const_array_iterator<array_type_1> const &lhs, const_array_iterator<array_type_1> const &rhs) noexcept;
        template<typename array_type_1>
        friend bool operator<=(const_array_iterator<array_type_1> const &lhs, const_array_iterator<array_type_1> const &rhs) noexcept;

        template<typename array_type_1>
        friend bool operator>(const_array_iterator<array_type_1> const &lhs, const_array_iterator<array_type_1> const &rhs) noexcept;
        template<typename array_type_1>
        friend bool operator>=(const_array_iterator<array_type_1> const &lhs, const_array_iterator<array_type_1> const &rhs) noexcept;

    private:
        pointer_type first() const;
        pointer_type last() const;
    };

    template<typename array_type>
    class array_iterator : public const_array_iterator<array_type> {
        //TYPES
    public:
        using iterator_concept  = std::contiguous_iterator_tag;
        using iterator_category = std::random_access_iterator_tag;

        using value_type     = typename array_type::value_type;
        using pointer_type   = typename array_type::pointer_type;
        using reference_type = typename array_type::reference_type;

        //VARIABLES
    private:
        array_type *array{ nullptr };
        pointer_type ptr{ nullptr };

        //FUNCTIONS
    public:
        array_iterator();
        array_iterator(array_type *array, pointer_type start);

        array_iterator(array_iterator const &other);
        array_iterator(array_iterator &&other) noexcept;

        array_iterator &operator=(array_iterator const &other);
        array_iterator &operator=(array_iterator &&other) noexcept;

        ~array_iterator();

        pointer_type operator->();
        reference_type operator*();

        array_iterator &operator++();
        array_iterator &operator--();

        template<typename array_type_1>
        friend array_iterator<array_type_1> operator+(array_iterator<array_type_1> const &lhs, std::size_t const num);
        template<typename array_type_1>
        friend array_iterator<array_type_1> operator-(array_iterator<array_type_1> const &lhs, std::size_t const num);

        template<typename array_type_1>
        friend bool operator==(array_iterator<array_type_1> const &lhs, array_iterator<array_type_1> const &rhs) noexcept;
        template<typename array_type_1>
        friend bool operator!=(array_iterator<array_type_1> const &lhs, array_iterator<array_type_1> const &rhs) noexcept;

        template<typename array_type_1>
        friend bool operator<(array_iterator<array_type_1> const &lhs, array_iterator<array_type_1> const &rhs) noexcept;
        template<typename array_type_1>
        friend bool operator<=(array_iterator<array_type_1> const &lhs, array_iterator<array_type_1> const &rhs) noexcept;

        template<typename array_type_1>
        friend bool operator>(array_iterator<array_type_1> const &lhs, array_iterator<array_type_1> const &rhs) noexcept;
        template<typename array_type_1>
        friend bool operator>=(array_iterator<array_type_1> const &lhs, array_iterator<array_type_1> const &rhs) noexcept;

    private:
        pointer_type first() const;
        pointer_type last() const;
    };
}

namespace ember::containers {
    /**
     * @brief Dynamically sized contiguous array of type T.
     * @tparam T 
     */
    template<typename T>
    class array {
        //TYPES
    public:
        using value_type           = T;
        using pointer_type         = T *;
        using const_pointer_type   = T const *;
        using reference_type       = T &;
        using const_reference_type = T const &;

        using iterator       = internal::array_iterator<array<T>>;
        using const_iterator = internal::const_array_iterator<array<T>>;

        friend iterator;
        friend const_iterator;

    private:
        enum class reallocate_type {
            ignore_current_items,
            preserve_current_items,
        };

        //VARIABLES
    private:
        std::byte *memory{ nullptr };  /**< Underlying memory of the array. */
        pointer_type first{ nullptr }; /**< Points past the first item of the array. */

        std::size_t elems{ 0 }; /**< How many items are currently stored in this array. */
        std::size_t cap{ 0 };   /**< Total capacity of the array. */

        //FUNCTIONS
    public:
        array() noexcept;
        explicit array(std::size_t const size) requires std::is_default_constructible_v<T>;
        array(std::initializer_list<T> init);
        template<typename iterator_type>
        array(iterator_type begin, iterator_type end);

        array(array const &other);
        array(array &&other);

        array &operator=(array const &other);
        array &operator=(array &&other);

        ~array();

        /**
         * @brief Copies val onto the end of the array.
         * @param val 
         */
        void push_back(const_reference_type val);

        /**
         * @brief Constructs a new item at the end of the array with args.
         * @tparam args_t 
         * @param args 
         */
        template<typename... args_t>
        void emplace_back(args_t &&...args);

        /**
         * @brief Removes an element from the end of this array
         */
        void pop_back();

        /**
         * @brief Erases an element from the array at the specified point.
         * @param where Where to remove the element from.
         * @return Returns an iterator after the removed element.
         */
        iterator erase(iterator where);
        /**
         * @brief Erases elements in the range [from, to).
         * @param from Where to start erasing from (inclusive).
         * @param to Where to erasse to (exclusive).
         * @return Returns an iterator after the last removed element.
         */
        iterator erase(iterator from, iterator to);

        /**
         * @brief Erases a specific value from the array.
         * @param value 
         */
        void erase(T const &value);

        /**
         * @brief Increases the capacity of the array to meet new_capacity. If the 
         * current capacity is already larger than new_capatacity then this function
         * does nothing.
         * @param new_capacity 
         */
        void reserve(std::size_t new_capacity);

        /**
         * @brief Changes the number of elements stored to size.
         * @details If the current size of the array is greater than size then extra items will be destructed. If the
         * current size of the array is less than size then extra items will be constructed on the end.
         * @param size 
         * @param args List of arguments to pass to the constructor if required.
         */
        template<typename... args_t>
        void resize(std::size_t const size, args_t &&...args);

        /**
         * @brief Clears all of the elements in the array
         */
        void clear();

        /**
         * @brief Returns the number of elements.
         * @return 
         */
        std::size_t size() const noexcept;
        /**
         * @brief Returns the number of elements that can be held in currently allocated storage.
         * @return 
         */
        std::size_t capacity() const noexcept;

        /**
         * @brief Returns true if this array contains no elements.
         * @return 
         */
        [[nodiscard]] bool empty() const noexcept;

        /**
         * @brief Returns the item at the front of the array.
         * @return 
         */
        reference_type front();
        /**
         * @overload front() 
         */
        const_reference_type front() const;

        /**
         * @brief Returns the item at the end of the array.
         * @return 
         */
        reference_type back();
        /**
         * @overload back() 
         */
        const_reference_type back() const;

        /**
         * @brief Direct access to the underlying array.
         * @return 
         */
        pointer_type data();
        /**
         * @overload data() 
         */
        const_pointer_type data() const;

        /**
         * @brief Returns an iterator to the beginning of this array.
         * @return 
         */
        iterator begin() noexcept;
        /**
         * @overload begin() 
         */
        const_iterator begin() const noexcept;
        /**
         * @brief Returns an iterator to the end of this array.
         * @return 
         */
        iterator end() noexcept;
        /**
         * @overload end() 
         */
        const_iterator end() const noexcept;

        reference_type operator[](std::size_t pos);
        const_reference_type operator[](std::size_t pos) const;

    private:
        void allocate_array(std::size_t const capacity);
        void reallocate_array(std::size_t const new_capacity, reallocate_type const type);

        void destruct_items();

        void double_size();
    };
}

#include "array.inl"