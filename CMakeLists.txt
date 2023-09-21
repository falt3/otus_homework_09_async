cmake_minimum_required(VERSION 3.2)


set(PATCH_VERSION "0" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

set(myProject Async)
project(${myProject} VERSION ${PROJECT_VESRION})

set(nameApp asyncapp)
set(SOURCES app/main.cpp)
set(SOURCES_LIB 
        lib/async.cpp 
        lib/base.cpp 
        lib/interpret.cpp
        lib/pool.cpp
)


add_executable(${nameApp} ${SOURCES} ${SOURCES_LIB})
# add_executable(testIp test.cpp ip.cpp ip.h)
target_include_directories(${nameApp} PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
)


set_target_properties(
    ${nameApp} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)


target_compile_options(
    ${nameApp} PRIVATE
    -Wall -Wextra -pedantic -Werror
)    

# install(TARGETS ${nameApp} RUNTIME DESTINATION bin)

# set(CPACK_GENERATOR DEB)
# set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
# set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
# set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
# set(CPACK_PACKAGE_CONTACT example@example.com)
# include(CPack)
