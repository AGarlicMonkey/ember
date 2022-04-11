set(TARGET_NAME ember_core)

set(SOURCE_PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/ember/core)
set(SOURCE_PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private)

set(TRACY_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/../../external/tracy_profiler/client)

#CMake
option(EMBER_CORE_ENABLE_LOGGING "Enables the logging macros in ember. Allowing the application to output log messages." ON)
option(EMBER_CORE_ENABLE_ASSERTIONS "Enables the assertion macros in ember." ON)
option(EMBER_CORE_ENABLE_EXCEPTIONS "Enables the exception macros in ember. Allowing the application to throw." ON)

option(EMBER_CORE_ENABLE_PROFILING "Enables ember's profiler" OFF)

find_package(spdlog REQUIRED CONFIG)
find_package(tracy REQUIRED CONFIG)

#Library
add_library(
    ${TARGET_NAME}
        ${SOURCE_PUBLIC}/exception.hpp
        ${SOURCE_PUBLIC}/export.hpp
        ${SOURCE_PUBLIC}/log.hpp
        ${SOURCE_PUBLIC}/log.inl
        ${SOURCE_PRIVATE}/log.cpp
        ${SOURCE_PUBLIC}/profiling.hpp

        ${TRACY_SOURCE}/Tracy.hpp
        ${TRACY_SOURCE}/TracyClient.cpp
)

#Include
target_include_directories(
    ${TARGET_NAME}

    PUBLIC
        public
        ${CMAKE_SOURCE_DIR}/external/tracy_profiler/client

    PRIVATE
        private   
)

#Modules
target_link_libraries(
    ${TARGET_NAME}

    PUBLIC
		spdlog::spdlog
        tracy::tracy
)

#Preprocessor
target_compile_definitions(
    ${TARGET_NAME}
        PUBLIC
            EMBER_EXPORT=$<BOOL:${BUILD_SHARED_LIBS}>

            EMBER_CORE_ENABLE_LOGGING=$<BOOL:${EMBER_CORE_ENABLE_LOGGING}>
            EMBER_CORE_ENABLE_ASSERTIONS=$<BOOL:${EMBER_CORE_ENABLE_ASSERTIONS}>
            EMBER_CORE_ENABLE_EXCEPTIONS=$<BOOL:${EMBER_CORE_ENABLE_EXCEPTIONS}>

            EMBER_CORE_ENABLE_PROFILING=$<BOOL:${EMBER_CORE_ENABLE_PROFILING}>

            $<$<BOOL:${EMBER_CORE_ENABLE_PROFILING}>:TRACY_ENABLE>
            $<$<AND:$<BOOL:${EMBER_CORE_ENABLE_PROFILING}>,$<BOOL:${BUILD_SHARED_LIBS}>>:TRACY_IMPORTS>

        PRIVATE
            EMBER

            $<$<AND:$<BOOL:${EMBER_CORE_ENABLE_PROFILING}>,$<BOOL:${BUILD_SHARED_LIBS}>>:TRACY_EXPORTS>
)