namespace ember::memory {
    std::size_t global_allocator::get_size() const {
        return size;
    }
}