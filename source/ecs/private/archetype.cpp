#include "ember/ecs/archetype.hpp"

#include <ember/memory/memory.hpp>

namespace ember::ecs {
    archetype::archetype(archetype &&other) noexcept
        : id{ std::move(other.id) }
        , entities{ std::move(other.entities) }
        , component_helper_map{ other.component_helper_map }
        , component_data{ std::move(other.component_data) } {
        other.component_data.memory = nullptr;
    }

    archetype &archetype::operator=(archetype &&other) noexcept {
        id                   = std::move(other.id);
        entities             = std::move(other.entities);
        component_helper_map = other.component_helper_map;
        component_data       = std::move(other.component_data);

        other.component_data.memory = nullptr;

        return *this;
    }

    archetype::~archetype() {
        if(component_data.memory != nullptr) {
            std::byte *memory{ component_data.memory };
            for(std::size_t i{ 0 }; i < component_data.allocations; ++i) {
                for(auto &component_id : id) {
                    auto &component{ component_helper_map->at(component_id) };

                    component->destruct(memory);
                    
                    memory += component->get_size();
                }
            }
            memory::free(component_data.memory);
        }
    }

    void archetype::add_entity(entity const entity) {
        if((component_data.allocations * component_data.stride) + component_data.stride > component_data.bytes) {
            increase_arena_size();
        }
        ++component_data.allocations;

        entities[entity] = component_data.allocations - 1;
    }

    void archetype::transfer_entity(entity const entity, archetype &previous_archetype) {
        add_entity(entity);

        for(auto &component_id : id) {
            if(previous_archetype.allows_component(component_id)) {
                std::byte *old_component_memory{ previous_archetype.get_component_memory(entity, component_id) };
                std::byte *new_component_memory{ get_component_memory(entity, component_id) };

                component_helper_map->at(component_id)->move(old_component_memory, new_component_memory);
            }
        }

        previous_archetype.remove_entity(entity);
    }

    void archetype::remove_entity(entity const entity) {
        for(auto &component_id : id) {
            std::byte *component_memory{ get_component_memory(entity, component_id) };
            component_helper_map->at(component_id)->destruct(component_memory);
        }

        std::size_t const to_remove_index{ component_data.allocations - 1 };
        ecs::entity to_move_entity{};
        for(auto &&[tracked_entity, entity_index] : entities) {
            if(entity_index == to_remove_index) {
                to_move_entity = tracked_entity;
                break;
            }
        }
        EMBER_CHECK(to_move_entity != null_entity);

        if(to_move_entity != entity) {
            for(auto &component_id : id) {
                std::byte *old_component_memory{ get_component_memory(to_move_entity, component_id) };
                std::byte *new_component_memory{ get_component_memory(entity, component_id) };

                component_helper_map->at(component_id)->move(old_component_memory, new_component_memory);
            }
            entities.at(to_move_entity) = entities.at(entity);
        }

        --component_data.allocations;
        entities.erase(entity);
    }

    void archetype::increase_arena_size() {
        if(component_data.bytes == 0) {
            component_data.bytes  = component_data.stride;
            component_data.memory = memory::alloc(component_data.bytes, 0);
        } else {
            std::byte *old_memory{ component_data.memory };

            component_data.bytes *= 2;
            component_data.memory = memory::alloc(component_data.bytes, 0);

            std::byte *source{ old_memory };
            std::byte *destination{ component_data.memory };
            for(std::size_t i{ 0 }; i < component_data.allocations; ++i) {
                for(auto &component_id : id) {
                    auto &component{ component_helper_map->at(component_id) };

                    component->move(source, destination);

                    source += component->get_size();
                    destination += component->get_size();
                }
            }

            memory::free(old_memory);
        }
    }

    std::size_t archetype::get_component_offset(component_id_t const component_id) const {
        EMBER_CHECK(allows_component(component_id));

        std::size_t offset{ 0 };
        for(auto c_id : id) {
            if(c_id == component_id) {
                return offset;
            } else {
                offset += component_helper_map->at(c_id)->get_size();
            }
        }

        EMBER_CHECK_MSG(false, "Could not find component.");
        return 0;
    }

    std::byte *archetype::get_component_memory(entity const entity, component_id_t const component_id) const {
        EMBER_CHECK(contains_entity(entity));

        std::size_t const entity_index{ entities.at(entity) };
        return component_data.memory + (component_data.stride * entity_index) + get_component_offset(component_id);
    }
}