#include "ember/ecs/entity_manager.hpp"

namespace ember::ecs {
    void entity_manager::destroy(entity &entity) {
        EMBER_THROW_IF_FAILED(is_valid(entity), exception{ "Cannot destroy an invalid entity." });

        components.remove(entity);

        entities.erase(entity);
        entity = null_entity;
    }

    void entity_manager::destroy_all() {
        for(auto entity : entities) {
            components.remove(entity);
        }

        entities.clear();
    }
    
    bool entity_manager::is_valid(entity const entity) {
        if(entity != null_entity) {
            return std::find(entities.begin(), entities.end(), entity) != entities.end();
        } else {
            return false;
        }
    }
}