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

    template<typename component_t, typename... construct_args_t>
    component_t &entity_manager::add_component(entity const entity, construct_args_t &&...construct_args) {
        EMBER_THROW_IF_FAILED(is_valid(entity), exception{ "Cannot add component on an invalid entity." });
        return components.add<component_t>(entity, std::forward<construct_args_t>(construct_args)...);
    }

    template<typename component_t>
    bool entity_manager::has_component(entity const entity) {
        EMBER_THROW_IF_FAILED(is_valid(entity), exception{ "Cannot check a component on an invalid entity." });
        return components.has<component_t>(entity);
    }

    template<typename component_t>
    component_t &entity_manager::get_component(entity const entity) {
        EMBER_THROW_IF_FAILED(is_valid(entity), exception{ "Cannot get component on an invalid entity." });
        return components.get<component_t>(entity);
    }

    template<typename component_t>
    void entity_manager::remove_component(entity const entity) {
        EMBER_THROW_IF_FAILED(is_valid(entity), exception{ "Cannot remove component on an invalid entity." });
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