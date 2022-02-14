cmake_minimum_required(VERSION 3.22)
project(tieba_backend)
add_executable(tieba_backend main.cpp global.hpp model/model.hpp model/model.cpp)
target_link_libraries(tieba_backend PUBLIC jsoncpp)