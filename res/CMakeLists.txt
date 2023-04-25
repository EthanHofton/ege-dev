# Define a new CMake target for the res directory
add_custom_target(res_target ALL
    # Copy the contents of the res directory to the build directory
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Copying res directory to build directory"
)

# Set the source directory of the res target
set_property(TARGET res_target PROPERTY SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
