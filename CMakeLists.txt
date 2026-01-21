cmake_minimum_required(VERSION 3.20)

project(NOJ_Solution)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置项目根目录（所有路径都相对于此目录）
set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# 设置第三方库路径（相对路径）
set(THIRD_PARTY_DIR ${PROJECT_ROOT_DIR}/ThirdParty)

# 配置SFML使用静态链接
set(SFML_STATIC_LIBRARIES TRUE)

# Windows平台：使用静态运行时库，避免依赖MSVC运行时DLL
if(MSVC)
    # 使用现代CMake方式设置静态运行时库（CMake 3.15+）
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    # 确保所有目标都使用静态运行时库（如果CMAKE_MSVC_RUNTIME_LIBRARY不支持，使用传统方式）
    if(CMAKE_VERSION VERSION_LESS "3.15")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
    endif()
endif()

# 添加SFML库（使用相对路径）
set(SFML_DIR ${THIRD_PARTY_DIR}/SFML-2.6.1/lib/cmake/SFML)
find_package(SFML 2.6.1 COMPONENTS graphics window system audio REQUIRED)
<<<<<<< Updated upstream
=======

# 设置输出目录为 ./123/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/123)
>>>>>>> Stashed changes

# 添加可执行文件（所有源文件使用相对路径）
add_executable(NOJ_Solution
        # 主程序
        ${PROJECT_ROOT_DIR}/Menu/game.cpp
        
        # Tools目录
        ${PROJECT_ROOT_DIR}/Tools/JsonParser.cpp
        ${PROJECT_ROOT_DIR}/Tools/JsonParser.h
<<<<<<< Updated upstream
        ${PROJECT_ROOT_DIR}/Tools/Timer.cpp
        ${PROJECT_ROOT_DIR}/Tools/Timer.h
        ${PROJECT_ROOT_DIR}/Tools/Music.cpp
        ${PROJECT_ROOT_DIR}/Tools/Music.h
        ${PROJECT_ROOT_DIR}/Game/Gameplay.cpp
        ${PROJECT_ROOT_DIR}/Game/Gameplay.h
=======
        ${PROJECT_ROOT_DIR}/Tools/Image.cpp
        ${PROJECT_ROOT_DIR}/Tools/Image.h
        ${PROJECT_ROOT_DIR}/Tools/Music.cpp
        ${PROJECT_ROOT_DIR}/Tools/Music.h
        ${PROJECT_ROOT_DIR}/Tools/Timer.cpp
        ${PROJECT_ROOT_DIR}/Tools/Timer.h
        
        # FileManager目录
>>>>>>> Stashed changes
        ${PROJECT_ROOT_DIR}/FileManager/Beatmap.cpp
        ${PROJECT_ROOT_DIR}/FileManager/Beatmap.h
        ${PROJECT_ROOT_DIR}/FileManager/Config.cpp
        ${PROJECT_ROOT_DIR}/FileManager/Config.h
        ${PROJECT_ROOT_DIR}/FileManager/Logger.cpp
        ${PROJECT_ROOT_DIR}/FileManager/Logger.h
        ${PROJECT_ROOT_DIR}/FileManager/FileManager.cpp
        ${PROJECT_ROOT_DIR}/FileManager/FileManager.h
        ${PROJECT_ROOT_DIR}/FileManager/Menu.cpp
        ${PROJECT_ROOT_DIR}/FileManager/Menu.h
        ${PROJECT_ROOT_DIR}/FileManager/LocalScore.cpp
        ${PROJECT_ROOT_DIR}/FileManager/LocalScore.h
        
        # Game目录
        ${PROJECT_ROOT_DIR}/Game/Gameplay.cpp
        ${PROJECT_ROOT_DIR}/Game/Gameplay.h
        
        # Menu目录
        ${PROJECT_ROOT_DIR}/Menu/game.cpp
        ${PROJECT_ROOT_DIR}/Menu/game.h
        ${PROJECT_ROOT_DIR}/Menu/select.cpp
        ${PROJECT_ROOT_DIR}/Menu/select.h
        ${PROJECT_ROOT_DIR}/Menu/start_page.cpp
        ${PROJECT_ROOT_DIR}/Menu/start_page.h)

# 配置头文件目录（使用相对路径）
include_directories(
        ${THIRD_PARTY_DIR}/SFML-2.6.1/include
        ${THIRD_PARTY_DIR}/nlohmann_json/include
)

# 链接第三方库（静态链接）
# 当设置了 SFML_STATIC_LIBRARIES TRUE 后，使用标准名称会自动链接静态版本
target_link_libraries(NOJ_Solution PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)

# Windows平台：添加静态链接所需的系统库和SFML依赖
if(WIN32)
    # SFML 静态链接所需的系统库
    target_link_libraries(NOJ_Solution PRIVATE
            opengl32      # OpenGL
            winmm         # Windows多媒体
            gdi32         # GDI图形
            user32        # Windows用户接口
            advapi32      # Windows高级API
    )
    
    # 如果SFML静态库需要额外的第三方库，确保它们也是静态链接的
    # 这些库通常由SFML的CMake配置自动处理，但如果出现链接错误，可能需要手动添加
endif()

# 链接 filesystem 库（不同版本的 MinGW 需要不同的处理）
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # 对于 MinGW 13.1.0，std::filesystem 内置在 libstdc++ 中
    # 但为了避免运行时找不到符号，我们静态链接标准库
    # 这样所有符号都会包含在可执行文件中
    if(WIN32)
        # MinGW-w64: 静态链接 GCC 标准库，避免运行时 DLL 依赖问题
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
    else()
        # Linux 或其他平台：链接 stdc++fs（旧版本需要）
        target_link_libraries(NOJ_Solution PRIVATE stdc++fs)
    endif()
endif()

# 确保配置支持库的编译选项、依赖项和平台兼容性
if(WIN32)
    # 静态链接所有依赖，不需要复制DLL文件
    # 如果SFML静态库需要额外的依赖库，在这里添加
    # 注意：确保使用静态版本的SFML库（lib后缀，而不是dll后缀）
    
    # 创建成品目录结构（beatmap/, logs/, screenshot/）
    add_custom_command(TARGET NOJ_Solution POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/beatmap
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/logs
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/screenshot
            COMMENT "创建成品目录结构"
    )
endif()