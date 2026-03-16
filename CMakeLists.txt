cmake_minimum_required(VERSION 3.10)
project(GTUI)

include_directories(include)

add_library(GTUI STATIC ./src/gtui_impl.c)