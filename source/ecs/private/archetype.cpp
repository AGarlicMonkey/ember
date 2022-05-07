#include "ember/ecs/archetype.hpp"

namespace ember::ecs {
    void archetype::add_entity(entity entity) {        
        component_data.resize(component_data.size() + stride);
        entities[entity] = (component_data.size() / stride) - 1;
    }
}