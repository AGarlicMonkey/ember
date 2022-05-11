#include "ember/ecs/function_traits.hpp"

#include <ember/core/log.hpp>
#include <numeric>

namespace ember::ecs {
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

    template<typename function_t>
    void archetype::invoke_on_components(function_t function) {
        //Generate an invoke call where index sequence contains indices to each function params
        invoke(function, std::make_index_sequence<internal::function_traits<function_t>::arity>{});
    }

    template<typename function_t, typename object_t>
    void archetype::invoke_on_components(function_t function, object_t *object) {
        //Generate an invoke call where index sequence contains indices to each function params
        invoke_member(function, object, std::make_index_sequence<internal::function_traits<function_t>::arity>{});
    }

    template<typename component_t>
    std::size_t archetype::get_component_offset() const {
        return get_component_offset(id_generator::get<component_t>());
    }

    std::size_t archetype::get_component_offset(component_id_t const component_id) const {
        EMBER_CHECK(allows_component(component_id));
        return component_offsets.at(component_id);
    }

    template<typename component_t>
    std::byte *archetype::get_component_memory(entity const entity) const {
        return get_component_memory(entity, id_generator::get<component_t>());
    }

    template<typename component_t>
    component_t &archetype::get_component_within_block(std::byte *const block) {
        //TODO: This will likely need to be optimised in the future. get_component_offset loops through the archetype's id on every call.
        return *reinterpret_cast<component_t *>(block + get_component_offset<component_t>());
    }

    template<typename function_t, std::size_t... parameter_indices_t>
    void archetype::invoke(function_t function, std::index_sequence<parameter_indices_t...>) {
        //Generate an invoke call where the inidices are used to expand out each type of the function's params
        return invoke<function_t, std::tuple_element_t<parameter_indices_t, typename internal::function_traits<function_t>::decayed_parameter_types_tuple>...>(function);
    }

    template<typename function_t, typename... components_t>
    void archetype::invoke(function_t function) {
        for(std::size_t i{ 0 }; i < component_data.allocations; ++i) {
            std::byte *const archetype_block{ component_data.memory + (i * component_data.stride) };
            function(get_component_within_block<components_t>(archetype_block)...);
        }
    }

    template<typename function_t, typename object_t, std::size_t... parameter_indices_t>
    void archetype::invoke_member(function_t function, object_t *object, std::index_sequence<parameter_indices_t...>) {
        //Generate an invoke call where the inidices are used to expand out each type of the function's params
        return invoke_member<function_t, object_t, std::tuple_element_t<parameter_indices_t, typename internal::function_traits<function_t>::decayed_parameter_types_tuple>...>(function, object);
    }

    template<typename function_t, typename object_t, typename... components_t>
    void archetype::invoke_member(function_t function, object_t *object) {
        for(std::size_t i{ 0 }; i < component_data.allocations; ++i) {
            std::byte *const archetype_block{ component_data.memory + (i * component_data.stride) };
            (object->*function)(get_component_within_block<components_t>(archetype_block)...);
        }
    }
}