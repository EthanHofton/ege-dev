include(FetchContent)
FetchContent_Declare(
  ere
  GIT_REPOSITORY https://github.com/EthanHofton/ere.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(ere)


set(EXE_NAME "ege")

set(SOURCES
    main_layer.cpp
    main.cpp
)


add_executable(${EXE_NAME} ${SOURCES})

target_include_directories(${EXE_NAME} PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include}
)

target_link_libraries(${EXE_NAME} PUBLIC ere)
