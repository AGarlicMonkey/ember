#include "ember/ecs/function_traits.hpp"

#include <algorithm>

namespace ember::ecs {
    component_manager::component_manager() = default;

    component_manager::component_manager(component_manager &&other) noexcept = default;

    component_manager &component_manager::operator=(component_manager &&other) noexcept = default;

    component_manager::~component_manager() {
        archetypes.clear();
    }

    template<typename component_t, typename... construct_args_t>
    component_t &component_manager::add(entity const entity, construct_args_t &&...construct_args) {
        component_id_t const component_id{ id_generator::get<component_t>() };

        if(!component_helper_map.contains(component_id)) {
            component_helper_map[component_id] = memory::make_unique<internal::component_helpers_impl<component_t>>();
        }

        archetype *new_archetype{ nullptr };
        component_t *ret_comp{ nullptr };

        if(entity_to_archetype.contains(entity)) {
            archetype *old_archetype{ &archetypes[entity_to_archetype.at(entity)] };

            if(old_archetype->allows_component(component_id)) {
                //Destruct the existing component as we just re alloc it below.
                new_archetype = old_archetype;
                new_archetype->destruct_component<component_t>(entity);
            } else {
                archetype_id_t new_archetype_id{ old_archetype->get_id() };
                new_archetype_id.push_back(component_id);
                std::sort(new_archetype_id.begin(), new_archetype_id.end());

                auto archetype_iter{ find_or_add_archetype(new_archetype_id) };
                old_archetype = &archetypes[entity_to_archetype.at(entity)];//Need to get the archetype again as the array could've resized.
                archetype_iter->transfer_entity(entity, *old_archetype);

                entity_to_archetype[entity] = std::distance(archetypes.begin(), archetype_iter);

                new_archetype = archetype_iter.get();
            }
        } else {
            archetype_id_t const archetype_id{ component_id };

            auto archetype_iter{ find_or_add_archetype(archetype_id) };
            archetype_iter->add_entity(entity);
            entity_to_archetype[entity] = std::distance(archetypes.begin(), archetype_iter);

            new_archetype = archetype_iter.get();
        }

        EMBER_CHECK(new_archetype != nullptr);

        ret_comp = &new_archetype->alloc_component<component_t>(entity, std::forward<construct_args_t>(construct_args)...);

        EMBER_CHECK(ret_comp != nullptr);
        return *ret_comp;
    }

    template<typename component_t>
    bool component_manager::has(entity const entity) {
        component_id_t const component_id{ id_generator::get<component_t>() };

        if(!entity_to_archetype.contains(entity)) {
            return false;
        }

        archetype const &archetype{ archetypes[entity_to_archetype.at(entity)] };
        return archetype.allows_component(component_id);
    }

    template<typename component_t>
    component_t &component_manager::get(entity const entity) {
        EMBER_CHECK(has<component_t>(entity));
        return archetypes[entity_to_archetype.at(entity)].get_component<component_t>(entity);
    }

    template<typename component_t>
    void component_manager::remove(entity const entity) {
        if(has<component_t>(entity)) {
            component_id_t const component_id{ id_generator::get<component_t>() };

            archetype *old_archetype{ &archetypes[entity_to_archetype.at(entity)] };

            archetype_id_t new_archetype_id{ old_archetype->get_id() };
            new_archetype_id.erase(component_id);
            std::sort(new_archetype_id.begin(), new_archetype_id.end());//Sorted IDs stop issues with similar archetypes but different orders

            //If this was the entity's last component then we do not need to add it in another archetype.
            if(new_archetype_id.empty()) {
                old_archetype->remove_entity(entity);
                entity_to_archetype.erase(entity);
                return;
            }

            auto new_archetype{ find_or_add_archetype(new_archetype_id) };
            old_archetype = &archetypes[entity_to_archetype.at(entity)];//Need to get the pointer again incase the array resized.

            new_archetype->transfer_entity(entity, *old_archetype);
            entity_to_archetype.at(entity) = std::distance(archetypes.begin(), new_archetype);
        }
    }

    template<typename function_t>
    void component_manager::for_each(function_t function) {
        archetype_id_t const match_id{ generate_archetype_id_from_function<function_t>(std::make_index_sequence<internal::function_traits<function_t>::arity>{}) };

        for(auto &archetype : archetypes) {
            auto const &archetype_id{ archetype.get_id() };
            bool const is_match{ std::includes(archetype_id.begin(), archetype_id.end(), match_id.begin(), match_id.end()) };

            if(is_match) {
                archetype.invoke_on_components(function);
            }
        }
    }

    template<typename function_t, typename object_t>
    void component_manager::for_each(function_t function, object_t *object) {
        archetype_id_t const match_id{ generate_archetype_id_from_function<function_t>(std::make_index_sequence<internal::function_traits<function_t>::arity>{}) };

        for(auto &archetype : archetypes) {
            auto const &archetype_id{ archetype.get_id() };
            bool const is_match{ std::includes(archetype_id.begin(), archetype_id.end(), match_id.begin(), match_id.end()) };

            if(is_match) {
                archetype.invoke_on_components(function, object);
            }
        }
    }

    template<typename function_t, std::size_t... parameter_indices_t>
    archetype_id_t component_manager::generate_archetype_id_from_function(std::index_sequence<parameter_indices_t...>) {
        //Use the provided index sequence to expand out each type of the function's params
        return generate_archetype_id_from_types<std::tuple_element_t<parameter_indices_t, typename internal::function_traits<function_t>::decayed_parameter_types_tuple>...>();
    }

    template<typename component_t, typename... components_t>
    archetype_id_t component_manager::generate_archetype_id_from_types() {
        archetype_id_t id;

        //If the first 'component' is actually an entity then don't use it to generate the ID
        if constexpr(std::is_same_v<component_t, entity>) {
            id = { id_generator::get<components_t>()... };
        } else {
            id = { id_generator::get<component_t>(), id_generator::get<components_t>()... };
        }

        std::sort(id.begin(), id.end());
        return id;
    }
}