cmake_minimum_required(VERSION 3.18.0)

project(
	ember
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
  add_compile_options(/WX)
else()
  add_compile_options(-Werror)
endif()


#Set up conan
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR})

if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/conan.cmake")
	message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
	file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/release/0.17/conan.cmake"
       	          "${CMAKE_CURRENT_BINARY_DIR}/conan.cmake")
endif()
include(${CMAKE_CURRENT_BINARY_DIR}/conan.cmake)

get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(${isMultiConfig})
	foreach(TYPE ${CMAKE_CONFIGURATION_TYPES})
    	conan_cmake_autodetect(settings 
							   BUILD_TYPE ${TYPE})
   		conan_cmake_install(PATH_OR_REFERENCE ${CMAKE_CURRENT_SOURCE_DIR}
                       		BUILD missing
                       		REMOTE conancenter
                       		SETTINGS ${settings})
	endforeach()
else()
	conan_cmake_autodetect(settings)
	conan_cmake_install(PATH_OR_REFERENCE ${CMAKE_CURRENT_SOURCE_DIR}
       		            BUILD missing
               		    REMOTE conancenter
                   		SETTINGS ${settings})
endif()	

add_subdirectory(source)