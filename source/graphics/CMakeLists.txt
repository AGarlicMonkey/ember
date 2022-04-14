set(TARGET_NAME ember_graphics)

set(SOURCE_PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/ember/graphics)
set(SOURCE_PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private)

#CMake
option(EMBER_GRAPHICS_DEVICE_VALIDATION "Enables API specific device validation layers." ON)
option(EMBER_GRAPHICS_USAGE_VALIDATION "Enables ember's API usage validation. This helps prevent issues developing a renderer for one API for it not to work on another." ON)
option(EMBER_GRAPHICS_TRACK_MEMORY "Enables extra logging of memory state within a device." ON)

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

    ${SOURCE_PRIVATE}/vulkan/allocation_callbacks.hpp
    ${SOURCE_PRIVATE}/vulkan/allocation_callbacks.inl
    ${SOURCE_PRIVATE}/vulkan/allocation_callbacks.cpp
    ${SOURCE_PRIVATE}/vulkan/exception.hpp
    ${SOURCE_PRIVATE}/vulkan/log.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_device.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_device.cpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_instance.hpp
    ${SOURCE_PRIVATE}/vulkan/vulkan_instance.cpp
)

#Library
add_library(
    ${TARGET_NAME}

    ${SOURCE_PUBLIC}/device.hpp
    ${SOURCE_PUBLIC}/graphics.hpp
    ${SOURCE_PRIVATE}/graphics.cpp
    ${SOURCE_PUBLIC}/instance.hpp

    ${SOURCE_PUBLIC}/command_buffer.hpp
    ${SOURCE_PUBLIC}/command_buffer.inl
    ${SOURCE_PRIVATE}/command_buffer.cpp

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
        $<$<PLATFORM_ID:Windows>:Vulkan::Vulkan>
        $<$<PLATFORM_ID:Linux>:Vulkan::Vulkan>
)

#Preprocessor
target_compile_definitions(
    ${TARGET_NAME}

    PRIVATE
        EMBER

        EMBER_GRAPHICS_DEVICE_VALIDATION=$<BOOL:${EMBER_GRAPHICS_DEVICE_VALIDATION}>
        EMBER_GRAPHICS_USAGE_VALIDATION=$<BOOL:${EMBER_GRAPHICS_USAGE_VALIDATION}>
        EMBER_GRAPHICS_TRACK_MEMORY=$<BOOL:${EMBER_GRAPHICS_TRACK_MEMORY}>
)