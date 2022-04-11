set(TARGET_NAME ember_memory)

set(SOURCE_PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/ember/memory)
set(SOURCE_PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private)

#Library
add_library(
    ${TARGET_NAME}

    ${SOURCE_PRIVATE}/allocators/global_allocator.hpp
    ${SOURCE_PRIVATE}/allocators/global_allocator.inl
    ${SOURCE_PRIVATE}/allocators/global_allocator.cpp

    ${SOURCE_PUBLIC}/memory.hpp
    ${SOURCE_PUBLIC}/memory.inl
    ${SOURCE_PRIVATE}/memory.cpp

    ${SOURCE_PUBLIC}/unique_ptr.hpp
    ${SOURCE_PUBLIC}/unique_ptr.inl
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