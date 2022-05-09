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
            component_helper_map[component_id] = memory::make_unique<archetype::component_helpers_impl<component_t>>();
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
                archetype_id_t archetype_id{ old_archetype->get_id() };
                archetype_id.push_back(component_id);
                std::sort(archetype_id.begin(), archetype_id.end());

                auto archetype_iter{ find_or_add_archetype(archetype_id) };
                old_archetype = &archetypes[entity_to_archetype.at(entity)];//NOTE: Need to get the archetype again as the array could've resized.
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

            archetype &old_archetype{ archetypes[entity_to_archetype.at(entity)] };

            archetype_id_t archetype_id{ old_archetype.get_id() };
            archetype_id.erase(component_id);
            std::sort(archetype_id.begin(), archetype_id.end());//Sorted IDs stop issues with similar archetypes but different orders

            //If this was the entity's last component then we can just return here
            if(archetype_id.empty()) {
                old_archetype.remove_entity(entity);
                entity_to_archetype.erase(entity);
                return;
            }

            auto new_archetype{ find_archetype(archetype_id) };
            new_archetype->transfer_entity(entity, old_archetype);
            entity_to_archetype.at(entity) = std::distance(archetypes.begin(), new_archetype);
        }
    }
}