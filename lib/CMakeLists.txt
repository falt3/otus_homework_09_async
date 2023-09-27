cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "0" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(asynclib VERSION ${PROJECT_VESRION})

set(nameLib async)

set(SOURCES_LIB 
        async.cpp 
        interpret.cpp
        pool.cpp)

add_library(${nameLib} SHARED ${SOURCES_LIB})

set_target_properties(
    ${nameLib} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(${nameLib} PRIVATE
    -Wall -Wextra -pedantic -Werror
)

install(TARGETS ${nameLib} EXPORT ${nameLib}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)
install(FILES async.h DESTINATION include)

install(EXPORT ${nameLib}Targets
    FILE ${nameLib}Config.cmake
    NAMESPACE Asinc::
    DESTINATION lib/cmake/${nameLib}
)

message(${PROJECT_VESRION})
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)