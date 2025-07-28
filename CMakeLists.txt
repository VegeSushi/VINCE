cmake_minimum_required(VERSION 3.10)
project(vince)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    include_directories(
        ${CMAKE_SOURCE_DIR}/packages/allegro.5.2.7.0/build/native/include
        ${CMAKE_SOURCE_DIR}/packages/lua.5.4.0/build/native/include
    )

    link_directories(
        ${CMAKE_SOURCE_DIR}/packages/allegro.5.2.7.0/build/native/lib
        ${CMAKE_SOURCE_DIR}/packages/lua.5.4.0/build/native/lib
    )

    add_executable(vince main.cpp)

    target_link_libraries(vince
        allegro-5.2
        allegro_video-5.2
        allegro_image-5.2
        allegro_ttf-5.2
        allegro_font-5.2
        lua54
    )

else()
    include_directories(/usr/include/allegro5 /usr/include/lua5.4)

    link_directories(/usr/lib)

    add_executable(vince main.cpp)

    target_link_libraries(vince
        allegro
        allegro_video
        allegro_image
        allegro_ttf
        allegro_font
        allegro_image
        lua5.4
    )
endif()
