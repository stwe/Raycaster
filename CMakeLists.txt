cmake_minimum_required(VERSION 3.29)
project(Shooter)

set(CMAKE_CXX_STANDARD 20)

find_package(spdlog CONFIG REQUIRED)

add_executable(Shooter main.cpp
        Game.cpp
        Game.h
        Map.cpp
        Map.h
        Player.cpp
        Player.h
        Utils.h
        Ray.cpp
        Ray.h
        Assert.h
        Log.h
        ini.h
)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(${PROJECT_NAME} PUBLIC FPS_DEBUG_BUILD SPDLOG_NO_EXCEPTIONS)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC SPDLOG_NO_EXCEPTIONS)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog X11 GL png)

# copy config.ini
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/config.ini $<TARGET_FILE_DIR:${PROJECT_NAME}>)
