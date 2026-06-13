cmake_minimum_required(VERSION 3.13.4)

project(meu_hud_status)

find_package(Geode REQUIRED)

add_geode_mod(
    ${PROJECT_NAME}
    GD_VERSION 2.204
    SOURCES src/main.cpp
)

