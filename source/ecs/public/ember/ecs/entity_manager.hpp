#pragma once

#include "ember/ecs/entity.hpp"

#include <ember/containers/array.hpp>
#include <ember/core/export.hpp>

namespace ember::ecs {
    /**
     * @brief Manages entities and their components.
     */
    class EMBER_API entity_manager {
        //VARIABLES
    private:
        inline static entity next_entity{ 0 };

        containers::array<entity> entities{};

        //FUNCTIONS
    public:
        inline entity_manager();

        entity_manager(entity_manager const &other) = delete;
        inline entity_manager(entity_manager &&other) noexcept;

        entity_manager &operator=(entity_manager const &other) = delete;
        inline entity_manager &operator=(entity_manager &&other) noexcept;

        inline ~entity_manager();

        /**
         * @brief Creates a new entity.
         * @return 
         */
        inline entity create();
        /**
         * @brief Destroys an entity. Removing all of it's components from this manager.
         * @param entity 
         */
        inline void destroy(entity &entity);
        /**
         * @brief Destroys all entities contained within this manager.
         */
        inline void destroy_all();

        /**
         * @brief Checks if an entity is valid within the context of this manager. Entities created from
         * other managers will be invalid.
         * @param entity 
         * @return 
         */
        bool is_valid(entity entity);
    };
}

#include "entity_manager.inl"