cmake_minimum_required(VERSION 3.20)
get_filename_component(CURRENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${CURRENT_DIR})

cpp_execute(${PROJECT_NAME})

if(MSVC)
    #设为启动项

    # set_property(DIRECTORY ${CMAKE_BINARY_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME}) 
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${OUT_RUN_PATH})
    set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "XOOP")

    # --- 关键：设置运行时库和子系统，避免链接错误 ---
    # 为Debug配置设置调试运行时库(/MDd 或 /MTd)
    target_compile_options(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:/MDd>"  # 使用动态调试运行时库（推荐）
        # "$<$<CONFIG:Debug>:/MTd>" # 或者使用静态调试运行时库
    )
    # 为Release配置设置发布运行时库(/MD 或 /MT)
    target_compile_options(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Release>:/MD>"   # 使用动态发布运行时库（推荐）
        # "$<$<CONFIG:Release>:/MT>" # 或者使用静态发布运行时库
    )

    # set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/GL")
    # set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/LTCG")

    # set_target_properties(${PROJECT_NAME} PROPERTIES
    # LINK_FLAGS "/ENTRY:mainCRTStartup")
   
    # 修改入口点
    target_link_options(${PROJECT_NAME} PRIVATE
        "/ENTRY:mainCRTStartup"
    )

    # 设置 Windows 特性
    # set_target_properties(${PROJECT_NAME} PROPERTIES
    #    WIN32_EXECUTABLE TRUE
    # )

endif()