#pragma once

#include "ember/ecs/archetype.hpp"
#include "ember/ecs/types.hpp"

#include <ember/containers/array.hpp>
#include <ember/containers/map.hpp>
#include <ember/core/export.hpp>

namespace ember::ecs {
    /**
     * @brief Manages components accross all currently known entity archetypes.
     */
    class EMBER_API component_manager {
        //VARIABLES
    private:
        containers::array<archetype> archetypes{};
        containers::map<entity, std::size_t> entity_to_archetype{};//Maps an entity to an index to the archetypes array;
        containers::map<component_id_t, memory::unique_ptr<internal::component_helpers>> component_helper_map{};

        //FUNCTIONS
    public:
        inline component_manager();

        component_manager(component_manager const &other) = delete;
        inline component_manager(component_manager &&other) noexcept;

        component_manager &operator=(component_manager const &other) = delete;
        inline component_manager &operator=(component_manager &&other) noexcept;

        inline ~component_manager();

        template<typename component_t, typename... construct_args_t>
        component_t &add(entity const entity, construct_args_t &&...construct_args);

        template<typename component_t>
        bool has(entity const entity);

        template<typename component_t>
        component_t &get(entity const entity);

        template<typename component_t>
        void remove(entity const entity);

    private:
        containers::array<archetype>::iterator find_archetype(archetype_id_t const &id);
        containers::array<archetype>::iterator find_or_add_archetype(archetype_id_t const &id);
    };
}

#include "component_manager.inl"