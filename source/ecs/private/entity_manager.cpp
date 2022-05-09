#include "ember/ecs/entity_manager.hpp"

namespace ember::ecs {
    bool entity_manager::is_valid(entity const entity) {
        if(entity != null_entity) {
            return std::find(entities.begin(), entities.end(), entity) != entities.end();
        } else {
            return false;
        }
    }
}