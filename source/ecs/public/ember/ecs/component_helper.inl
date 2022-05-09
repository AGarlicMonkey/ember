namespace ember::ecs::internal{
    template<typename component_t>
    void component_helpers_impl<component_t>::move(std::byte *source, std::byte *destination) const {
        auto *component_source{ reinterpret_cast<component_t *>(source) };
        new(destination) component_t{ std::move(*component_source) };
        component_source->~component_t();
    }

    template<typename component_t>
    void component_helpers_impl<component_t>::destruct(std::byte *memory) const {
        auto *component{ reinterpret_cast<component_t *>(memory) };
        component->~component_t();
    }

    template<typename component_t>
    std::size_t component_helpers_impl<component_t>::get_size() const {
        return sizeof(component_t);
    }

    template<typename component_t>
    component_id_t component_helpers_impl<component_t>::get_id() const {
        return id_generator::get<component_t>();
    }
}