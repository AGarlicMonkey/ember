#include <ember/core/log.hpp>

namespace ember::ecs {
    entity_manager::entity_manager() = default;

    entity_manager::entity_manager(entity_manager &&other) noexcept = default;

    entity_manager &entity_manager::operator=(entity_manager &&other) noexcept = default;

    entity_manager::~entity_manager() = default;

    entity entity_manager::create() {
        entity entity{ ++next_entity };
        entities.push_back(entity);

        return entity;
    }

    void entity_manager::destroy(entity &entity) {
        entities.erase(entity);
        entity = null_entity;
    }

    void entity_manager::destroy_all() {
        entities.clear();
    }

    template<typename component_t, typename... construct_args_t>
    component_t &entity_manager::add_component(entity entity, construct_args_t &&...construct_args) {
        EMBER_CHECK(is_valid(entity));
        return components.add_component<component_t>(entity, std::forward<construct_args_t>(construct_args)...);
    }

    template<typename component_t>
    bool entity_manager::has_component(entity entity) {
        EMBER_CHECK(is_valid(entity));
        return components.has_component<component_t>(entity);
    }

    template<typename component_t>
    component_t &entity_manager::get_component(entity entity) {
        EMBER_CHECK(is_valid(entity));
        return components.get_component<component_t>(entity);
    }

    template<typename component_t>
    void entity_manager::remove_component(entity entity) {
        EMBER_CHECK(is_valid(entity));
        components.remove_component<component_t>(entity);
    }
}