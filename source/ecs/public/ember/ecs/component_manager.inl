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
            if(std::find(old_archetype->id.begin(), old_archetype->id.end(), component_id) != old_archetype->id.end()) {// array::contains? Could just deref the iterator here
                //Already have it
            }

            //TODO...
        } else {
            archetype_id_t const archetype_id{ component_id };

            if(auto iter{ std::find_if(archetypes.begin(), archetypes.end(), [&archetype_id](archetype const &archetype) { return archetype.id == archetype_id; }) }; iter != archetypes.end()) {
                //Archetype exists add it to it.
                //TODO...
            } else {
                //Create new archetype
                archetype archetype{ archetype_id };
                archetype.entities.push_back(entity);
                archetype.component_data.resize(sizeof(component_t));
                archetype.component_data_size.push_back(sizeof(component_t));

                ret_comp = new(archetype.component_data.data()) component_t{ std::forward<construct_args_t>(construct_args)... };
                EMBER_CHECK(ret_comp != nullptr);

                archetypes.emplace_back(archetype_id);
                entity_to_archetype[entity] = archetypes.size() - 1;
            }
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
        return std::find(archetype.id.begin(), archetype.id.end(), component_id) != archetype.id.end();
    }
}