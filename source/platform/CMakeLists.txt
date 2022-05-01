set(TARGET_NAME ember_platform)

set(SOURCE_PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/ember/platform)
set(SOURCE_PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private)

#Library
add_library(
    ${TARGET_NAME}

    ${SOURCE_PUBLIC}/input.hpp

    ${SOURCE_PUBLIC}/window.hpp
    ${SOURCE_PUBLIC}/window.inl
    
    ${SOURCE_PRIVATE}/win32/win32_input.hpp
    ${SOURCE_PRIVATE}/win32/win32_input.cpp
    ${SOURCE_PRIVATE}/win32/win32_window.cpp
)

#Include
target_include_directories(
    ${TARGET_NAME}

    PUBLIC
        public

    PRIVATE
        private   
)

#Modules
target_link_libraries(
    ${TARGET_NAME}

    PUBLIC
		ember_core
        ember_containers
        ember_maths
        ember_memory
)

#Preprocessor
target_compile_definitions(
    ${TARGET_NAME}

    PUBLIC
        EMBER_PLATFORM_WIN32=$<PLATFORM_ID:Windows>
        EMBER_PLATFORM_MACOS=$<PLATFORM_ID:Darwin>
        EMBER_PLATFORM_XLIB=$<PLATFORM_ID:Linux>

    PRIVATE
        EMBER
)