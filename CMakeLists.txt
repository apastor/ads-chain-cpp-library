cmake_minimum_required(VERSION 3.13)
project(adschain VERSION 1.1 DESCRIPTION "ads.chain C++ library")

#set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -fPIC")
set_property (GLOBAL PROPERTY USE_FOLDERS ON)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG=1)
endif ()

if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else ()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif ()

add_library(adschain SHARED "")
set_target_properties(adschain PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(adschain PROPERTIES SOVERSION 1)
set_target_properties(adschain PROPERTIES PUBLIC_HEADER $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/include/>)

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
target_link_libraries(adschain ${CONAN_LIBS})

target_include_directories(adschain
    PUBLIC
    $<INSTALL_INTERFACE:include/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/include>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/src
    )

add_subdirectory(lib/src)
add_subdirectory(lib/include)

# Now enable our tests.
enable_testing()
add_subdirectory(lib/test)

include(GNUInstallDirs)
install(TARGETS adschain
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )