#include <ember/core/log.hpp>
#include <numeric>

namespace ember::ecs {
    template<typename component_t>
    void archetype::component_helpers_impl<component_t>::move(std::byte *source, std::byte *destination) const {
        auto *component_source{ reinterpret_cast<component_t *>(source) };
        new(destination) component_t{ std::move(*component_source) };
        component_source->~component_t();
    }

    template<typename component_t>
    void archetype::component_helpers_impl<component_t>::destruct(std::byte *memory) const {
        auto *component{ reinterpret_cast<component_t *>(memory) };
        component->~component_t();
    }

    template<typename component_t>
    std::size_t archetype::component_helpers_impl<component_t>::get_size() const {
        return sizeof(component_t);
    }

    template<typename component_t>
    component_id_t archetype::component_helpers_impl<component_t>::get_id() const {
        return id_generator::get<component_t>();
    }

    archetype::archetype(archetype_id_t id, containers::map<component_id_t, memory::unique_ptr<component_helpers>> *component_helper_map)
        : id{ std::move(id) }
        , component_helper_map{ component_helper_map } {
        component_data.stride = 0;
        for(auto component_id : this->id) {
            component_data.stride += component_helper_map->at(component_id)->get_size();
        }
        EMBER_CHECK(component_data.stride != 0);
    }

    archetype_id_t const &archetype::get_id() const {
        return id;
    }

    bool archetype::contains_entity(entity const entity) const {
        return entities.contains(entity);
    }

    bool archetype::allows_component(component_id_t const component_id) const {
        return std::find(id.begin(), id.end(), component_id) != id.end();
    }

    template<typename component_t, typename... construct_args_t>
    component_t &archetype::alloc_component(entity const entity, construct_args_t &&...construct_args) {
        EMBER_CHECK(contains_entity(entity));
        EMBER_CHECK(allows_component(id_generator::get<component_t>()));

        std::byte *component_memory{ get_component_memory<component_t>(entity) };
        component_t *ret_comp{ new(component_memory) component_t{ std::forward<construct_args_t>(construct_args)... } };
        return *ret_comp;
    }

    template<typename component_t>
    void archetype::destruct_component(entity const entity) {
        component_id_t const component_id{ id_generator::get<component_t>() };

        EMBER_CHECK(contains_entity(entity));
        EMBER_CHECK(allows_component(component_id));

        std::byte *component_memory{ get_component_memory(entity, component_id) };
        component_helper_map->at(component_id)->destruct(component_memory);
    }

    template<typename component_t>
    component_t &archetype::get_component(entity const entity) {
        EMBER_CHECK(contains_entity(entity));
        EMBER_CHECK(allows_component(id_generator::get<component_t>()));

        std::byte *component_memory{ get_component_memory<component_t>(entity) };
        return *reinterpret_cast<component_t *>(component_memory);
    }

    template<typename component_t>
    std::size_t archetype::get_component_offset() const {
        return get_component_offset(id_generator::get<component_t>());
    }

    template<typename component_t>
    std::byte *archetype::get_component_memory(entity const entity) const {
        return get_component_memory(entity, id_generator::get<component_t>());
    }
}