namespace ember::graphics {
    shader_cache::shader_cache() = default;

    shader_cache::shader_cache(shader_cache &&other) noexcept = default;

    shader_cache &shader_cache::operator=(shader_cache &&other) noexcept = default;

    shader_cache::~shader_cache() = default;

    containers::array<std::uint32_t> shader_cache::get_spriv_for_shader(std::string const &shader_name) {
        //TODO: Look up if it is already being compiled
        return compiled_shaders.at(shader_name);
    }
}