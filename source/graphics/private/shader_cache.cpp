#include "ember/graphics/shader_cache.hpp"

#include "ember/core/log.hpp"
#include "ember/graphics/exception.hpp"

#include <fstream>
#include <shaderc/shaderc.hpp>

EMBER_LOG_CATEGORY(EmberGraphicsShaderCompiler);

namespace {
    std::filesystem::path const default_save_dir{ std::filesystem::current_path() };

    ember::shader_cache *instance{ nullptr };

    ember::array<char> read_file(std::filesystem::path const &file_path) {
        //Start at the end so we can get the file size
        std::ifstream file{ file_path.c_str(), std::ios::ate | std::ios::binary };
        EMBER_THROW_IF_FAILED(file.is_open(), ember::shader_compilation_failed_exception("Failed to open shader file.", file_path.filename().string()));

        auto const file_size{ file.tellg() };
        ember::array<char> buffer(static_cast<std::size_t>(file_size));

        file.seekg(0);
        file.read(buffer.data(), static_cast<std::streamsize>(file_size));

        file.close();

        return buffer;
    }

    shaderc_shader_kind convert_shader_stage(ember::shader::stage const shader_stage) {
        switch(shader_stage) {
            case ember::shader::stage::vertex:
                return shaderc_vertex_shader;
            case ember::shader::stage::pixel:
                return shaderc_fragment_shader;
            case ember::shader::stage::compute:
                return shaderc_compute_shader;
            default:
                EMBER_CHECK(false);
                return shaderc_vertex_shader;
        }
    }
}

namespace ember::inline graphics {
    void shader_cache::add_compile_job(std::string const &shader_name, std::filesystem::path const &source_path, shader::stage const shader_stage) {
        EMBER_THROW_IF_FAILED(source_path.has_filename(), shader_compilation_failed_exception("Failed to compile shader. source_path is not a file", source_path.string()));

        array<char> const source{ read_file(source_path) };
        raw_shaders[shader_name] = std::string{ source.data(), source.size() };

        //TODO: Actually add jobs instead of doing it synchronously
        compiled_shaders[shader_name] = compile(shader_name, raw_shaders[shader_name], shader_stage);
    }

    array<std::uint32_t> shader_cache::compile(std::string const &shader_name, std::string const &shader_source, shader::stage const shader_stage) {
        shaderc::CompileOptions options{};
        //TODO: set includer
#if EMBER_GRAPHICS_COMPILE_DEBUG_SHADERS
        options.SetOptimizationLevel(shaderc_optimization_level_zero);
#else
        options.SetOptimizationLevel(shaderc_optimization_level_performance);
#endif

        shaderc::Compiler compiler{};
        shaderc::SpvCompilationResult const spirv_result{ compiler.CompileGlslToSpv(shader_source, convert_shader_stage(shader_stage), shader_name.data(), options) };

        if(spirv_result.GetCompilationStatus() != shaderc_compilation_status_success) {
            EMBER_LOG(EmberGraphicsShaderCompiler, log_level::error, "Failed to compile shader {0}:\n\t{1}", shader_name, spirv_result.GetErrorMessage());
            EMBER_THROW(shader_compilation_failed_exception("Failed to compile shader. Compilation status was not success.", shader_name));
        }

        EMBER_LOG(EmberGraphicsShaderCompiler, log_level::trace, "Compiled shader {0} into SPIR-V", shader_name);
        return array<std::uint32_t>{ spirv_result.begin(), spirv_result.end() };
    }
}