#include <ember/core/exception.hpp>
#include <ember/core/log.hpp>
#include <ember/memory/memory.hpp>

namespace ember::containers {
    namespace internal {
        template<typename array_type>
        const_array_iterator<array_type>::const_array_iterator() = default;

        template<typename array_type>
        const_array_iterator<array_type>::const_array_iterator(array_type const *array, typename array_type::pointer_type start)
            : array{ array }
            , ptr{ start } {
        }

        template<typename array_type>
        const_array_iterator<array_type>::const_array_iterator(const_array_iterator &&other) noexcept = default;
        template<typename array_type>

        const_array_iterator<array_type>::const_array_iterator(const_array_iterator const &other) = default;

        template<typename array_type>
        const_array_iterator<array_type> &const_array_iterator<array_type>::operator=(const_array_iterator &&other) noexcept = default;

        template<typename array_type>
        const_array_iterator<array_type> &const_array_iterator<array_type>::operator=(const_array_iterator const &other) = default;

        template<typename array_type>
        const_array_iterator<array_type>::~const_array_iterator() = default;

        template<typename array_type>
        const_array_iterator<array_type>::pointer_type const_array_iterator<array_type>::operator->() const {
            EMBER_CHECK(ptr < last());
            return ptr;
        }

        template<typename array_type>
        const_array_iterator<array_type>::reference_type const_array_iterator<array_type>::operator*() const {
            EMBER_CHECK(ptr < last());
            return *ptr;
        }

        template<typename array_type>
        const_array_iterator<array_type> &const_array_iterator<array_type>::operator++() {
            EMBER_CHECK(ptr < last());
            ++ptr;
            return *this;
        }

        template<typename array_type>
        const_array_iterator<array_type> &const_array_iterator<array_type>::operator--() {
            EMBER_CHECK(ptr > first());
            --ptr;
            return *this;
        }

        template<typename array_type_1>
        const_array_iterator<array_type_1> operator+(const_array_iterator<array_type_1> const &lhs, std::size_t const num) {
            EMBER_CHECK(lhs.ptr + num <= lhs.last());
            return const_array_iterator<array_type_1>{ lhs.array, lhs.ptr + num };
        }

        template<typename array_type_1>
        const_array_iterator<array_type_1> operator-(const_array_iterator<array_type_1> const &lhs, std::size_t const num) {
            EMBER_CHECK(lhs.ptr - num >= lhs.first());
            return const_array_iterator<array_type_1>{ lhs.array, lhs.ptr - num };
        }

        template<typename array_type_1>
        bool operator==(const_array_iterator<array_type_1> const &lhs, const_array_iterator<array_type_1> const &rhs) noexcept {
            return lhs.ptr == rhs.ptr;
        }

        template<typename array_type_1>
        bool operator!=(const_array_iterator<array_type_1> const &lhs, const_array_iterator<array_type_1> const &rhs) noexcept {
            return !(lhs == rhs);
        }

        template<typename array_type_1>
        bool operator<(const_array_iterator<array_type_1> const &lhs, const_array_iterator<array_type_1> const &rhs) noexcept {
            return lhs.ptr < rhs.ptr;
        }

        template<typename array_type_1>
        bool operator<=(const_array_iterator<array_type_1> const &lhs, const_array_iterator<array_type_1> const &rhs) noexcept {
            return lhs.ptr <= rhs.ptr;
        }

        template<typename array_type_1>
        bool operator>(const_array_iterator<array_type_1> const &lhs, const_array_iterator<array_type_1> const &rhs) noexcept {
            return lhs.ptr > rhs.ptr;
        }

        template<typename array_type_1>
        bool operator>=(const_array_iterator<array_type_1> const &lhs, const_array_iterator<array_type_1> const &rhs) noexcept {
            return lhs.ptr >= rhs.ptr;
        }

        template<typename array_type>
        typename const_array_iterator<array_type>::pointer_type const_array_iterator<array_type>::first() const {
            return this->array->first;
        }

        template<typename array_type>
        typename const_array_iterator<array_type>::pointer_type const_array_iterator<array_type>::last() const {
            return this->array->first + this->array->elems;
        }

        template<typename array_type>
        array_iterator<array_type>::array_iterator() = default;

        template<typename array_type>
        array_iterator<array_type>::array_iterator(array_type *array, pointer_type start)
            : array{ array }
            , ptr{ start } {
        }

        template<typename array_type>
        array_iterator<array_type>::array_iterator(array_iterator &&other) noexcept = default;

        template<typename array_type>
        array_iterator<array_type>::array_iterator(array_iterator const &other) = default;

        template<typename array_type>
        array_iterator<array_type> &array_iterator<array_type>::operator=(array_iterator &&other) noexcept = default;

        template<typename array_type>
        array_iterator<array_type> &array_iterator<array_type>::operator=(array_iterator const &other) = default;

        template<typename array_type>
        array_iterator<array_type>::~array_iterator() = default;

        template<typename array_type>
        array_iterator<array_type>::pointer_type array_iterator<array_type>::operator->() {
            EMBER_CHECK(ptr < last());
            return ptr;
        }

        template<typename array_type>
        array_iterator<array_type>::reference_type array_iterator<array_type>::operator*() {
            EMBER_CHECK(ptr < last());
            return *ptr;
        }

        template<typename array_type>
        array_iterator<array_type> &array_iterator<array_type>::operator++() {
            EMBER_CHECK(ptr < last());
            ++ptr;
            return *this;
        }

        template<typename array_type>
        array_iterator<array_type> &array_iterator<array_type>::operator--() {
            EMBER_CHECK(ptr > first());
            --ptr;
            return *this;
        }

        template<typename array_type_1>
        array_iterator<array_type_1> operator+(array_iterator<array_type_1> const &lhs, std::size_t const num) {
            EMBER_CHECK(lhs.ptr + num <= lhs.last());
            return array_iterator<array_type_1>{ lhs.array, lhs.ptr + num };
        }

        template<typename array_type_1>
        array_iterator<array_type_1> operator-(array_iterator<array_type_1> const &lhs, std::size_t const num) {
            EMBER_CHECK(lhs.ptr - num >= lhs.first());
            return array_iterator<array_type_1>{ lhs.array, lhs.ptr - num };
        }

        template<typename array_type_1>
        bool operator==(array_iterator<array_type_1> const &lhs, array_iterator<array_type_1> const &rhs) noexcept {
            return lhs.ptr == rhs.ptr;
        }

        template<typename array_type_1>
        bool operator!=(array_iterator<array_type_1> const &lhs, array_iterator<array_type_1> const &rhs) noexcept {
            return !(lhs == rhs);
        }

        template<typename array_type_1>
        bool operator<(array_iterator<array_type_1> const &lhs, array_iterator<array_type_1> const &rhs) noexcept {
            return lhs.ptr < rhs.ptr;
        }

        template<typename array_type_1>
        bool operator<=(array_iterator<array_type_1> const &lhs, array_iterator<array_type_1> const &rhs) noexcept {
            return lhs.ptr <= rhs.ptr;
        }

        template<typename array_type_1>
        bool operator>(array_iterator<array_type_1> const &lhs, array_iterator<array_type_1> const &rhs) noexcept {
            return lhs.ptr > rhs.ptr;
        }

        template<typename array_type_1>
        bool operator>=(array_iterator<array_type_1> const &lhs, array_iterator<array_type_1> const &rhs) noexcept {
            return lhs.ptr >= rhs.ptr;
        }

        template<typename array_type>
        typename array_iterator<array_type>::pointer_type array_iterator<array_type>::first() const {
            return this->array->first;
        }

        template<typename array_type>
        typename array_iterator<array_type>::pointer_type array_iterator<array_type>::last() const {
            return this->array->first + this->array->elems;
        }
    }

    template<typename T>
    array<T>::array() noexcept = default;

    template<typename T>
    array<T>::array(std::size_t const size) requires std::is_default_constructible_v<T>
        : elems{ size }, cap{ size } {
        allocate_array(cap);
        for(std::size_t i{ 0 }; i < elems; ++i) {
            new(&first[i]) value_type{};
        }
    }

    template<typename T>
    array<T>::array(std::initializer_list<T> init)
        : elems{ init.size() }
        , cap{ init.size() } {
        allocate_array(cap);
        std::size_t index{ 0 };
        for(auto iter{ init.begin() }; iter != init.end(); ++iter, ++index) {
            new(&first[index]) value_type{ *iter };
        }
    }

    template<typename T>
    template<typename iterator_type>
    array<T>::array(iterator_type begin, iterator_type end)
        : elems{ static_cast<std::size_t>(std::distance(begin, end)) }
        , cap{ elems } {
        allocate_array(cap);
        std::size_t index{ 0 };
        for(auto iter{ begin }; iter != end; ++iter, ++index) {
            new(&first[index]) value_type{ *iter };
        }
    }

    template<typename T>
    array<T>::array(array<T> const &other) {
        destruct_items();

        elems = other.elems;
        cap   = other.cap;
        reallocate_array(cap, reallocate_type::ignore_current_items);

        for(std::size_t i{ 0 }; i < elems; ++i) {
            new(&first[i]) value_type{ other[i] };
        }
    }

    template<typename T>
    array<T>::array(array<T> &&other) {
        destruct_items();

        elems = other.elems;
        cap   = other.cap;
        reallocate_array(cap, reallocate_type::ignore_current_items);

        for(std::size_t i{ 0 }; i < elems; ++i) {
            new(&first[i]) value_type{ std::move(other[i]) };
        }

        memory::free(other.memory);
        other.memory = nullptr;
        other.first  = nullptr;
    }

    template<typename T>
    array<T> &array<T>::operator=(array<T> const &other) {
        destruct_items();

        elems = other.elems;
        cap   = other.cap;
        reallocate_array(cap, reallocate_type::ignore_current_items);

        for(std::size_t i{ 0 }; i < elems; ++i) {
            new(&first[i]) value_type{ other[i] };
        }

        return *this;
    }

    template<typename T>
    array<T> &array<T>::operator=(array<T> &&other) {
        destruct_items();

        elems = other.elems;
        cap   = other.cap;
        reallocate_array(cap, reallocate_type::ignore_current_items);

        for(std::size_t i{ 0 }; i < elems; ++i) {
            new(&first[i]) value_type{ std::move(other[i]) };
        }

        memory::free(other.memory);
        other.memory = nullptr;
        other.first  = nullptr;

        return *this;
    }

    template<typename T>
    array<T>::~array() {
        if(memory != nullptr) {
            destruct_items();
            memory::free(memory);
        }
    }

    template<typename T>
    void array<T>::push_back(const_reference_type val) {
        if(elems + 1 > cap) {
            double_size();
        }
        new(&first[elems++]) value_type{ val };
    }

    template<typename T>
    template<typename... args_t>
    void array<T>::emplace_back(args_t &&...args) {
        if(elems + 1 > cap) {
            double_size();
        }
        new(&first[elems++]) value_type{ std::forward<args_t>(args)... };
    }

    template<typename T>
    void array<T>::pop_back() {
        if(elems > 0) {
            first[elems - 1].~T();
            --elems;
        }
    }

    template<typename T>
    array<T>::iterator array<T>::erase(iterator where) {
        return erase(where, where + 1);
    }

    template<typename T>
    array<T>::iterator array<T>::erase(iterator from, iterator to) {
        EMBER_CHECK(to <= end());

        if(from == to) {
            return to;
        }

        if(to == end()) {
            std::size_t count{ 0 };

            //If we're erasing until the end then we can just destruct the last elements
            for(; from != end(); ++from, ++count) {
                (*from).~T();
            }

            elems -= count;

            return end();
        } else {
            std::size_t count{ 0 };

            //Destruct the items we are going to remove
            for(auto iter{ from }; iter != to; ++iter, ++count) {
                (*from).~T();
            }

            //Shift the entire array over the removed elemends
            for(auto iter{ to }, move_to{ from }; iter != end(); ++iter, ++move_to) {
                *move_to = std::move(*iter);
                (*iter).~T();
            }

            elems -= count;

            return from;
        }
    }

    template<typename T>
    void array<T>::reserve(std::size_t new_capacity) {
        if(cap < new_capacity) {
            cap = new_capacity;
            reallocate_array(cap, reallocate_type::preserve_current_items);
        }
    }

    template<typename T>
    template<typename... args_t>
    void array<T>::resize(std::size_t const size, args_t &&...args) {
        if(cap < size) {
            cap = size;
            reallocate_array(cap, reallocate_type::preserve_current_items);
        }

        if(elems < size) {
            //Grow the array
            for(std::size_t i{ elems }; i < size; ++i) {
                new(&first[i]) T{ std::forward<args_t>(args)... };
            }
        } else {
            //Shrink the array
            for(std::size_t i{ size }; i < elems; ++i) {
                first[i].~T();
            }
        }
        elems = size;
    }

    template<typename T>
    void array<T>::clear() {
        for(std::size_t i{ 0 }; i < elems; ++i) {
            first[i].~T();
        }
        elems = 0;
    }

    template<typename T>
    std::size_t array<T>::size() const noexcept {
        return elems;
    }

    template<typename T>
    std::size_t array<T>::capacity() const noexcept {
        return cap;
    }

    template<typename T>
    array<T>::reference_type array<T>::front() {
        EMBER_CHECK(elems > 0);
        return *first;
    }

    template<typename T>
    array<T>::const_reference_type array<T>::front() const {
        EMBER_CHECK(elems > 0);
        return *first;
    }

    template<typename T>
    array<T>::reference_type array<T>::back() {
        EMBER_CHECK(elems > 0);
        return *(first + (elems - 1));
    }

    template<typename T>
    array<T>::const_reference_type array<T>::back() const {
        EMBER_CHECK(elems > 0);
        return *(first + (elems - 1));
    }

    template<typename T>
    array<T>::pointer_type array<T>::data() {
        return first;
    }

    template<typename T>
    array<T>::const_pointer_type array<T>::data() const {
        return first;
    }

    template<typename T>
    array<T>::iterator array<T>::begin() noexcept {
        return iterator{ this, first };
    }

    template<typename T>
    array<T>::const_iterator array<T>::begin() const noexcept {
        return const_iterator{ this, first };
    }

    template<typename T>
    array<T>::iterator array<T>::end() noexcept {
        return iterator{ this, first + elems };
    }

    template<typename T>
    array<T>::const_iterator array<T>::end() const noexcept {
        return const_iterator{ this, first + elems };
    }

    template<typename T>
    array<T>::reference_type array<T>::operator[](std::size_t pos) {
        EMBER_CHECK(pos < elems);
        return first[pos];
    }

    template<typename T>
    array<T>::const_reference_type array<T>::operator[](std::size_t pos) const {
        EMBER_CHECK(pos < elems);
        return first[pos];
    }

    template<typename T>
    void array<T>::allocate_array(std::size_t const capacity) {
        EMBER_CHECK(memory == nullptr);

        memory = memory::alloc(sizeof(value_type) * capacity, alignof(value_type));
        EMBER_THROW_IF_FAILED(memory != nullptr, exception{ "Failed to allocate array." });
        first = reinterpret_cast<pointer_type>(memory);
    }

    template<typename T>
    void array<T>::reallocate_array(std::size_t const new_capacity, reallocate_type const type) {
        if(memory != nullptr) {
            if(type == reallocate_type::preserve_current_items) {
                std::byte *new_memory = memory::alloc(sizeof(value_type) * new_capacity, alignof(value_type));
                EMBER_THROW_IF_FAILED(memory != nullptr, exception{ "Failed to reallocate array." });
                auto *new_first{ reinterpret_cast<pointer_type>(new_memory) };

                if constexpr(std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                    for(std::size_t i{ 0 }; i < elems; ++i) {
                        new(&new_first[i]) value_type{ std::move(first[i]) };
                    }
                } else {
                    for(std::size_t i{ 0 }; i < elems; ++i) {
                        new(&new_first[i]) value_type{ first[i] };
                    }
                }

                memory::free(memory);
                memory = new_memory;
                first  = new_first;
            } else {
                memory = memory::realloc(memory, sizeof(value_type) * new_capacity, alignof(value_type));
                EMBER_THROW_IF_FAILED(memory != nullptr, exception{ "Failed to reallocate array." });
                first = reinterpret_cast<pointer_type>(memory);
            }
        } else {
            allocate_array(new_capacity);
        }
    }

    template<typename T>
    void array<T>::destruct_items() {
        for(std::size_t i{ 0 }; i < elems; ++i) {
            first[i].~T();
        }
    }

    template<typename T>
    void array<T>::double_size() {
        if(cap == 0) {
            cap = 1;
        } else {
            cap *= 2;
        }
        reallocate_array(cap, reallocate_type::preserve_current_items);
    }
}