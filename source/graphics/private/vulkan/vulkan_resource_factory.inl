namespace ember::graphics {
    vulkan_resource_factory::vulkan_resource_factory(vulkan_resource_factory &&other) noexcept = default;

    vulkan_resource_factory &vulkan_resource_factory::operator=(vulkan_resource_factory &&other) noexcept = default;

    vulkan_resource_factory::~vulkan_resource_factory() = default;
}