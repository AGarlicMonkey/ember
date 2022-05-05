#pragma once

#include "ember/containers/array.hpp"

namespace ember::containers {
    template<typename T>
    concept integral_t = std::is_integral_v<T>;

    template<typename T>
    concept moveable_t = std::is_move_assignable_v<T>;//values in the set must at the very least be move assignable.

    /**
     * @brief A sparse set allows for O(1) insertion/deletion/look up while also mainting an
     * O(n) iteration time.
     * @tparam key_type Used to look up objects. Must be an integral type.
     * @tparam value_type 
     */
    template<integral_t key_t, moveable_t value_t>
    class sparse_set {
        //TYPES
    public:
        using value_type           = value_t;
        using pointer_type         = value_type *;
        using const_pointer_type   = value_type const *;
        using reference_type       = value_type &;
        using const_reference_type = value_type const &;

        using sparse_array_type = array<std::size_t>;
        using dense_array_type  = array<value_type>;//Store the key/value pair here so we can index back into the sparse array

        using iterator       = typename dense_array_type::iterator;
        using const_iterator = typename dense_array_type::const_iterator;

        friend iterator;
        friend const_iterator;

    private:
        using index_to_key_array_type = array<key_t>;

        //VARIABLES
    private:
        static std::size_t constexpr invalid_index{ ~0u };

        sparse_array_type key_to_index{};       /**< Sparse array that maps key_t into an index in item_array. */
        dense_array_type item_array{};          /**< Densely packed contiguous container of value_type. */
        index_to_key_array_type index_to_key{}; /**< A second sprase array that maps and index back into a key_t. */

        //FUNCTIONS
    public:
        sparse_set();

        sparse_set(sparse_set const &other);
        sparse_set(sparse_set &&other) noexcept;

        sparse_set &operator=(sparse_set const &other);
        sparse_set &operator=(sparse_set &&other) noexcept;

        ~sparse_set();

        /**
         * @brief Constructs a new item at key with args. If no item exists at key then a new
         * object is constructed in place else it is move assigned.
         * @tparam args_t 
         * @param key 
         * @param args 
         */
        template<typename... args_t>
        void emplace(key_t key, args_t &&...args);

        /**
         * @brief Erases the item at key.
         * @param key 
         */
        void erase(key_t key);

        /**
         * @brief Clears all of the elements in the set
         */
        void clear();

        /**
         * @brief Returns the number of elements.
         * @return 
         */
        std::size_t size() const noexcept;

        /**
         * @brief Checks to see if the set contains a specific key.
         * @param key 
         * @return 
         */
        bool contains(key_t key) const noexcept;

        /**
         * @brief Returns an iterator to the beginning of this set.
         * @return 
         */
        iterator begin() noexcept;
        /**
         * @overload begin() 
         */
        const_iterator begin() const noexcept;
        /**
         * @brief Returns an iterator to the end of this set.
         * @return 
         */
        iterator end() noexcept;
        /**
         * @overload end() 
         */
        const_iterator end() const noexcept;

        /**
         * @brief Index into the set with key. Default constructing a new object
         * if nothing exists at key.
         * @param key 
         * @return 
         */
        reference_type operator[](key_t key);
    };
}

#include "sparse_set.inl"