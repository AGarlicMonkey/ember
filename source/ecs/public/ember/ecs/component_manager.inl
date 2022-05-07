namespace ember::ecs {
    template<typename component_t, typename... construct_args_t>
    component_t &component_manager::add_component(entity entity, construct_args_t &&...construct_args) {
        component_id_t const component_id{ id_generator::get<component_t>() };

        archetype *old_archetype{ nullptr };
        archetype *new_archetype{ nullptr };

        component_t *ret_comp{ nullptr };

        if(entity_to_archetype.contains(entity)) {
            old_archetype = &archetypes[entity_to_archetype.at(entity)];
        }

        //If we cannot find an old archetype then it is our first time seeing this entity
        if(old_archetype != nullptr) {
            if(old_archetype->allows_component(component_id)) {
                //Already have it
                //TODO...
            }

            //TODO...
        } else {
            archetype_id_t const archetype_id{ component_id };
            archetype *found_archetype{ nullptr };

            if(auto iter{ std::find_if(archetypes.begin(), archetypes.end(), [&archetype_id](archetype const &archetype) { return archetype.get_id() == archetype_id; }) }; iter != archetypes.end()) {
                entity_to_archetype[entity] = std::distance(archetypes.begin(), iter);

                found_archetype = iter.get();
            } else {
                containers::array<std::size_t> component_sizes{ sizeof(component_id_t) };

                archetypes.emplace_back(std::move(archetype_id), std::move(component_sizes));
                entity_to_archetype[entity] = archetypes.size() - 1;

                found_archetype = &archetypes.back();
            }

            EMBER_CHECK(found_archetype != nullptr);

            found_archetype->add_entity(entity);
            ret_comp = &found_archetype->alloc_component<component_t>(entity, std::forward<construct_args_t>(construct_args)...);
            
            EMBER_CHECK(ret_comp != nullptr);
        }

        return *ret_comp;
    }

    template<typename component_t>
    bool component_manager::has_component(entity entity) {
        component_id_t const component_id{ id_generator::get<component_t>() };

        if(!entity_to_archetype.contains(entity)) {
            return false;
        }

        archetype const &archetype{ archetypes[entity_to_archetype.at(entity)] };
        return archetype.allows_component(component_id);
    }

    template<typename component_t>
    component_t &component_manager::get_component(entity entity) {
        EMBER_CHECK(has_component<component_t>(entity));
        return archetypes[entity_to_archetype.at(entity)].get_component<component_t>(entity);
    }
}