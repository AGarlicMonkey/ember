#include "ember/ecs/archetype.hpp"

#include <ember/memory/memory.hpp>

namespace ember::inline ecs {
    archetype::archetype(archetype_id_t id, map<component_id_t, unique_ptr<internal::component_helpers>> *component_helper_map)
        : id{ std::move(id) }
        , component_helper_map{ component_helper_map } {
        component_data.stride = 0;
        for(auto component_id : this->id) {
            component_data.stride += component_helper_map->at(component_id)->get_size();
        }
        EMBER_CHECK(component_data.stride != 0);

        std::size_t offset{ 0 };
        for(auto component_id : this->id) {
            component_offsets[component_id] = offset;
            offset += component_helper_map->at(component_id)->get_size();
        }
        EMBER_CHECK(component_offsets.size() != 0);
    }

    archetype::archetype(archetype &&other) noexcept
        : id{ std::move(other.id) }
        , entity_to_index{ std::move(other.entity_to_index) }
        , index_to_entity{ std::move(other.index_to_entity) }
        , component_helper_map{ other.component_helper_map }
        , component_offsets{ std::move(other.component_offsets) } {
        destruct_memory_arena();
        component_data              = std::move(other.component_data);
        other.component_data.memory = nullptr;
    }

    archetype &archetype::operator=(archetype &&other) noexcept {
        id                   = std::move(other.id);
        entity_to_index      = std::move(other.entity_to_index);
        index_to_entity      = std::move(index_to_entity);
        component_helper_map = other.component_helper_map;
        component_offsets    = std::move(other.component_offsets);

        destruct_memory_arena();
        component_data              = std::move(other.component_data);
        other.component_data.memory = nullptr;

        return *this;
    }

    archetype::~archetype() {
        destruct_memory_arena();
    }

    void archetype::add_entity(entity const entity) {
        if((component_data.allocations * component_data.stride) + component_data.stride > component_data.bytes) {
            increase_arena_size();
        }
        ++component_data.allocations;

        std::size_t const index{ component_data.allocations - 1 };
        entity_to_index[entity] = index;
        index_to_entity[index]  = entity;
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

        EMBER_CHECK(contains_entity(entity));
        EMBER_CHECK(!previous_archetype.contains_entity(entity));
    }

    void archetype::remove_entity(entity const entity) {
        for(auto &component_id : id) {
            std::byte *component_memory{ get_component_memory(entity, component_id) };
            component_helper_map->at(component_id)->destruct(component_memory);
        }

        std::size_t const to_move_index{ component_data.allocations - 1 };
        ecs::entity const to_move_entity{ index_to_entity.at(to_move_index) };
        EMBER_CHECK(to_move_entity != null_entity);

        //If we removed from the middle of the array then move the components at the end to the new location
        if(to_move_entity != entity) {
            for(auto &component_id : id) {
                std::byte *old_component_memory{ get_component_memory(to_move_entity, component_id) };
                std::byte *new_component_memory{ get_component_memory(entity, component_id) };

                component_helper_map->at(component_id)->move(old_component_memory, new_component_memory);
            }
            entity_to_index.at(to_move_entity)                     = entity_to_index.at(entity);
            index_to_entity.at(entity_to_index.at(to_move_entity)) = to_move_entity;
        }

        --component_data.allocations;
        entity_to_index.erase(entity);
        index_to_entity.erase(to_move_index);
        EMBER_CHECK(entity_to_index.size() == index_to_entity.size());
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

    std::byte *archetype::get_component_memory(entity const entity, component_id_t const component_id) const {
        EMBER_CHECK(contains_entity(entity));

        std::size_t const entity_index{ entity_to_index.at(entity) };
        return component_data.memory + (component_data.stride * entity_index) + get_component_offset(component_id);
    }

    void archetype::destruct_memory_arena() {
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
            component_data.memory = nullptr;
        }
    }
}