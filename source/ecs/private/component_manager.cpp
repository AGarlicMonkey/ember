#include "ember/ecs/component_manager.hpp"

namespace ember::ecs {
    void component_manager::remove(entity const entity) {
        if(entity_to_archetype.contains(entity)) {
            archetypes[entity_to_archetype.at(entity)].remove_entity(entity);
            entity_to_archetype.erase(entity);
        }
    }

    containers::array<archetype>::iterator component_manager::find_archetype(archetype_id_t const &id) {
        return std::find_if(archetypes.begin(), archetypes.end(), [&id](archetype const &archetype) { return archetype.get_id() == id; });
    }

    containers::array<archetype>::iterator component_manager::find_or_add_archetype(archetype_id_t const &id) {
        if(auto archetype_iter{ find_archetype(id) }; archetype_iter != archetypes.end()) {
            return archetype_iter;
        } else {
            archetypes.emplace_back(id, &component_helper_map);
            return archetypes.end() - 1;
        }
    }
}