project (ugui)
cmake_minimum_required(VERSION 3.0)

### Common Settings
set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")

### SHARED Library
set(LIB_SRCS ugui.c)
include_directories(.)
add_library(ugui SHARED ${LIB_SRCS})
set_target_properties(ugui PROPERTIES PUBLIC_HEADER ugui.h)
set_target_properties(ugui PROPERTIES SOVERSION "1")
install(TARGETS ugui DESTINATION lib PUBLIC_HEADER DESTINATION inc)

### Example executable
find_package(PkgConfig REQUIRED)
pkg_check_modules(DRM REQUIRED libdrm)
add_executable(drm_example examples/drm/drm_backend.c examples/drm/drm_example.c)
target_link_libraries(drm_example ${DRM_LIBRARIES})
target_include_directories(drm_example PUBLIC ${DRM_INCLUDE_DIRS})
target_compile_options(drm_example PUBLIC ${DRM_CFLAGS_OTHER} -Wall -Werror "-D_FILE_OFFSET_BITS=64")
target_link_libraries(drm_example ugui)
install(TARGETS drm_example RUNTIME DESTINATION bin)

### Debian Package generation
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Franz Flasch")
include(CPack)
