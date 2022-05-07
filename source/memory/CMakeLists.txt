set(TARGET_NAME ember_memory)

set(SOURCE_PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/ember/memory)
set(SOURCE_PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private)

#CMake
option(EMBER_MEMORY_DETAILED_LOGGING "Allows allocators to log every operation. Leaving this on can produce extremely large log files and should only be used to track hard to find bugs." OFF)
option(EMBER_MEMORY_DISABLE_GLOBAL_ALLOCATOR "Disables the global allocator using std::malloc and std::free instead. This can be used to find bugs where objects access memory that is not theirs." OFF)

#Library
add_library(
    ${TARGET_NAME}

    ${SOURCE_PRIVATE}/allocators/global_allocator.hpp
    ${SOURCE_PRIVATE}/allocators/global_allocator.inl
    ${SOURCE_PRIVATE}/allocators/global_allocator.cpp

    ${SOURCE_PUBLIC}/exception.hpp

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

        EMBER_MEMORY_DETAILED_LOGGING=$<BOOL:${EMBER_MEMORY_DETAILED_LOGGING}>
        EMBER_MEMORY_DISABLE_GLOBAL_ALLOCATOR=$<BOOL:${EMBER_MEMORY_DISABLE_GLOBAL_ALLOCATOR}>
)

if(EMBER_BUILD_TESTS)
	add_subdirectory(tests)
endif()