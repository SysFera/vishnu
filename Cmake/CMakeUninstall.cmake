# macros inspired from CMake FAQ (licensed under CC By 2.5)
# add an uninstall target to project
# just add the following line into the top CMakeLists.txt
#   include(CMakeUninstall)
## generate uninstall script
configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake_uninstall.cmake.in"
    "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake")


