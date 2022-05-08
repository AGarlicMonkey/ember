namespace ember::ecs {
    component_manager::component_manager() = default;

    component_manager::component_manager(component_manager &&other) noexcept = default;

    component_manager &component_manager::operator=(component_manager &&other) noexcept = default;

    component_manager::~component_manager() {
        archetypes.clear();
    }

    template<typename component_t, typename... construct_args_t>
    component_t &component_manager::add_component(entity entity, construct_args_t &&...construct_args) {
        component_id_t const component_id{ id_generator::get<component_t>() };

        if(!component_helper_map.contains(component_id)) {
            component_helper_map[component_id] = memory::make_unique<archetype::component_helpers_impl<component_t>>();
        }

        archetype *old_archetype{ nullptr };
        archetype *new_archetype{ nullptr };

        component_t *ret_comp{ nullptr };

        if(entity_to_archetype.contains(entity)) {
            old_archetype = &archetypes[entity_to_archetype.at(entity)];
        }

        if(old_archetype != nullptr) {
            //NOTE: Make sure to sort the IDs when creating new archetypes

            if(old_archetype->allows_component(component_id)) {
                //Already have it - just return the component
                //TODO...
            }

            //TODO...
        } else {
            archetype_id_t const archetype_id{ component_id };
            archetype *found_archetype{ nullptr };

            if(auto iter{ find_archetype(archetype_id) }; iter != archetypes.end()) {
                entity_to_archetype[entity] = std::distance(archetypes.begin(), iter);

                found_archetype = iter.get();
            } else {
                archetypes.emplace_back(std::move(archetype_id), &component_helper_map);
                entity_to_archetype[entity] = archetypes.size() - 1;

                found_archetype = &archetypes.back();
            }

            EMBER_CHECK(found_archetype != nullptr);

            found_archetype->add_entity(entity);
            ret_comp = &found_archetype->alloc_component<component_t>(entity, std::forward<construct_args_t>(construct_args)...);
        }

        EMBER_CHECK(ret_comp != nullptr);

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

    template<typename component_t>
    void component_manager::remove_component(entity entity) {
        if(has_component<component_t>(entity)) {
            component_id_t const component_id{ id_generator::get<component_t>() };

            archetype &old_archetype{ archetypes[entity_to_archetype.at(entity)] };

            archetype_id_t archetype_id{ old_archetype.get_id() };
            archetype_id.erase(component_id);

            //If this was the entity's last component then we can just return here
            if(archetype_id.empty()) {
                old_archetype.remove_entity(entity);
                entity_to_archetype.erase(entity);
                return;
            }

            //Find or add the new archetype created from removing the component
            if(auto iter{ find_archetype(archetype_id) }; iter != archetypes.end()) {
                iter->transfer_entity(entity, old_archetype);
                entity_to_archetype.at(entity) = std::distance(archetypes.begin(), iter);
            } else {
                archetype new_archetype{ archetype_id, &component_helper_map };
                new_archetype.transfer_entity(entity, old_archetype);

                archetypes.emplace_back(std::move(new_archetype));
                entity_to_archetype.at(entity) = archetypes.size() - 1;
            }
        }
    }
}