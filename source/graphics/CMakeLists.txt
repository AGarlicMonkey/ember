set(TARGET_NAME ember_graphics)

set(SOURCE_PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/ember/graphics)
set(SOURCE_PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private)

#CMake
option(EMBER_GRAPHICS_DEVICE_VALIDATION "Enables API specific device validation layers." ON)
option(EMBER_GRAPHICS_USAGE_VALIDATION "Enables ember's API usage validation. This helps prevent issues developing a renderer for one API and for it not to work on another." ON)
option(EMBER_GRAPHICS_TRACK_MEMORY "Enables extra logging of memory state within a device." ON)
option(EMBER_GRAPHICS_DEBUG_UTILITIES "Enables the graphics debugging utilities such as naming resources and adding user markers." ON)
option(EMBER_GRAPHICS_COMPILE_DEBUG_SHADERS "Compiles shaders in debug mode, with no optimisations enabled." OFF)

find_package(shaderc REQUIRED CONFIG)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "Linux")
	find_package(Vulkan REQUIRED)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	#TODO
endif()

set(
    validation_files


)

set(
    vulkan_files

    ${SOURCE_PRIVATE}/vulkan/device_memory_allocator.hpp
    ${SOURCE_PRIVATE}/vulkan/device_memory_allocator.inl
    ${SOURCE_PRIVATE}/vulkan/device_memory_allocator.cpp
    ${SOURCE_PRIVATE}/vulkan/host_memory_allocator.hpp
    ${SOURCE_PRIVATE}/vulkan/host_memory_allocator.cpp
    ${SOURCE_PRIVATE}/vulkan/log.hpp
    ${SOURCE_PRIVATE}/vulkan/memory_barrier_simplification.hpp
    ${SOURCE_PRIVATE}/vulkan/memory_barrier_simplification.cpp
    ${SOURCE_PRIVATE}/vulkan/types.hpp
    ${SOURCE_PRIVATE}/vulkan/verification.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_extension_functions.hpp

    ${SOURCE_PRIVATE}/vulkan/vulkan_instance.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_instance.cpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_device.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_device.inl
    ${SOURCE_PRIVATE}/vulkan/vulkan_device.cpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_resource_factory.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_resource_factory.inl
    ${SOURCE_PRIVATE}/vulkan/vulkan_resource_factory.cpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_swapchain.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_swapchain.inl
    ${SOURCE_PRIVATE}/vulkan/vulkan_swapchain.cpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_queue.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_queue.inl
    ${SOURCE_PRIVATE}/vulkan/vulkan_queue.cpp

    ${SOURCE_PRIVATE}/vulkan/vulkan_buffer.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_buffer.inl
    ${SOURCE_PRIVATE}/vulkan/vulkan_buffer.cpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_image.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_image.inl
    ${SOURCE_PRIVATE}/vulkan/vulkan_image.cpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_image_view.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_image_view.inl
    ${SOURCE_PRIVATE}/vulkan/vulkan_image_view.cpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_shader.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_shader.inl
    ${SOURCE_PRIVATE}/vulkan/vulkan_shader.cpp

    ${SOURCE_PRIVATE}/vulkan/vulkan_graphics_pipeline_object.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_graphics_pipeline_object.inl
    ${SOURCE_PRIVATE}/vulkan/vulkan_graphics_pipeline_object.cpp

    ${SOURCE_PRIVATE}/vulkan/vulkan_descriptor_set_layout.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_descriptor_set_layout.inl
    ${SOURCE_PRIVATE}/vulkan/vulkan_descriptor_set_layout.cpp

    ${SOURCE_PRIVATE}/vulkan/vulkan_render_pass.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_render_pass.inl
    ${SOURCE_PRIVATE}/vulkan/vulkan_render_pass.cpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_framebuffer.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_framebuffer.inl
    ${SOURCE_PRIVATE}/vulkan/vulkan_framebuffer.cpp

    ${SOURCE_PRIVATE}/vulkan/vulkan_fence.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_fence.inl
    ${SOURCE_PRIVATE}/vulkan/vulkan_fence.cpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_semaphore.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_semaphore.inl

    ${SOURCE_PRIVATE}/vulkan/vulkan_shader_cache.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_shader_cache.inl
    ${SOURCE_PRIVATE}/vulkan/vulkan_shader_cache.cpp
)

#Library
add_library(
    ${TARGET_NAME}

    ${SOURCE_PUBLIC}/exception.hpp
    ${SOURCE_PRIVATE}/resource_cast.hpp
    ${SOURCE_PRIVATE}/resource_cast.inl
    ${SOURCE_PUBLIC}/submission_types.hpp
    ${SOURCE_PUBLIC}/pipeline_stage.hpp

    ${SOURCE_PUBLIC}/graphics.hpp
    ${SOURCE_PRIVATE}/graphics.cpp
    ${SOURCE_PUBLIC}/instance.hpp
    ${SOURCE_PUBLIC}/device.hpp
    ${SOURCE_PUBLIC}/resource_factory.hpp
    ${SOURCE_PUBLIC}/swapchain.hpp

    ${SOURCE_PUBLIC}/graphics_command_buffer.hpp
    ${SOURCE_PUBLIC}/graphics_command_buffer.inl
    ${SOURCE_PRIVATE}/graphics_command_buffer.cpp
    ${SOURCE_PUBLIC}/compute_command_buffer.hpp
    ${SOURCE_PUBLIC}/compute_command_buffer.inl
    ${SOURCE_PRIVATE}/compute_command_buffer.cpp
    ${SOURCE_PUBLIC}/transfer_command_buffer.hpp
    ${SOURCE_PUBLIC}/transfer_command_buffer.inl
    ${SOURCE_PRIVATE}/transfer_command_buffer.cpp
    ${SOURCE_PUBLIC}/command_buffer.hpp
    ${SOURCE_PUBLIC}/command_buffer.inl
    ${SOURCE_PRIVATE}/command_buffer.cpp
    ${SOURCE_PRIVATE}/commands.hpp
    ${SOURCE_PRIVATE}/commands.cpp
    ${SOURCE_PUBLIC}/command_types.hpp

    ${SOURCE_PUBLIC}/resource.hpp
    ${SOURCE_PUBLIC}/buffer.hpp
    ${SOURCE_PUBLIC}/image.hpp
    ${SOURCE_PUBLIC}/image_view.hpp
    ${SOURCE_PUBLIC}/shader.hpp

    ${SOURCE_PUBLIC}/graphics_pipeline_object.hpp
    ${SOURCE_PUBLIC}/compute_pipeline_object.hpp
    
    ${SOURCE_PUBLIC}/descriptor_set_layout.hpp
    ${SOURCE_PUBLIC}/descriptor_set.hpp

    ${SOURCE_PUBLIC}/render_pass.hpp
    ${SOURCE_PUBLIC}/framebuffer.hpp

    ${SOURCE_PUBLIC}/fence.hpp
    ${SOURCE_PUBLIC}/semaphore.hpp

    ${SOURCE_PUBLIC}/shader_cache.hpp
    ${SOURCE_PUBLIC}/shader_cache.inl
    ${SOURCE_PRIVATE}/shader_cache.cpp

    ${validation_files}
    ${vulkan_files}
)

#Include
target_include_directories(
    ${TARGET_NAME}

    PUBLIC
        public

    PRIVATE
        private
        Vulkan::Vulkan
)

#Modules
target_link_libraries(
    ${TARGET_NAME}

    PUBLIC
        ember_containers
		ember_core
        ember_maths
        ember_memory
        ember_platform

    PRIVATE
        shaderc::shaderc

        $<$<OR:$<PLATFORM_ID:Windows>,$<PLATFORM_ID:Linux>>:Vulkan::Vulkan>
)

#Preprocessor
target_compile_definitions(
    ${TARGET_NAME}

    PUBLIC
        EMBER_GRAPHICS_DEBUG_UTILITIES=$<BOOL:${EMBER_GRAPHICS_DEBUG_UTILITIES}>

    PRIVATE
        EMBER

        EMBER_GRAPHICS_DEVICE_VALIDATION=$<BOOL:${EMBER_GRAPHICS_DEVICE_VALIDATION}>
        EMBER_GRAPHICS_USAGE_VALIDATION=$<BOOL:${EMBER_GRAPHICS_USAGE_VALIDATION}>
        EMBER_GRAPHICS_TRACK_MEMORY=$<BOOL:${EMBER_GRAPHICS_TRACK_MEMORY}>
        EMBER_GRAPHICS_COMPILE_DEBUG_SHADERS=$<BOOL:${EMBER_GRAPHICS_COMPILE_DEBUG_SHADERS}>
)