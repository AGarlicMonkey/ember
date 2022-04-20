#pragma once

#include "ember/graphics/shader.hpp"

#include <cinttypes>
#include <ember/containers/array.hpp>
#include <ember/core/export.hpp>
#include <filesystem>
#include <string>
#include <ember/containers/map.hpp>

namespace ember::graphics {
    /**
     * @brief 
     */
    class EMBER_API shader_cache {
        //VARIABLES
    private:
        inline static containers::map<std::string, std::string> raw_shaders{};                           /**< Contains raw glsl shader code with the name as the key. */
        inline static containers::map<std::string, containers::array<std::uint32_t>> compiled_shaders{}; /**< Contains SPIR-V byte code compiled shader with the name as key. */

        //FUNCTIONS
    public:
        inline shader_cache();

        shader_cache(shader_cache const &other) = delete;
        inline shader_cache(shader_cache &&other) noexcept;

        shader_cache &operator=(shader_cache const &other) = delete;
        inline shader_cache &operator=(shader_cache &&other) noexcept;

        inline virtual ~shader_cache();

        /**
         * @brief Adds a shader compilation job.
         * @details It is best to use the macro EMBER_REGISTER_SHADER rather than call this function directly
         * @param shader_name 
         * @param source_path 
         * @param shader_stage 
         */
        static void add_compile_job(std::string const &shader_name, std::filesystem::path const &source_path, shader::stage const shader_stage);

        /**
         * @brief Returns the compiled shader data for shader_name.
         * @details The lifetime of the shader object is tied to the lifetime of this cache.
         * @param shader_name 
         * @return 
         */
        virtual shader *get_shader(std::string const &shader_name) = 0;

    protected:
        inline containers::array<std::uint32_t> get_spriv_for_shader(std::string const &shader_name);

    private:
        static containers::array<std::uint32_t> compile(std::string const &shader_name, std::string const &shader_source, shader::stage const shader_stage);
    };
}

#include "shader_cache.inl"

#define INTERNAL_SHADER_CAT_2(a, b) a##b
#define INTERNAL_SHADER_CAT(a, b) INTERNAL_SHADER_CAT_2(a, b)

/**
 * @brief 
 * @param shader_type 
 * @param file_path 
 * @param shader_stage 
 */
#define EMBER_REGISTER_SHADER(shader_type, file_path, shader_stage)               \
    struct shader_compiler_helper_##shader_type {                                 \
        shader_compiler_helper_##shader_type() {                                  \
            shader_cache::add_compile_job(#shader_type, file_path, shader_stage); \
        }                                                                         \
    };                                                                            \
    static inline shader_compiler_helper_##shader_type const INTERNAL_SHADER_CAT(shader_type, __COUNTER__){};