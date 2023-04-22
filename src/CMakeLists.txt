include(FetchContent)

FetchContent_Declare(
  ere
  GIT_REPOSITORY https://github.com/EthanHofton/ere.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(ere)

FetchContent_Declare(
    EnTT
  GIT_REPOSITORY https://github.com/skypjack/entt.git
)
FetchContent_MakeAvailable(EnTT)



set(EXE_NAME "ege")

set(SOURCES
    scene_view_system.cpp
    inspector_system.cpp
    scene_system.cpp
    system_manager.cpp
    system.cpp
    scene.cpp
    main_layer.cpp
    main.cpp
)


add_executable(${EXE_NAME} ${SOURCES})

target_include_directories(${EXE_NAME} PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include}
)

target_link_libraries(${EXE_NAME} PUBLIC
    ere
)

target_link_libraries(${EXE_NAME} PRIVATE
    EnTT::EnTT
)
