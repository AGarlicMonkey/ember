#include <ember/core/log.hpp>
#include <numeric>

namespace ember::ecs {
    archetype::archetype(archetype_id_t id, containers::array<std::size_t> component_sizes)
        : id{ std::move(id) }
        , component_sizes{ std::move(component_sizes) } {
        stride = std::accumulate(this->component_sizes.begin(), this->component_sizes.end(), static_cast<std::size_t>(0));
        EMBER_CHECK(stride != 0);
    }

    archetype_id_t const &archetype::get_id() const {
        return id;
    }

    bool archetype::contains_entity(entity entity) const {
        return entities.contains(entity);
    }

    bool archetype::allows_component(component_id_t component_id) const {
        return std::find(id.begin(), id.end(), component_id) != id.end();
    }

    template<typename component_t, typename... construct_args_t>
    component_t &archetype::alloc_component(entity entity, construct_args_t &&...construct_args) {
        component_id_t const component_id{ id_generator::get<component_t>() };

        EMBER_CHECK(contains_entity(entity));
        EMBER_CHECK(allows_component(component_id));

        std::size_t const entity_index{ entities.at(entity) };
        std::byte *component_memory{ component_data.data() + (stride * entity_index) };

        //TODO: This does not handle multiple component archetypes.
        component_t *ret_comp{ new(component_memory) component_t{ std::forward<construct_args_t>(construct_args)... } };
        return *ret_comp;
    }

    template<typename component_t>
    component_t &archetype::get_component(entity entity) {
        component_id_t const component_id{ id_generator::get<component_t>() };

        EMBER_CHECK(contains_entity(entity));
        EMBER_CHECK(allows_component(component_id));

        std::size_t const entity_index{ entities.at(entity) };
        std::byte *component_memory{ component_data.data() + (stride * entity_index) };

        //TODO: This does not handle multiple component archetypes.
        return *reinterpret_cast<component_t *>(component_memory);
    }
}