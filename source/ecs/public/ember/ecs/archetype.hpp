#pragma once

#include "ember/ecs/types.hpp"

#include <ember/containers/array.hpp>
#include <ember/containers/map.hpp>
#include <ember/core/export.hpp>
#include <ember/memory/unique_ptr.hpp>

namespace ember::ecs {
    /**
     * @brief Contains data for an entity archetype.
     * @details An entity archetype is defined by it's unique combination of components.
     */
    class EMBER_API archetype {
        //TYPES
    public:
        class component_helpers {
        public:
            virtual void move(std::byte *source, std::byte *destination) const = 0;
            virtual void destruct(std::byte *memory) const                     = 0;

            virtual std::size_t get_size() const  = 0;
            virtual component_id_t get_id() const = 0;
        };

        template<typename component_t>
        class component_helpers_impl : public component_helpers {
        public:
            void move(std::byte *source, std::byte *destination) const override;
            void destruct(std::byte *memory) const override;

            std::size_t get_size() const override;
            component_id_t get_id() const override;
        };

    private:
        /**
         * @brief Memory arena for the components in this archetype.
         * All components are stored contigously.
         */
        struct component_arena {
            std::byte *memory{ nullptr };
            std::size_t bytes{ 0 }; /**< Total size of the pool. */

            std::size_t stride{ 0 }; /**< Total size of all component types. */
            std::size_t allocations{ 0 };
        };

        //VARIABLES
    private:
        archetype_id_t id{};

        containers::map<entity, std::size_t> entities{};                                              /**< All entities belonging to this archetype. */
        containers::map<component_id_t, memory::unique_ptr<component_helpers>> *component_helper_map; /**< Maps a component id to it's helper type*/
        component_arena component_data{};                                                             /**< Memory pool for the archetype's components. */

        //FUNCTIONS
    public:
        archetype() = delete;
        inline archetype(archetype_id_t id, containers::map<component_id_t, memory::unique_ptr<component_helpers>> *component_helper_map);

        archetype(archetype const &other) = delete;
        inline archetype(archetype &&other) noexcept;

        archetype &operator=(archetype const &other) = delete;
        inline archetype &operator=(archetype &&other) noexcept;

        ~archetype();

        inline archetype_id_t const &get_id() const;

        void add_entity(entity entity);
        void transfer_entity(entity entity, archetype &previous_archetype);
        void remove_entity(entity entity);

        inline bool contains_entity(entity entity) const;
        inline bool allows_component(component_id_t component_id) const;

        template<typename component_t, typename... construct_args_t>
        component_t &alloc_component(entity entity, construct_args_t &&...construct_args);

        template<typename component_t>
        component_t &get_component(entity entity);

    private:
        void increase_arena_size();

        template<typename component_t>
        std::size_t get_component_offset() const;
        std::size_t get_component_offset(component_id_t component_id) const;

        template<typename component_t>
        std::byte *get_component_memory(entity entity) const;
        std::byte *get_component_memory(entity entity, component_id_t component_id) const;
    };
}

#include "archetype.inl"