set(TARGET_NAME ember_ecs)

set(SOURCE_PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/ember/ecs)
set(SOURCE_PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private)

#Library
add_library(
    ${TARGET_NAME}

    ${SOURCE_PUBLIC}/types.hpp
    ${SOURCE_PUBLIC}/entity_manager.hpp
    ${SOURCE_PUBLIC}/entity_manager.inl
    ${SOURCE_PRIVATE}/entity_manager.cpp
    ${SOURCE_PUBLIC}/archetype.hpp
    ${SOURCE_PUBLIC}/archetype.inl
    ${SOURCE_PRIVATE}/archetype.cpp
    ${SOURCE_PUBLIC}/component_manager.hpp
    ${SOURCE_PUBLIC}/component_manager.inl
    ${SOURCE_PRIVATE}/component_manager.cpp
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
        ember_memory
)

#Preprocessor
target_compile_definitions(
    ${TARGET_NAME}

    PRIVATE
        EMBER
)

if(EMBER_BUILD_TESTS)
	add_subdirectory(tests)
endif()