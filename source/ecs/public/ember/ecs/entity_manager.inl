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
}