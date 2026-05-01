cmake_minimum_required(VERSION 3.10)

project(ScoreManage2 VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    Menu.cpp
    ScoreManager.cpp
    Student.cpp
)

set(HEADERS
    Menu.h
    ScoreManager.h
    Student.h
)

add_executable(ScoreManage2 ${SOURCES} ${HEADERS})

if(WIN32)
    target_link_libraries(ScoreManage2 PRIVATE kernel32 user32)
endif()

if(MSVC)
    # MSVC编译器：设置UTF-8编码，解决中文字符编译错误
    # /utf-8 等价于 /source-charset:utf-8 /execution-charset:utf-8
    target_compile_options(ScoreManage2 PRIVATE /W4 /utf-8)
else()
    target_compile_options(ScoreManage2 PRIVATE -Wall -Wextra -O2)
endif()

message(STATUS "======================================")
message(STATUS "  学生成绩管理系统 v${PROJECT_VERSION}")
message(STATUS "======================================")
message(STATUS "CMake配置完成！")
message(STATUS "")
message(STATUS "构建方式：")
message(STATUS "  1. 使用 Ninja:")
message(STATUS "     cmake -B build -G Ninja")
message(STATUS "     cmake --build build")
message(STATUS "")
message(STATUS "  2. 使用 MinGW Makefiles:")
message(STATUS "     cmake -B build -G \"MinGW Makefiles\"")
message(STATUS "     cmake --build build")
message(STATUS "")
message(STATUS "  3. 使用 Visual Studio:")
message(STATUS "     cmake -B build")
message(STATUS "     cmake --build build --config Release")
message(STATUS "======================================")
