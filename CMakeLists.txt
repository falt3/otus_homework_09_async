cmake_minimum_required(VERSION 3.2)


set(PATCH_VERSION "0" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(async VERSION ${PROJECT_VERSION})

option(BUILD_ONE23 "Build app & lib" OFF)

set(SOURCES_APP 
        main.cpp)

if (ON)
    message(STATUS "Build library")
    set(nameApp asynctest)

    add_subdirectory(./lib)
    
    add_executable(${nameApp} ${SOURCES_APP})

    target_include_directories(${nameApp} PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/lib)

    # find_package(async CONFIG REQUIRED)
    # target_link_libraries(${nameApp} Async::async)
    target_link_libraries(${nameApp} async)

    
    install(TARGETS ${nameApp} RUNTIME DESTINATION bin)

    # set(CPACK_GENERATOR "DEB")
    # set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
    # set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
    # set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
    # set(CPACK_PACKAGE_CONTACT example@example.com)
    # include(CPack)    
else() 
    message(STATUS "Build application")
    set(nameApp asyncapp)

    set(SOURCES_LIB 
        lib/async.cpp 
        lib/interpret.cpp
        lib/pool.cpp)

    add_executable(${nameApp} ${SOURCES_APP} ${SOURCES_LIB})
    
    target_include_directories(${nameApp} PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
    )
endif()


set_target_properties(
    ${nameApp} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(
    ${nameApp} PRIVATE
    -Wall -Wextra -pedantic -Werror
)  
