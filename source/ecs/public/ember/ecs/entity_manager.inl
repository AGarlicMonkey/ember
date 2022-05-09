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
    component_t &entity_manager::add_component(entity const entity, construct_args_t &&...construct_args) {
        EMBER_CHECK(is_valid(entity));
        return components.add<component_t>(entity, std::forward<construct_args_t>(construct_args)...);
    }

    template<typename component_t>
    bool entity_manager::has_component(entity const entity) {
        EMBER_CHECK(is_valid(entity));
        return components.has<component_t>(entity);
    }

    template<typename component_t>
    component_t &entity_manager::get_component(entity const entity) {
        EMBER_CHECK(is_valid(entity));
        return components.get<component_t>(entity);
    }

    template<typename component_t>
    void entity_manager::remove_component(entity const entity) {
        EMBER_CHECK(is_valid(entity));
        components.remove<component_t>(entity);
    }

    template<typename function_t>
    void entity_manager::for_each(function_t function) {
        components.for_each(function);
    }

    template<typename function_t, typename object_t>
    void entity_manager::for_each(function_t function, object_t *object) {
        components.for_each(function, object);
    }
}