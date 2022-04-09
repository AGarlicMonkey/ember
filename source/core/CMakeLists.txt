set(TARGET_NAME ember_core)

set(SOURCE_PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/ember/core)
set(SOURCE_PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private)

#CMake
find_package(spdlog REQUIRED CONFIG)

#Library
add_library(
    ${TARGET_NAME}
        ${SOURCE_PUBLIC}/export.hpp
        ${SOURCE_PUBLIC}/log.hpp
        ${SOURCE_PUBLIC}/log.inl
        ${SOURCE_PRIVATE}/log.cpp
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
		spdlog::spdlog
)

#Preprocessor
target_compile_definitions(
    ${TARGET_NAME}
        PUBLIC
            EMBER_EXPORT=$<BOOL:${BUILD_SHARED_LIBS}>

            EMBER_CORE_ENABLE_LOGGING=1
            EMBER_CORE_ENABLE_ASSERTIONS=1
            EMBER_CORE_ENABLE_EXCEPTIONS=1

        PRIVATE
            EMBER
)