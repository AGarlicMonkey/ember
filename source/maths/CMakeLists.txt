set(TARGET_NAME ember_maths)

set(SOURCE_PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/ember/maths)
set(SOURCE_PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private)

#Library
add_library(
    ${TARGET_NAME} INTERFACE
)

#Include
target_include_directories(
    ${TARGET_NAME}
        
    INTERFACE
        public  
)

#Modules
target_link_libraries(
    ${TARGET_NAME}

    INTERFACE
		ember_core
        ember_containers
)

if(EMBER_BUILD_TESTS)
	add_subdirectory(tests)
endif()