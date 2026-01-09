cmake_minimum_required(VERSION 4.0)

project(NOJ_Solution)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置项目根目录（所有路径都相对于此目录）
set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# 设置第三方库路径（相对路径）
set(THIRD_PARTY_DIR ${PROJECT_ROOT_DIR}/ThirdParty)

# 添加SFML库（使用相对路径）
set(SFML_DIR ${THIRD_PARTY_DIR}/SFML-2.6.1/lib/cmake/SFML)
find_package(SFML 2.6.1 COMPONENTS graphics window system REQUIRED)

# 添加可执行文件（所有源文件使用相对路径）
add_executable(NOJ_Solution
        ${PROJECT_ROOT_DIR}/main.cpp
        ${PROJECT_ROOT_DIR}/Tools/JsonParser.cpp
        ${PROJECT_ROOT_DIR}/Tools/JsonParser.h
        ${PROJECT_ROOT_DIR}/FileManager/Beatmap.cpp
        ${PROJECT_ROOT_DIR}/FileManager/Beatmap.h
        ${PROJECT_ROOT_DIR}/FileManager/Config.cpp
        ${PROJECT_ROOT_DIR}/FileManager/Config.h
        ${PROJECT_ROOT_DIR}/FileManager/User.cpp
        ${PROJECT_ROOT_DIR}/FileManager/User.h
        ${PROJECT_ROOT_DIR}/FileManager/Screenshot.cpp
        ${PROJECT_ROOT_DIR}/FileManager/Screenshot.h
        ${PROJECT_ROOT_DIR}/FileManager/Logger.cpp
        ${PROJECT_ROOT_DIR}/FileManager/Logger.h)

# 配置头文件目录（使用相对路径）
include_directories(
        ${THIRD_PARTY_DIR}/SFML-2.6.1/include
        ${THIRD_PARTY_DIR}/nlohmann_json/include
)

# 链接第三方库
target_link_libraries(NOJ_Solution PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)

# 链接 filesystem 库（某些编译器需要）
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_link_libraries(NOJ_Solution PRIVATE stdc++fs)
endif()

# 确保配置支持库的编译选项、依赖项和平台兼容性
if(WIN32)
    # 复制SFML的DLL文件到可执行文件目录（使用相对路径）
    add_custom_command(TARGET NOJ_Solution POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${THIRD_PARTY_DIR}/SFML-2.6.1/bin
            $<TARGET_FILE_DIR:NOJ_Solution>
    )
endif()