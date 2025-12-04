cmake_minimum_required(VERSION 3.20)
get_filename_component(CURRENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${CURRENT_DIR})

cpp_execute(${PROJECT_NAME})

if(MSVC)
    #设为启动项

    # set_property(DIRECTORY ${CMAKE_BINARY_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME}) 
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${OUT_RUN_PATH})
    set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "XDocument")

    # set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/GL")
    # set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/LTCG")

    # 确保使用正确的预编译头标志
    # target_compile_options(${PROJECT_NAME} PRIVATE 
    #     /Yu"stdafx.h"  # 使用预编译头
    # )
    
    # # 为 stdafx.cpp 单独设置
    # set_source_files_properties(stdafx.cpp
    #     PROPERTIES
    #     COMPILE_FLAGS "/Yc\"stdafx.h\""  # 创建预编译头
    # )

    # target_precompile_headers(${PROJECT_NAME} PRIVATE "include/stdafx.h")
endif()