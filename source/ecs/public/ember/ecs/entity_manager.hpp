#pragma once

#include "ember/ecs/types.hpp"
#include "ember/ecs/component_manager.hpp"

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
        component_manager components{};

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
        bool is_valid(entity const entity);

        /**
         * @brief Adds a component to a given entity.
         * @tparam component_t 
         * @tparam construct_args_t 
         * @param entity 
         * @param construct_args 
         * @return The newly created component.
         */
        template<typename component_t, typename... construct_args_t>
        component_t &add_component(entity const entity, construct_args_t &&...construct_args);

        /**
         * @brief Checks if an entity has a component.
         * @tparam component_t Type of component to check for.
         * @param entity 
         * @return True if the component exists for the entity.
         */
        template<typename component_t>
        bool has_component(entity const entity);

        /**
         * @brief Gets a specified from an entity. Will assert if that component does not exist.
         * @tparam component_t 
         * @param entity 
         * @return 
         */
        template<typename component_t>
        component_t &get_component(entity const entity);

        /**
         * @brief Removes a component from an entity.
         * @tparam component_t 
         * @param entity 
         */
        template<typename component_t>
        void remove_component(entity const entity);
    };
}

#include "entity_manager.inl"