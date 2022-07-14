namespace ember::inline containers {
    template<integral_t key_t, moveable_t value_t>
    sparse_set<key_t, value_t>::sparse_set() = default;

    template<integral_t key_t, moveable_t value_t>
    sparse_set<key_t, value_t>::sparse_set(sparse_set const &other) = default;

    template<integral_t key_t, moveable_t value_t>
    sparse_set<key_t, value_t>::sparse_set(sparse_set &&other) noexcept = default;

    template<integral_t key_t, moveable_t value_t>
    sparse_set<key_t, value_t> &sparse_set<key_t, value_t>::operator=(sparse_set const &other) = default;

    template<integral_t key_t, moveable_t value_t>
    sparse_set<key_t, value_t> &sparse_set<key_t, value_t>::operator=(sparse_set &&other) noexcept = default;

    template<integral_t key_t, moveable_t value_t>
    sparse_set<key_t, value_t>::~sparse_set() = default;

    template<integral_t key_t, moveable_t value_t>
    template<typename... args_t>
    void sparse_set<key_t, value_t>::emplace(key_t key, args_t &&...args) {
        if(key_to_index.size() <= key) {
            key_to_index.resize(key + 1, invalid_index);
        }

        //We always erase here to avoid situations where an assignment operator might be deleted if we do
        //item_array[key_to_index[key]] = value_type{ std::forward<args_t>(args)... };
        //When a key already exists.
        erase(key);
        EMBER_CHECK(!contains(key));

        item_array.emplace_back(value_type{ std::forward<args_t>(args)... });
        index_to_key.push_back(key);
        key_to_index[key] = item_array.size() - 1;
    }

    template<integral_t key_t, moveable_t value_t>
    void sparse_set<key_t, value_t>::erase(key_t key) {
        if(contains(key)) {
            key_t const to_remove_index{ key_to_index[key] };//Index of the item we will remove.
            key_t const last_index{ item_array.size() - 1 }; //Index of the item we will replace it with.

            if(to_remove_index < last_index) {
                key_t const moved_key{ index_to_key[last_index] };

                key_to_index[moved_key]       = to_remove_index;
                item_array[to_remove_index]   = std::move(item_array.back());
                index_to_key[to_remove_index] = std::move(index_to_key.back());
            }

            key_to_index[key] = invalid_index;
            item_array.pop_back();
            index_to_key.pop_back();
        }
    }

    template<integral_t key_t, moveable_t value_t>
    void sparse_set<key_t, value_t>::clear() {
        key_to_index.clear();
        item_array.clear();
        index_to_key.clear();
    }

    template<integral_t key_t, moveable_t value_t>
    std::size_t sparse_set<key_t, value_t>::size() const noexcept {
        return item_array.size();
    }

    template<integral_t key_t, moveable_t value_t>
    bool sparse_set<key_t, value_t>::contains(key_t key) const noexcept {
        return key_to_index.size() > key && key_to_index[key] != invalid_index;
    }

    template<integral_t key_t, moveable_t value_t>
    typename sparse_set<key_t, value_t>::iterator sparse_set<key_t, value_t>::begin() noexcept {
        return item_array.begin();
    }

    template<integral_t key_t, moveable_t value_t>
    typename sparse_set<key_t, value_t>::const_iterator sparse_set<key_t, value_t>::begin() const noexcept {
        return item_array.begin();
    }

    template<integral_t key_t, moveable_t value_t>
    typename sparse_set<key_t, value_t>::iterator sparse_set<key_t, value_t>::end() noexcept {
        return item_array.end();
    }

    template<integral_t key_t, moveable_t value_t>
    typename sparse_set<key_t, value_t>::const_iterator sparse_set<key_t, value_t>::end() const noexcept {
        return item_array.end();
    }

    template<integral_t key_t, moveable_t value_t>
    typename sparse_set<key_t, value_t>::reference_type sparse_set<key_t, value_t>::operator[](key_t key) {
        if(!contains(key)) {
            emplace(key);
        }

        return item_array[key_to_index[key]];
    }
}