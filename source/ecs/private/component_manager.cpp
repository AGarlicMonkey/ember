#include "ember/ecs/component_manager.hpp"

namespace ember::ecs {
    containers::array<archetype>::iterator component_manager::find_archetype(archetype_id_t const &id) {
        return std::find_if(archetypes.begin(), archetypes.end(), [&id](archetype const &archetype) { return archetype.get_id() == id; });
    }
}